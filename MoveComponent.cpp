// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, float updateOrder)
	:Component(owner, updateOrder)
{

}

void MoveComponent::Update(float deltaTime)
{
	// Update position based on speeds and delta time
	Vector2 pos = mOwner->GetPosition();
	float rightSpeed = mOwner->GetRightSpeed();
	float downSpeed = mOwner->GetDownSpeed();
	if (rightSpeed || downSpeed)
	{
		pos.x += rightSpeed * deltaTime;
		pos.y += downSpeed * deltaTime;
		mOwner->SetPosition(pos);
	}
}
