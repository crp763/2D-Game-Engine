// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"
#include "SDL/SDL.h"
#include "Math.h"
class SpriteComponent : public Component
{
public:
	// (Lower draw order corresponds with further back)
	SpriteComponent(class Actor* owner, float drawOrder = 100.0f);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer, Vector2 camera);
	virtual void SetTexture(SDL_Texture* texture, int xLoc = 0, int yLoc = 0);

	float GetDrawOrder() const { return mDrawOrder; }
	void SetDrawOrder(float x) { mDrawOrder = x; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }
	int GetXLoc() const { return mTexXLoc; }
	int GetYLoc() const { return mTexYLoc; }
protected:
	SDL_Texture* mTexture;
	float mDrawOrder;
	int mTexWidth;
	int mTexHeight;
	int mTexXLoc;
	int mTexYLoc;
};
