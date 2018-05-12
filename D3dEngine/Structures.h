#pragma once
#include "stdafx.h"
#include <vector>
#include "RootSignatureManager.h"
#include <map>

using namespace DirectX;
/**
 * @brief the common structures required in the engine
 *
 */
namespace Structures
{
	struct AnimFrameBone
	{
		std::string name;
		DirectX::XMFLOAT4X4 MATRIX;
		int index;
	};
	struct AnimTransforms {
		XMVECTOR pos;
		XMVECTOR rot;
		XMVECTOR scale;
	};
	struct AnimBone
	{
		std::string name;
		std::vector<XMMATRIX> transforms;
		std::vector<XMMATRIX> finalTransforms = std::vector<XMMATRIX>();
		XMMATRIX offsetMatrix;
		XMMATRIX mtrans;
		XMMATRIX toRoot;
	};
	struct assimpVertexWeight {
		unsigned int mVertexId;
		float mWeight;
	};
	struct assimpBone {
		std::string mName;
		unsigned int mNumWeights;
		std::vector<assimpVertexWeight> mWeights;
	};
	struct Animation
	{
		std::vector<AnimBone> bonesWithKeys;
		UINT frameCount;
		std::vector<assimpBone> bones;
		int numBones;
	};
	struct Bone
	{
		UINT boneIndex;
		float weight;
	};
	struct VertexTexCoordNormalBones
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 texCoord;
		DirectX::XMFLOAT3 normal;
		std::vector<Bone> bones;
	};
	struct VertexTexCoordNormal
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 texCoord;
		DirectX::XMFLOAT3 normal;
		XMFLOAT4 weights1;
		XMFLOAT4 weights2;
		XMFLOAT4 bones1;
		XMFLOAT4 bones2;
		int isAnimated;
	};

	struct VerticesIndices
	{
		std::vector<VertexTexCoordNormalBones> vertices;
		std::vector<unsigned int> indices;
		std::vector<Structures::Animation> animations;
	};
	struct D3DENGINE_API VerticesIndicesFromBin
	{
		std::shared_ptr<std::vector<VertexTexCoordNormal>> vertices;
		std::shared_ptr<std::vector<unsigned long>> indices;
		std::shared_ptr<std::vector<std::vector<std::vector<XMMATRIX>>>> animations;
	};

	struct ShaderData
	{
		byte* shader;
		UINT size;
	};
	struct ModelViewProjectionConstantBuffer
	{
		DirectX::XMFLOAT4X4 model;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	struct HeightMapInfo {        // Heightmap structure
		int terrainWidth;        // Width of heightmap
		int terrainHeight;        // Height (Length) of heightmap
		XMFLOAT3* heightMap;    // Array to store terrain's vertex positions
	};

	struct Transform {
		XMVECTOR position;
		XMVECTOR rotationQuat;
		XMVECTOR scale;
		Transform() {
			position = XMVectorSet(0, 0, 0, 1);
			rotationQuat = XMQuaternionIdentity();
			scale = XMVectorSet(1, 1, 1, 1);
		}
	};
	struct BoundingCylinder {
		XMFLOAT3 Center;
		float radius;
		float extentY;
		BoundingCylinder(XMFLOAT3 cen, float rad, float extY) :
			Center(cen), radius(rad), extentY(extY) {}

		BoundingCylinder() {
			Center = { 0.f, 0.f, 0.f };
			radius = 0.0f;
			extentY = 0.0f;
		}

		bool Intersects(BoundingCylinder otherCylinder) {
			auto diff = XMFLOAT3{};
			XMStoreFloat3(&diff, XMLoadFloat3(&Center) - XMLoadFloat3(&otherCylinder.Center));
			auto length = XMVectorGetX(XMVector3Length(XMVectorSet(diff.x, 0, diff.z, 0)));
			auto collideY = ((diff.y - (extentY * 2) < 0 && diff.y > 0) || (diff.y + (extentY * 2) > 0 && diff.y < 0));
			if (length < radius + otherCylinder.radius && collideY)
			{
				return true;
			}
			return false;
		}
	};
}