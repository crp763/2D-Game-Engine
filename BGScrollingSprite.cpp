// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "BGScrollingSprite.h"
#include "Actor.h"
#include "Math.h"
#include <iostream>

BGScrollingSprite::BGScrollingSprite(class Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	,mScrollSpeed(0.0f, 0.0f)
{
}

void BGScrollingSprite::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);
	for (auto& bg : mBGTextures)
	{
		// Update the x offset
		bg.mOffset.x += mScrollSpeed.x * deltaTime;
		bg.mOffset.y += mScrollSpeed.y * deltaTime;
	}
}

void BGScrollingSprite::Draw(SDL_Renderer* renderer, Vector2 camera)
{
	// Draw each background texture
	for (auto& bg : mBGTextures)
	{
		// Now loop the background tiles based on the screen size and tile matrix dimensions in order to create seamless scrolling effect
		// This loop needs to be in Draw() for access to the camera
		while (bg.mOffset.x + (mFixedBG - 1)*camera.x < -mScreenSize.x)
		{
			bg.mOffset.x += mScreenSize.x * mBGTexDim[1];
		}
		while (bg.mOffset.x + (mFixedBG - 1) * camera.x > mScreenSize.x)
		{
			bg.mOffset.x -= mScreenSize.x * mBGTexDim[1];
		}
		while (bg.mOffset.y + (mFixedBG - 1) * camera.y < -mScreenSize.y)
		{
			bg.mOffset.y += mScreenSize.y * mBGTexDim[0];
		}
		while (bg.mOffset.y + (mFixedBG - 1) * camera.y > mScreenSize.y)
		{
			bg.mOffset.y -= mScreenSize.y * mBGTexDim[0];
		}
		SDL_Rect r;
		// Assume screen size dimensions
		r.w = static_cast<int>(mScreenSize.x);
		r.h = static_cast<int>(mScreenSize.y);
		// Center the rectangle around the position of the owner
		r.x = static_cast<int>(mOwner->GetPosition().x + bg.mOffset.x + (mFixedBG-1)*camera.x);
		r.y = static_cast<int>(mOwner->GetPosition().y + bg.mOffset.y + (mFixedBG-1)*camera.y);

		// Draw this background
		SDL_RenderCopy(renderer,
			bg.mTexture,
			nullptr,
			&r
		);
	}
}

void BGScrollingSprite::SetBGTextures(const std::vector<SDL_Texture*>& textures)
{
	mCount = 0;
	for (auto tex : textures)
	{
		BGTexture temp;
		temp.mTexture = tex;
		// Each texture is screen width in offset
		temp.mOffset.x = mCount * mScreenSize.x;
		temp.mOffset.y = 0;
		mBGTextures.emplace_back(temp);
		mCount++;
	}
	mBGTexDim = {1, mCount}; // After passing vector of textures, initially assume all textures align left to right in a row
}

void BGScrollingSprite::SetBGTexDims(int m, int n)
{
	mBGTexDim = { m,n };
	int row = 0, col = 0;
	for (int i = 0; i < mCount; i++)
	{
		mBGTextures[i].mOffset.x = row * mScreenSize.x;
		mBGTextures[i].mOffset.y = col * mScreenSize.y;
		row++;
		if (row == n)
		{
			row = 0;
			col++;
		}
	}
}
