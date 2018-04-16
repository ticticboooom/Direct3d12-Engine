#include "stdafx.h"
#include "PhysicsHelper.h"


/**
 * @brief Construct a new Physics Helper:: Physics Helper object
 * controls the falling of objects
 */
PhysicsHelper::PhysicsHelper()
{
	yPos = 0;
	yPosFallDiff = yPosFallStart;
}

/**
 * @brief Destroy the Physics Helper:: Physics Helper object
 * 
 */
PhysicsHelper::~PhysicsHelper()
{
}

/**
 * @brief Update the position of a vector.
 * 
 * @param minY the minimum y location that a vector can be at
 * @param objectPosition the vector that the physics is being applied to
 * @param isRising is the vector going up
 * @param collider the bounding box collider that will control the offset of the vector
 */
void PhysicsHelper::Update(float minY, XMVECTOR* objectPosition, bool isRising, BoundingBox collider)
{
	// if above minY 
	if (XMVectorGetY(*objectPosition) - collider.Extents.y >= minY && !isRising) {
		if (previouslyRising) {
			previouslyRising = false;
			yPos = XMVectorGetY(*objectPosition);
		}
		// fall down
		yPosFallDiff *= yPosFallMultiplyer;
		yPos -= yPosFallDiff;
		*objectPosition = XMVectorSetY(*objectPosition, yPos);
	}
	// is going up
	if (isRising) {
		yPos = XMVectorGetY(*objectPosition);
		yPosFallDiff = yPosFallStart;
		previouslyRising = true;
	}

	// is under minY
	if (XMVectorGetY(*objectPosition) - collider.Extents.y <= minY) {
		yPos = minY;
		//put on top of minY
		yPosFallDiff = yPosFallStart;
		*objectPosition = XMVectorSetY(*objectPosition, minY + collider.Extents.y);
	}
}
