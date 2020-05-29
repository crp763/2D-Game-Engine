// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
#include "SortFunctions.h"

Actor::Actor(Game* game, int slow)
	:mState(0)
	, mPosition(Vector2::Zero)
	, mScale(1.0f)
	, mRotation(0.0f)
	, mCollisionSize(8.0f)
	, mCollides(1)
	, mGame(game)
	, mZLevel(0)
	, mSlow(slow)
	// Resources
	, mMaxHealth(100)
	, mCurrHealth(100)
	, mHealthMod(0)
	, mMaxMana(100)
	, mCurrMana(100)
	, mManaMod(0)
	, mExp(0)
	, mExpMod(1)
	// Offensive stats array (Phys, Fire, Ice, Lightning, Poison, Time, Gravity, Holy)
	, mOffStats({ 0, 0, 0, 0, 0, 0, 0, 0 })
	// Defensive stats array (Phys, Fire, Ice, Lightning, Poison, Time, Gravity, Holy)
	, mDefStats({ 0, 0, 0, 0, 0, 0, 0, 0 })
	// Meters
	, mTempMeter(0)
	, mPoisonMeter(0)
	, mHolyMeter(0)
{
	mGame->AddActor(this, mSlow);
}

Actor::~Actor()
{
	mGame->RemoveActor(this, mSlow);
	// Need to delete components
	// Because ~Component calls RemoveComponent, need a different style loop
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Actor::SetMovement(int moveState, int moveDirection, float rightSpeed, float downSpeed)
{
	mCharMoveState = moveState;
	mCharDirection = moveDirection;
	mRightSpeed = rightSpeed;
	mDownSpeed = downSpeed;
}

void Actor::SetSpriteSheetPos(int x, int y)
{
	mSpriteSheetXPos = x;
	mSpriteSheetYPos = y;
}

void Actor::Update(float deltaTime)
{
	if (mState == 0)
	{
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
	if (mSprite)
	{
		mSprite->SetDrawOrder(10000*mZLevel + mPosition.y / 100.0f); 
	}
}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime)
{
}

void Actor::Damage(float damage, int type)
{
	int totalDamage = damage - mDefStats[type];
	if (totalDamage > 0)
	{
		mCurrHealth -= totalDamage;
		if (mCurrHealth < 0)
		{
			this->Death();
		}
	}
}

void Actor::Death()
{
	mGame->RemoveActor(this, mSlow);
	this->~Actor();
}

void Actor::Draw(SDL_Renderer* rend, Vector2 camera)
{
	if (mSprite)
	{
		mSprite->Draw(rend, camera);
	}
}

void Actor::AddComponent(Component* component)
{
	// Find the insertion point in the sorted vector
	mComponents.emplace_back(component);
	std::sort(mComponents.begin(), mComponents.end(), CompareUpdateOrder);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}
