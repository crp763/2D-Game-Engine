// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AnimCharComponent.h"
#include "Actor.h"
#include "PlayerChar.h"
#include "Math.h"

AnimCharComponent::AnimCharComponent(Actor* owner, float drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mAnimTextures(nullptr)
	, mLastDirection(0)
	, mWasMoving(0)
	, mSpriteFrame(0)
	, mCurrFrame(0.0f)
	, mAnimFPS(5.0f)
{
}

void AnimCharComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);
	int moveState, direction, yLoc, renderFrame, xOffset, yOffset;
	moveState = mOwner->GetMoveState();
	direction = mOwner->GetCharDirection();
	xOffset = mOwner->GetSpriteSheetXPos();
	yOffset = mOwner->GetSpriteSheetYPos();

	// Update current frame based on framerate and deltaTime
	mCurrFrame += mAnimFPS * deltaTime;
	// Check which direction sprite should be facing
	switch (moveState) // There are 9 possible states for still, then all of N,NE,E,SE,S,SW,W,NW
	{
	case 0:
		yLoc = mLastDirection;
		mCurrFrame = 1;
		break;
	case -11: // NW, don't change direction if player was already moving either N or W
		if (mLastDirection == 3) { yLoc = 3; }
		else if (mLastDirection == 1) { yLoc = 1; }
		else { yLoc = 1; }
		break;
	case -10:
		yLoc = 3; // N = face up
		break;
	case -9: // NE
		if (mLastDirection == 3) { yLoc = 3; }
		else if (mLastDirection == 2) { yLoc = 2; }
		else { yLoc = 2; }
		break;
	case -1:
		yLoc = 1; // W = face left
		break;
	case 1:
		yLoc = 2; // E = face right
		break;
	case 9: // SW
		if (mLastDirection == 0) { yLoc = 0; }
		else if (mLastDirection == 1) { yLoc = 1; }
		else { yLoc = 1; }
		break;
	case 10:
		yLoc = 0; // S = face down
		break;
	case 11: // SE
		if (mLastDirection == 0) { yLoc = 0; }
		else if (mLastDirection == 2) { yLoc = 2; }
		else { yLoc = 2; }
		break;
	}
	if (!mWasMoving && moveState) { mCurrFrame = 2.0f; }

	if (moveState != 0) { mWasMoving = 1; } else { mWasMoving = 0; }
	// If character started walking from a stop, then 
	mLastDirection = yLoc; // Store previous direction to be read for next iteration
	// Wrap current frame if needed, assumes all character sprite sheets use 3-frame animations
	renderFrame = static_cast<int>(mCurrFrame);
	switch (renderFrame)
	{
	case 3:
		renderFrame = 1;
		break;
	case 4:
		mCurrFrame -= 4.0f;
		renderFrame = 0;
	}
	// Set the current texture
	SetTexture(mAnimTextures,renderFrame + xOffset,yLoc+yOffset);
}

void AnimCharComponent::SetAnimTextures(SDL_Texture* textures)
{
	mAnimTextures = textures;
	// Set the active texture to first frame
	mCurrFrame = 0.0f;
	SetTexture(mAnimTextures, 1, 0); // 1,0 is the location of the standing facing down sprite on the standard sprite sheets
}
