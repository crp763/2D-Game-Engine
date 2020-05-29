// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SpriteComponent.h"
#include <vector>
#include "Math.h"
class BGScrollingSprite : public SpriteComponent
{
public:
	// Set draw order to default to lower (so it's in the background)
	BGScrollingSprite(class Actor* owner, int drawOrder = 0);
	// Update/draw overriden from parent
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer, Vector2 camera) override;
	// Set the textures used for the background
	void SetBGTextures(const std::vector<SDL_Texture*>& textures);
	void SetBGTexDims(int m, int n);
	// Get/set screen size and scroll speed
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetSpriteSize(const Vector2& size) { mSpriteSize = size; }
	void SetScrollSpeed(float speedX, float speedY) { mScrollSpeed.x = speedX, mScrollSpeed.y = speedY; }
	void SetFixedBG(int yn) { mFixedBG = yn; }
	Vector2 GetScrollSpeed() const { return mScrollSpeed; }
private:
	// Struct to encapsulate each bg image and its offset
	struct BGTexture
	{
		SDL_Texture* mTexture;
		Vector2 mOffset; // The way I've reprogrammed this class, there's no reason for mOffset to exist more than once but I'm too lazy to reprogram all the logic
	};
	int mCount; // Keep track of number of elements because std::vector.size() is not a signed int and I don't want to write a bunch of logic to cast
	std::vector<BGTexture> mBGTextures;
	Vector2 mScreenSize;
	Vector2 mSpriteSize; // Assumes all textures are the same size
	std::vector<int> mBGTexDim = { 0,0 }; // Maps row vector of BG textures into an m-by-n matrix
	Vector2 mScrollSpeed;
	int mFixedBG = 0; // If 1, BG is fixed with respect to the camera. If 0, BG moves with the camera
};
