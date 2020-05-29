// ----------------------------------------------------------------
// This class represents a single X-by-X map grid. Each grid starts
// every game tick empty; as actors are updated, they will be placed
// to the appropriate grid based on position. After actors are
// updated, each map grid will sort them based on Y position from 
// low to high, in order to simulate an isometric camera. Finally,
// the map grid will draw all the sprites associated with each actor
// where applicable. 

#pragma once
#include <vector>
#include "Math.h"
#include "Game.h"

class MapGrid
{
public:
	MapGrid(int x, int y, int i, Game* g); // Constructor
	virtual ~MapGrid(); // Destructor

	// Update function called from Game (not overridable)
	virtual void Update(float deltaTime);
	// Draw all the actors in the grid
	void Draw();

	class Game* GetGame() { return mGame; }

protected:
	class Game* mGame;
	// Which map grid am I?
	int mGridIndex;
	// Where is my top-left corner?
	Vector2 mGridPosition;
	// How big is the map grid? Single int representing size (in tiles) of each side of the square
	int mGridSize;

	// Vector of all the map tiles to draw to this grid (usually won't get changed after initializing)
	std::vector<class MapTile*> mMapTiles; // All the map tiles, which are not tied to an actor

	// Vector of all actors in the grid
	std::vector<class Actor*> mActors; // Actors in the grid
	std::vector<class SpriteComponent*> mSprites; // Sprites belonging to actors in the grid
};
