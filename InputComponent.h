// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <cstdint>
#include "MoveComponent.h"
#include "Component.h"

class InputComponent : public MoveComponent
{
public:
	// Lower update order to update first
	InputComponent(class Actor* owner, int UpdateOrder = 9);

	void Update(float deltaTime) override;

	// Getters/setters for private variables

	void SetKeys(int interact, int up, int down, int left, int right, int sprint, int a1, int a2, int a3, int a4, int a5);
private:

	// Key for interacting
	int mInteractKey;

	// Keys for movement
	int mUpKey;
	int mDownKey;
	int mLeftKey;
	int mRightKey;
	int mSprintKey;

	// Keys for abilities
	int mAbility1Key;
	int mAbility2Key;
	int mAbility3Key;
	int mAbility4Key;
	int mAbility5Key;
};
