#pragma once
#include "stdafx.h"
class PhysicsHelper
{
public:
	D3DENGINE_API PhysicsHelper();
	D3DENGINE_API ~PhysicsHelper();
	D3DENGINE_API void Update(float minY, XMVECTOR* objectPosition, bool isRising, BoundingBox collider);
private:
	bool previouslyRising = false;
	float yPos;
	float yPosFallDiff = yPosFallStart;
	const float yPosFallMultiplyer = 1.05f;
	const float yPosFallStart = 0.3f;
};

