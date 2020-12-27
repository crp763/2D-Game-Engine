// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include "SDL/SDL_image.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "PlayerChar.h"
#include "BGScrollingSprite.h"
#include "SortFunctions.h"
#include <math.h>
#include <fstream>
#include <iostream>
#include "ReadMapCSV.h"

Game::Game(Settings* set)
	:mWindow(nullptr)
	, mSettings(set)
	, mRenderer(nullptr)
	, mIsRunning(true)
	, mUpdatingActors(false)
	, mCamera(0.0f, 0.0f)
	, mMapBndry(2000.0f, 2000.0f)
	, mMapSize(10, 10)
	, mGridSize(10)
{
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 2)", 100, 100, mSettings->GetScreenX(), mSettings->GetScreenY(), 0);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	LoadData();

	mTicksCount = SDL_GetTicks();
	
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

}

void Game::UpdateGame()
{
	// Some processes will only be updated at larger intervals, to
	// help reduce CPU loading
	float slowDeltaTime = 0;
	// Compute delta time
	// Wait until 16ms has elapsed since last frame for 60fps
	// Wait until 32ms has elapsed since last frame for 33fps
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 33))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.1f)
	{
		std::cout << deltaTime << std::endl;
		deltaTime = 0.1f;
	}
	mTicksCount = SDL_GetTicks();
	slowDeltaTime += deltaTime;

	// Update all actors
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	this->SetCamera();

	if (slowDeltaTime > 1.00)
	{
		for (auto actor : mSlowActors)
		{
			actor->Update(slowDeltaTime);
		}
		slowDeltaTime = 0.0f;
	}

	// Add pending actors
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();
	for (auto pending : mPendingSlowActors)
	{
		mSlowActors.emplace_back(pending);
	}
	mPendingSlowActors.clear();
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);
	
	// Draw all sprite components assigned to entire game: TEMPORARY
	std::sort(mSprites.begin(), mSprites.end(), CompareDrawOrder);
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer, mCamera);
	}

	// Assign all sprites to their map grids
    AssignMapGridSprites();

	// Draw all sprites by looping through all map grids to be rendered
	// Map grids must be drawn from the bottom up (in the -Y direction) for the Z-position layering to work properly
	for (auto grid : mMapGrids)
	{
		if (grid->OnScreen(mCamera, mSettings)) // returns true if the grid is in camera view, or if it's the map grid at (0,0) where backgrounds/foregrounds are stored
		{
			grid->Draw(mRenderer, mCamera);
		}
	}

	// :::::::::: Draw player UI ::::::::::
	// UI Background
	SDL_SetRenderDrawColor(mRenderer, 150, 150, 115, 255);
	SDL_Rect UI; 
	UI.w = mSettings->GetScreenX();
	UI.h = 150;
	UI.x = 0;
	UI.y = mSettings->GetScreenY() - 150;
	SDL_RenderFillRect(mRenderer, &UI);
	// Health, mana, xp bars
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_Rect Health = UI;
	Health.x += 20;
	Health.y += 15;
	Health.w = 300;
	Health.h = 25; 
	SDL_RenderFillRect(mRenderer, &Health);
	SDL_Rect Mana = Health; Mana.y += 35;
	SDL_RenderFillRect(mRenderer, &Mana);
	SDL_Rect Exp = Mana; Exp.y += 35;
	SDL_RenderFillRect(mRenderer, &Exp);
	Health.w *= mPlayerChar->GetHealthPct();
	SDL_SetRenderDrawColor(mRenderer, 200, 0, 0, 255);
	SDL_RenderFillRect(mRenderer, &Health);
	Mana.w *= mPlayerChar->GetManaPct();
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 200, 255);
	SDL_RenderFillRect(mRenderer, &Mana);
	Exp.w *= mPlayerChar->GetExpPct();
	SDL_SetRenderDrawColor(mRenderer, 200, 200, 0, 255);
	SDL_RenderFillRect(mRenderer, &Exp);


	SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	// ::::::::: CREATE MAP ::::::::::
	int i = mMapSize.x * mMapSize.y - 1; // Map grid index (starts from 0 at top left corners, goes left-to-right then top-to-bottom in index order)
	for (int m = mMapSize.y; m > 0; m--) // Loop over y direction (bottom to top)
	{
		for (int n = mMapSize.x; n > 0; n--) // Loop over x direction (right to left)
		{
			MapGrid* grid = new MapGrid((n - 1) * mGridSize * 32, (m - 1) * mGridSize * 32, i, mGridSize, this); // Assign position of top left corner, indix, grid size (number of 32px tiles on each edge), and game*
			mMapGrids.emplace_back(grid);
			std::cout << "Map Grid " << i << ", X = " << ((n - 1) * mGridSize * 32) << ", Y = " << (m - 1) * mGridSize * 32 << std::endl;
			i--;
		}
	}

	std::vector<std::string> mapFiles = {
		"Map01/test_map_01_Tile_Layer_1.csv",
		"Map01/test_map_01_Tile_Layer_2.csv"
	};
	std::vector<SDL_Texture*> mapTilesets = {
		GetTexture("Assets/LPC_Base_Assets/tiles/grass.png"),
		GetTexture("Assets/LPC_Base_Assets/tiles/castle_outside.png")
	};
	std::string csvLine;
	std::vector<int> csvRead1;
	ReadMapCSV(mapFiles[0], csvRead1);
	std::vector<int> csvRead2;

	// Create player's PlayerChar
	mPlayerChar = new PlayerChar(this);
	mPlayerChar->SetPosition(Vector2(400.0f, 384.0f));
	mPlayerChar->SetZLevel(3);
	mPlayerChar->SetScale(1.3f);
	mPlayerChar->SetSpriteSheetPos(3, 4);
	mPlayerChar->SetKeys(SDL_SCANCODE_SPACE, SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D,
		SDL_SCANCODE_LSHIFT, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4, SDL_SCANCODE_5);

	int j = 0;
	int k = 0;
	for (int i = 0; i < 1800; i++)
	{
		if (k > 50) { k = 0; j = j + 1; }
		new PlayerChar(this);
		mPendingActors.back()->SetPosition(Vector2(400.0f + 50.0f*j, 100.0f + 50.0f*k));
		mPendingActors.back()->SetZLevel(3);
		mPendingActors.back()->SetScale(1.8f);
		mPendingActors.back()->SetSpriteSheetPos(6,4);
		k = k + 1;

	}

	// Create actor for the background (this doesn't need a subclass)
	Actor* bgAct = new Actor(this, 0);
	bgAct->SetZLevel(0);
	bgAct->SetPosition(Vector2(0.0f, 0.0f));
	// Create the "far back" background
	BGScrollingSprite* bg = new BGScrollingSprite(bgAct);
	bg->SetScreenSize(Vector2(mSettings->GetScreenX(), mSettings->GetScreenY()));
	bg->SetSpriteSize(Vector2(1024.0f, 768.0f));
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png"),
		GetTexture("Assets/Farback02.png"),
		GetTexture("Assets/Farback01.png")
	};
	bg->SetFixedBG(1);
	bg->SetBGTextures(bgtexs);
	bg->SetBGTexDims(2, 2);
	bg->SetScrollSpeed(0.0f,0.0f);
	// Create the closer background
	Actor* fgAct = new Actor(this, 0);
	fgAct->SetZLevel(10);
	fgAct->SetPosition(Vector2(0.0f, 0.0f));
	/*bg = new BGScrollingSprite(fgAct, 50);
	bg->SetScreenSize(Vector2(mSettings->GetScreenX(), mSettings->GetScreenY()));
	bg->SetSpriteSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
	};
	bg->SetFixedBG(0);
	bg->SetBGTextures(bgtexs);
	bg->SetBGTexDims(3, 3);
	bg->SetScrollSpeed(-300.0f, 200.0f);*/
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// Is the texture already in the map?
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// Load from file
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		// Create texture from surface
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor, int slow)
{

	// If we're updating actors, need to add to pending
	if (slow == 0)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mPendingSlowActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor, int slow)
{
	if (slow == 0)
	{
		auto iter = std::find(mActors.begin(), mActors.end(), actor);
		if (iter != mActors.end())
		{
			// Swap to end of vector and pop off (avoid erase copies)
			std::iter_swap(iter, mActors.end() - 1);
			mActors.pop_back();
		}
	}
	else
	{
		auto iter = std::find(mSlowActors.begin(), mSlowActors.end(), actor);
		if (iter != mSlowActors.end())
		{
			// Swap to end of vector and pop off (avoid erase copies)
			std::iter_swap(iter, mSlowActors.end() - 1);
			mSlowActors.pop_back();
		}
	}
}

