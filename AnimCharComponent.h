// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SpriteComponent.h"
class AnimCharComponent : public SpriteComponent
{
public:
	AnimCharComponent(class Actor* owner, float drawOrder = 100.0f);
	// Update animation every frame (overriden from component)
	void Update(float deltaTime) override;
	// Set the textures used for animation
	void SetAnimTextures(SDL_Texture* textures);
	// Set/get the animation FPS
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
private:
	// All textures in the animation, assumes a standardized sprite template
	SDL_Texture* mAnimTextures;
	// Previous direction character was facing
	int mLastDirection;
	// Tracks if character was moving on the previous frame
	int mWasMoving;
	// Current animation frame displayed
	int mSpriteFrame;
	// Current frame displayed
	float mCurrFrame;
	// Animation frame rate
	float mAnimFPS;
};
