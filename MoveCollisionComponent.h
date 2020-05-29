// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"
#include "Game.h"

class MoveCollisionComponent : public Component
{
public:
	// Lower update order to update first
	MoveCollisionComponent(class Actor* owner, float updateOrder = 11); // MoveComponent is order 10, so occurs right after moving

	void Update(float deltaTime) override;

protected:
};
