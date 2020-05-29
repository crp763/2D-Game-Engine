// ----------------------------------------------------------------
// Each MapGrid object can be used to 
// ----------------------------------------------------------------

#include "MapGrid.h"

MapGrid::MapGrid(int x, int y, int i, Game* g)
	:mGridIndex(i)
	, mGridPosition(x,y)
	, mGame(g)
{
}

MapGrid::~MapGrid()
{
}

MapGrid::Draw()
{
	if (!mMapTiles.empty())
	{
		for (auto tile : mMapTiles)
		{
			mMapTiles.Draw();
		}
	}
	if (!mSprites.empty())
	{
		for (auto sprite : mSprites)
		{
			sprite.Draw();
		}
	}
}