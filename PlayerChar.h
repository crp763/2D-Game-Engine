// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include "AnimCharComponent.h"
#include "InputComponent.h"

class PlayerChar : public Actor
{
public:
	PlayerChar(class Game* game);
	virtual ~PlayerChar();
	void UpdateActor(float deltaTime) override;
	float GetRightSpeed() const { return mRightSpeed; }
	float GetDownSpeed() const { return mDownSpeed; }
	void SetCameraControl(int x) { inp->SetCameraControl(x); }
	void SetKeys(int interact, int up, int down, int left, int right, int sprint, int a1, int a2, int a3, int a4, int a5);
private:
	// Int to store characters current movement state
	AnimCharComponent* asc;
	InputComponent* inp;
};