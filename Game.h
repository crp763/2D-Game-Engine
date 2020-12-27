// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SDL/SDL.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "Settings.h"
#include "MapGrid.h"

class Game
{
public:
	Game(Settings* set);
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor, int slow);
	void RemoveActor(class Actor* actor, int slow);

	void AddActorGrid(class Actor* actor, int i);
	void RemoveActorGrid(class Actor* actor, int i);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void AssignMapGridSprites();
	int Game::GetMapGridIndex(int x, int y); // Returns index of map grid based on input location
	Vector2 Game::GetMapGridPosition(int i); // Returns location of top left corner of any grid by its index

	std::vector<class Actor*> GetActors() { return mActors; }
	std::vector<class Actor*> GetActorsSurroundingGrids(class Actor* actor);

	Vector2 GetCamera() { return mCamera; }
	void SetCamera();
	Vector2 GetMapSize() { return mMapBndry; }
	
	SDL_Texture* GetTexture(const std::string& fileName);
protected:
	Settings* mSettings;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	
	// Map of textures loaded
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// All the actors in the game
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class Actor*> mSlowActors;
	std::vector<class Actor*> mPendingSlowActors;
	std::vector<class Actor*> mDeadActors;

	// All the map grids
	std::vector<class MapGrid*> mMapGrids;


	// All the sprite components owned by the Game() object specifically
	// This should be restricted to background/foreground sprites, like
	// backdrops and weather effects
	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	Vector2 mCamera;   // Position of camera
	Vector2 mMapBndry; // Defines how far camera can move
	Vector2 mMapSize; // Size of map in terms of map grids
	int mGridSize; // Size of each square map grid edge size (in terms of 32px tiles)
	bool mIsRunning;
	// Track if we're updating actors right now
	bool mUpdatingActors;

	// Game-specific
	class PlayerChar* mPlayerChar; // Player's PlayerChar
};
