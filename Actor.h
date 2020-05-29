// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include "Math.h"
#include "SpriteComponent.h"
#include <iostream>

class Actor
{
public:

	Actor(class Game* game, int slow);
	virtual ~Actor();

	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// Updates all the components attached to the actor (not overridable)
	void UpdateComponents(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void UpdateActor(float deltaTime);
	// Draw sprite owned by actor
	virtual void Draw(SDL_Renderer* rend, Vector2 camera);

	// Getters/setters
	const Vector2& GetPosition() const { return mPosition; }
	SpriteComponent* GetSprite() { return mSprite; }
	float GetYPosition() const { return mPosition.y; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetCollisionSize(float collisionSize) { mCollisionSize = collisionSize; }
	float GetCollisionSize() const { return mCollisionSize; }
	void SetCollisionBehavior(int collides) { mCollides = collides; }
	int GetCollisionBehavior() const { return mCollides; }
	int GetMoveState() { return mCharMoveState; }
	int GetCharDirection() { return mCharDirection; }
	int GetRightSpeed() { return mRightSpeed; }
	int GetDownSpeed() { return mDownSpeed; }
	int GetSpriteSheetXPos() { return mSpriteSheetXPos; }
	int GetSpriteSheetYPos() { return mSpriteSheetYPos; }
	void SetRotation(float rotation) { mRotation = rotation; }
	void SetMovement(int moveState, int moveDirection, float rightSpeed, float downSpeed);
	void SetSpriteSheetPos(int x, int y);
	void SetSprite(SpriteComponent* sprite) { mSprite = sprite; }
	int GetZLevel() const { return mZLevel; }
	void SetZLevel(int z) { mZLevel = z; }

	// Combat Related Functions
	void Damage(float damage, int type);
	void Death();
	float GetHealthPct() { return mCurrHealth / mMaxHealth; }
	float GetManaPct() { return mCurrMana / mMaxMana; }
	float GetExpPct() { return mExp / 100; }

	int GetState() const { return mState; }
	void SetState(int state) { mState = state; }

	class Game* GetGame() { return mGame; }


	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

protected:
	// Sprite tied to actor
	SpriteComponent* mSprite;

	// Actor's state
	int mState; // -1 = dead, 0 = normal, 1 = stunned

	// Transform
	Vector2 mPosition;
	float mScale;
	float mRotation;
	float mCollisionSize;
	int mCollides;

	// Movement
	int mCharMoveState; // Make two digits, first digit is vertical movement state of {-1,0,1}, second char is horitzontal movement state
	int mCharDirection; // Basically just stores the previous mCharMoveState, but won't zero out when the character stops moving
	float mRightSpeed;
	float mDownSpeed;

	// For finding the right sprite
	int mSpriteSheetXPos;
	int mSpriteSheetYPos;

	// For z-location, needed for draw order
	int mZLevel;

	// Whether the actors updates slowly (every 10th tick if 1) or every tick (if 0)
	int mSlow;

	// :::::::::: CHARACTER GAME STATS ::::::::::
	// Resources
	int mMaxHealth = 100;
	int mCurrHealth = 100;
	int mHealthMod = 0;
	int mMaxMana = 100;
	int mCurrMana = 100;
	int mManaMod = 0;
	int mExp = 0;
	int mExpMod = 1;
	// Offensive stats array (Phys, Fire, Ice, Lightning, Poison, Time, Gravity, Holy)
	std::vector<int> mOffStats = { 0, 0, 0, 0, 0, 0, 0, 0 };
	// Defensive stats array (Phys, Fire, Ice, Lightning, Poison, Time, Gravity, Holy)
	std::vector<int> mDefStats = { 0, 0, 0, 0, 0, 0, 0, 0 };
	// Meters
	int mTempMeter = 0;
	int mPoisonMeter = 0;
	int mHolyMeter = 0;


	std::vector<class Component*> mComponents;
	class Game* mGame;
};
