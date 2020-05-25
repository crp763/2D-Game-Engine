// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Math.h"

SpriteComponent::SpriteComponent(Actor* owner, float drawOrder)
	:Component(owner)
	,mTexture(nullptr)
	,mDrawOrder(drawOrder)
	,mTexWidth(0)
	,mTexHeight(0)
	,mTexXLoc(0)
	,mTexYLoc(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer, Vector2 camera)
{
	if (mTexture)
	{
		float scale = mOwner->GetScale();
		SDL_Rect r;
		// Scale the width/height by owner's scale
		r.w = static_cast<int>(mTexWidth * scale);
		r.h = static_cast<int>(mTexHeight * scale);
		// Center the rectangle around the position of the owner
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2 - camera.x);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2 - camera.y);

		SDL_Rect f;
		f = r;
		f.w /= scale;
		f.h /= scale;
		f.x = mTexXLoc;
		f.y = mTexYLoc;

		// Draw (have to convert angle from radians to degrees, and clockwise to counter)
		SDL_RenderCopyEx(renderer,
			mTexture,
			&f,
			&r,
			-Math::ToDegrees(mOwner->GetRotation()),
			nullptr,
			SDL_FLIP_NONE);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture, int xLoc, int yLoc)
{
	mTexture = texture;
	// Set width/height
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
	mTexWidth /= 12;
	mTexHeight /= 8;
	mTexXLoc = static_cast<int>(mTexWidth*xLoc);
	mTexYLoc = static_cast<int>(mTexHeight*yLoc);
}
