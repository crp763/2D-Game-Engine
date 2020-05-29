// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "InputComponent.h"
#include "Actor.h"

InputComponent::InputComponent(class Actor* owner, int UpdateOrder)
	:MoveComponent(owner, UpdateOrder)
	, mInteractKey(0)
	, mUpKey(0)
	, mDownKey(0)
	, mLeftKey(0)
	, mRightKey(0)
	, mSprintKey(0)
	, mAbility1Key(0)
	, mAbility2Key(0)
	, mAbility3Key(0)
	, mAbility4Key(0)
	, mAbility5Key(0)
{

}

void InputComponent::SetKeys(int interact, int up, int down, int left, int right, int sprint, int a1, int a2, int a3, int a4, int a5)
{
	mInteractKey = interact;
	mUpKey = up;
	mDownKey = down;
	mLeftKey = left;
	mRightKey = right;
	mSprintKey = sprint;
	mAbility1Key = a1;
	mAbility2Key = a2;
	mAbility3Key = a3;
	mAbility4Key = a4;
	mAbility5Key = a5;
}

void InputComponent::Update(float deltaTime)
{
	const Uint8* state = SDL_GetKeyboardState(NULL);
	int direction = mOwner->GetCharDirection();
	int moveState = 0;
	float rightSpeed = 0.0f;
	float downSpeed = 0.0f;
	// right/left
	if (state[mRightKey])
	{
		rightSpeed += 120.0f;
		moveState += 1;
	}
	if (state[mLeftKey])
	{
		rightSpeed -= 120.0f;
		moveState -= 1;
	}
	// up/down
	if (state[mDownKey])
	{
		downSpeed += 120.0f;
		moveState += 10;
	}
	if (state[mUpKey])
	{
		downSpeed -= 120.0f;
		moveState -= 10;
	}
	if (state[mSprintKey])
	{
		downSpeed *= 1.7f;
		rightSpeed *= 1.7f;
	}
	// If character is moving, set their direction equal to curret movement direction. Else, leave direction as-is
	if (moveState != 0)
	{
		direction = moveState;
	}
	mOwner->SetMovement(moveState, direction, rightSpeed, downSpeed);
}
