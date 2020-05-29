// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once

class Settings
{
public:
	Settings(float x, float y);

	float GetScreenX() const { return mScreenX; }
	float GetScreenY() const { return mScreenY; }
	float SetScreenSize(float x, float y) { mScreenX = x; mScreenY = y; }
protected:
	float mScreenX;
	float mScreenY;
};
