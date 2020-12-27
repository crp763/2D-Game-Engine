#pragma once
#include <vector>
#include "Math.h"
#include "SpriteComponent.h"
#include <iostream>

class MapTile
{
public:
	MapTile(class Game* game, int slow);
	virtual ~MapTile();

	// Any actor-specific update code (overridable)
	virtual void UpdateTile(float deltaTime);
	// Draw sprite owned by actor
	virtual void Draw(SDL_Renderer* rend, Vector2 camera);

protected:
	// Sprite tied to actor
	SpriteComponent* mSprite;

	Vector2 mPosition;
};

