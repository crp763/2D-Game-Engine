// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MoveCollisionComponent.h"
#include "Actor.h"
#include "Game.h"
#include <math.h>

MoveCollisionComponent::MoveCollisionComponent(class Actor* owner, float updateOrder)
	:Component(owner, updateOrder)
{

}

void MoveCollisionComponent::Update(float deltaTime)
{
	if (mOwner->GetRightSpeed() || mOwner->GetDownSpeed()) // Only need to check for collision if this actor moved
	{
		Vector2 charPos = mOwner->GetPosition();
		float charSize = mOwner->GetCollisionSize() * mOwner->GetScale();
		Game* game = mOwner->GetGame();
		std::vector<class Actor*> actors = game->GetActorsSurroundingGrids(mOwner);
		for (auto actor : actors)
		{
			if (actor != mOwner && actor->GetCollisionBehavior())
			{
				Vector2 otherPos = actor->GetPosition();
				float otherSize = actor->GetCollisionSize() * actor->GetScale();
				if (abs(charPos.x - otherPos.x) > abs(charPos.y - otherPos.y))
				{
					if (abs(charPos.y - otherPos.y) < charSize + otherSize)
					{
						if (charPos.x > otherPos.x && charPos.x < otherPos.x + charSize + otherSize)
						{
							charPos.x = otherPos.x + charSize + otherSize;
						}
						else if (charPos.x < otherPos.x && charPos.x + charSize + otherSize > otherPos.x)
						{
							charPos.x = otherPos.x - charSize - otherSize;
						}
					}
				}
				else
				{
					if (abs(charPos.x - otherPos.x) < charSize + otherSize)
					{
						if (charPos.y > otherPos.y && charPos.y < otherPos.y + charSize + otherSize)
						{
							charPos.y = otherPos.y + charSize + otherSize;
						}
						else if (charPos.y < otherPos.y && charPos.y + charSize + otherSize > otherPos.y)
						{
							charPos.y = otherPos.y - charSize - otherSize;
						}
					}
				}
				mOwner->SetPosition(charPos);
			}
		}
	}
}
