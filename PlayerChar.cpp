// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "PlayerChar.h"
#include "Game.h"
#include "Actor.h"

PlayerChar::PlayerChar(Game* game)
	:Actor(game, 0)
{
	// Create an animated sprite component
	asc = new AnimCharComponent(this);
	SDL_Texture* anims = game->GetTexture("Assets/dwarf1.png");
	asc->SetAnimTextures(anims);
	inp = new InputComponent(this);
	move = new MoveComponent(this);
	col = new MoveCollisionComponent(this);
}

PlayerChar::~PlayerChar()
{
	// Need to delete components
	// Because ~Component calls RemoveComponent, need a different style loop
	delete asc;
	delete inp;
	delete move;
	delete col;
}

void PlayerChar::SetKeys(int interact, int up, int down, int left, int right, int sprint, int a1, int a2, int a3, int a4, int a5)
{
	inp->SetKeys(interact, up, down, left, right, sprint, a1, a2, a3, a4, a5);
}

//void PlayerChar::UpdateActor(float deltaTime)
//{
//	inp->ProcessInput();
//	Actor::UpdateActor(deltaTime);
//	// Update position based on speeds and delta time
//	if (mSprite)
//	{
//		mSprite->SetDrawOrder(10000*mZLevel + mPosition.y / 100.0f);
//	}
//}