void Game::AddActorGrid(class Actor* actor, int i)
{
	mMapGrids[i]->AddActor(actor);
}

void Game::RemoveActorGrid(class Actor* actor, int i)
{
	mMapGrids[i]->RemoveActor(actor);
}

void Game::AddSprite(SpriteComponent* sprite)
{
	{
		mSprites.emplace_back(sprite);
		std::sort(mSprites.begin(), mSprites.end(), CompareDrawOrder);
	}
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	if (iter != mSprites.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mSprites.end() - 1);
		mSprites.pop_back();
		std::sort(mSprites.begin(), mSprites.end(), CompareDrawOrder);
	}
}

void Game::AssignMapGridSprites()
{
	for (auto sprite : mSprites)
	{
		int i = this->GetMapGridIndex(sprite->GetXLoc(), sprite->GetYLoc());
		if (i != -1)
		{
			mMapGrids[i]->AddSprite(sprite);
		}
		// sprite->Draw(mRenderer, mCamera);
	}
}

int Game::GetMapGridIndex(int x, int y)
{
	int n = (int) x / (32*mGridSize);
	int m = (int) y / (32*mGridSize);
	int i = (m * mMapSize.x) + n;
	if ((i < mMapSize.x * mMapSize.y) && (i >= 0))
	{
		return i;
	}
	return -1;
}

