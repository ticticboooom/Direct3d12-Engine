#include "stdafx.h"
#include "PhysicsHelper.h"



PhysicsHelper::PhysicsHelper()
{
	yPos = 0;
	yPosFallDiff = yPosFallStart;
}

PhysicsHelper::~PhysicsHelper()
{
}

void PhysicsHelper::Update(float minY, XMVECTOR* objectPosition, bool isRising, BoundingBox collider)
{
	if (XMVectorGetY(*objectPosition) - collider.Extents.y >= minY && !isRising) {
		if (previouslyRising) {
			previouslyRising = false;
			yPos = XMVectorGetY(*objectPosition);
		}
		yPosFallDiff *= yPosFallMultiplyer;
		yPos -= yPosFallDiff;
		*objectPosition = XMVectorSetY(*objectPosition, yPos);
	}
	if (isRising) {
		yPos = XMVectorGetY(*objectPosition);
		yPosFallDiff = yPosFallStart;
		previouslyRising = true;
	}

	if (XMVectorGetY(*objectPosition) - collider.Extents.y <= minY) {
		yPos = minY;
		yPosFallDiff = yPosFallStart;
		*objectPosition = XMVectorSetY(*objectPosition, minY + collider.Extents.y);
	}
}
