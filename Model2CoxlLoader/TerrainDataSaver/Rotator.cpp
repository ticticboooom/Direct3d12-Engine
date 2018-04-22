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
	auto rotator = (XMMatrixAffineTransformation(XMVectorSet(1, 1, 1, 0), { 0,0,0,1 }, XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), -1.59), XMVectorSet(0, 0, 0, 0)));
	auto verts = std::make_shared<std::vector<Structures::VertexTexCoordNormal>>();
	for (auto& vert : *data.vertices) {
		XMStoreFloat3(&vert.pos, XMVector3TransformCoord(XMLoadFloat3(&vert.pos), rotator));
		XMStoreFloat3(&vert.normal, XMVector3Normalize(XMVector3TransformCoord(XMLoadFloat3(&vert.normal), rotator)));
		verts->push_back(vert);
	}
	data.vertices = verts;
	auto animRotator = XMMatrixRotationQuaternion(XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), 1.59));
	for (auto& anim : data.animations->perBoneAnims) {
		for (auto& bone : anim) {
			for (auto& frame : bone) {
				frame = XMMatrixMultiply(animRotator, frame);
			}
		}
	}

	return data;
}
