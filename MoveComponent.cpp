// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
	, mControlCamera(0)
{

}

void MoveComponent::Update(float deltaTime)
{
	// Update position based on speeds and delta time
	Vector2 pos = mOwner->GetPosition();
	float rightSpeed = mOwner->GetRightSpeed();
	float downSpeed = mOwner->GetDownSpeed();
	pos.x += rightSpeed * deltaTime;
	pos.y += downSpeed * deltaTime;
	if (mControlCamera == 1)
	{
		// Move camera if character moves so far
		Game* game = mOwner->GetGame();
		Vector2 camera = game->GetCamera();
		Vector2 diff(pos.x - camera.x, pos.y - camera.y);
		if (diff.x < 300.0f)
		{
			camera.x = pos.x - 300.0f;
		}
		else if (diff.x > 724.0f)
		{
			camera.x = pos.x - 724.0f;
		}
		if (diff.y < 200.0f)
		{
			camera.y = pos.y - 200.0f;
		}
		else if (diff.y > 568.0f)
		{
			camera.y = pos.y - 568.0f;
		}
		game->SetCamera(camera.x, camera.y);
	}
	mOwner->SetPosition(pos);
}
