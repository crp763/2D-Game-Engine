#pragma once
#include "SpriteComponent.h"
#include "Actor.h"

inline bool CompareDrawOrder(SpriteComponent* a, SpriteComponent* b)
{
	return(a->GetDrawOrder() < b->GetDrawOrder());
}

inline bool CompareYPos(Actor* a, Actor* b)
{
	return(a->GetYPosition() < b->GetYPosition());
}

inline bool CompareUpdateOrder(Component* a, Component* b)
{
	return (a->GetUpdateOrder() < b->GetUpdateOrder());
}