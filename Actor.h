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
class Actor
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// Updates all the components attached to the actor (not overridable)
	void UpdateComponents(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void UpdateActor(float deltaTime);

	// Getters/setters
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	int GetMoveState() { return mCharMoveState; }
	int GetCharDirection() { return mCharDirection; }
	int GetRightSpeed() { return mRightSpeed; }
	int GetDownSpeed() { return mDownSpeed; }
	int GetSpriteSheetXPos() { return mSpriteSheetXPos; }
	int GetSpriteSheetYPos() { return mSpriteSheetYPos; }
	void SetRotation(float rotation) { mRotation = rotation; }
	void SetMovement(int moveState, int moveDirection, float rightSpeed, float downSpeed);
	void SetSpriteSheetPos(int x, int y);

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }


	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	// For std::sort
	bool operator< (const Actor &other) const { return mPosition.y < other.mPosition.y; }
protected:
	// Actor's state
	State mState;

	// Transform
	Vector2 mPosition;
	float mScale;
	float mRotation;

	// Movement
	int mCharMoveState; // Make two digits, first digit is vertical movement state of {-1,0,1}, second char is horitzontal movement state
	int mCharDirection; // Basically just stores the previous mCharMoveState, but won't zero out when the character stops moving
	float mRightSpeed;
	float mDownSpeed;

	// For finding the right sprite
	int mSpriteSheetXPos;
	int mSpriteSheetYPos;

	std::vector<class Component*> mComponents;
	class Game* mGame;
};
