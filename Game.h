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

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	std::vector<class Actor*> GetActors() { return mActors; }

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
	Vector2 mCamera;
	Vector2 mMapBndry;
	bool mIsRunning;
	// Track if we're updating actors right now
	bool mUpdatingActors;

	// Game-specific
	class PlayerChar* mPlayerChar; // Player's PlayerChar
};