Vector2 Game::GetMapGridPosition(int i)
{
	Vector2 pos;
	pos.y = (int) i / mMapSize.x;
	pos.y = i - mMapSize.x * pos.y;
	return pos;
}

std::vector<class Actor*> Game::GetActorsSurroundingGrids(class Actor* actor)
{
	int quad = 0; // Determine which quadrant of the current grid the actor is standing in
	int i = actor->GetGrid();
	std::vector<class Actor*> list; // List of actors to return
	std::vector<int> border_inds;
	if (actor->GetGrid() != -1)
	{
		list = mMapGrids[i]->GetActors();
		Vector2 pos = actor->GetPosition() - mMapGrids[i]->GetPosition();
		if (pos.x > 16 * mGridSize)
		{
			quad++;
		}
		if (pos.y > 16 * mGridSize)
		{
			quad = quad + 2;
		}
		switch (quad) {
		case 0 :
			border_inds.insert(border_inds.end(), i - 1);
			border_inds.insert(border_inds.end(), i - mMapSize.x);
			border_inds.insert(border_inds.end(), i - 1 - mMapSize.x);
		case 1 :
			border_inds.insert(border_inds.end(), i + 1);
			border_inds.insert(border_inds.end(), i - mMapSize.x);
			border_inds.insert(border_inds.end(), i + 1 - mMapSize.x);
		case 2 :
			border_inds.insert(border_inds.end(), i - 1);
			border_inds.insert(border_inds.end(), i + mMapSize.x);
			border_inds.insert(border_inds.end(), i - 1 + mMapSize.x);
		case 3 :
			border_inds.insert(border_inds.end(), i + 1);
			border_inds.insert(border_inds.end(), i + mMapSize.x);
			border_inds.insert(border_inds.end(), i + 1 + mMapSize.x);
		}
		for (auto ind : border_inds)
		{
			if (ind != -1)
			{
				std::vector<class Actor*> append = mMapGrids[i]->GetActors();
				list.insert(list.end(), append.begin(), append.end());
				append.clear();
			}
		}
	}
	return list;
}

void Game::SetCamera()
{
	Vector2 PlayerPos = mPlayerChar->GetPosition();
	mCamera.x = PlayerPos.x - mSettings->GetScreenX() / 2;
	mCamera.y = PlayerPos.y - (mSettings->GetScreenY() - 75) / 2;
	if (mCamera.x < 0)
	{
		mCamera.x = 0;
	}
	else if (mCamera.x + mSettings->GetScreenX() > mMapBndry.x)
	{
		mCamera.x = mMapBndry.x - mSettings->GetScreenX();

	}
	if (mCamera.y < 0)
	{
		mCamera.y = 0;
	}
	else if (mCamera.y + mSettings->GetScreenY() - 150 > mMapBndry.y)
	{
		mCamera.y = mMapBndry.y - mSettings->GetScreenY() + 150;

	}
}