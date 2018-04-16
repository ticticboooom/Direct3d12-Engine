#pragma once
#include "stdafx.h"
/**
 * @brief controls the falling of objects
 * 
 */
class PhysicsHelper
{
public:
	D3DENGINE_API PhysicsHelper();
	D3DENGINE_API ~PhysicsHelper();
	D3DENGINE_API void Update(float minY, XMVECTOR* objectPosition, bool isRising, BoundingBox collider);
private:
	bool previouslyRising = false; /// was the vector previsouly rising
	float yPos; /// the current y position
	float yPosFallDiff = yPosFallStart; /// fall distance each tick
	const float yPosFallMultiplyer = 1.05f; ///fall speed multiplyer 
	const float yPosFallStart = 0.3f; /// fall speed start
};

