#pragma once
#include "Structures.h"
class Rotator
{
public:
	Rotator();
	~Rotator();
	Structures::VerticesIndices RotateToYUp(Structures::VerticesIndices data);
};

