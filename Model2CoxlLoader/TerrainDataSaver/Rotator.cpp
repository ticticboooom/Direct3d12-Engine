#include "stdafx.h"
#include "Rotator.h"


Rotator::Rotator()
{
}


Rotator::~Rotator()
{
}

Structures::VerticesIndices Rotator::RotateToYUp(Structures::VerticesIndices data)
{
	auto animRotator = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), -XM_PI / 2));
	for (auto& anim : data.animations->perBoneAnims) {
		for (auto& bone : anim) {
			for (auto& frame : bone) {
				frame = frame * animRotator;
			}
		}
	}

	return data;
}
