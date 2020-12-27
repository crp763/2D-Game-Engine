// ----------------------------------------------------------------
// Each MapGrid object can be used to 
// ----------------------------------------------------------------

#include "MapGrid.h"

MapGrid::MapGrid(int x, int y, int i, int size, Game* g)
	:mGridIndex(i)
	, mGridPosition(x,y)
	, mGridSize(size)
	, mGame(g)
{
}

MapGrid::~MapGrid()
{
}

void MapGrid::AddActor(class Actor* actor)
{
	mActors.emplace_back(actor);
}

void MapGrid::RemoveActor(class Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}


void MapGrid::AddSprite(class SpriteComponent* sprite)
{
	mSprites.emplace_back(sprite);
}

void MapGrid::Draw(SDL_Renderer* rend, Vector2 camera)
{
	if (!mMapTiles.empty())
	{
		for (auto tile : mMapTiles)
		{
			// tile->Draw(rend, camera);
		}
	}
	if (!mSprites.empty())
	{
		for (auto sprite : mSprites)
		{
			sprite->Draw(rend, camera);
		}
		mSprites.clear();
	}
}

int MapGrid::OnScreen(Vector2 mCamera, Settings *mSettings)
{
	if ((32 * (mGridPosition.x + mGridSize) > mCamera.x) && (32 * mGridPosition.x < mCamera.x + mSettings->GetScreenX())) // Check that map tile is within view in X direction
	{
		if ((32 * (mGridPosition.y + mGridSize) > mCamera.y) && (32 * mGridPosition.y > mCamera.y + mSettings->GetScreenY())) // Check that map tile is also within view in the Y direction
		{
			return 1;
		}
	}
	return 0;
}