#pragma once
#include "stdafx.h"
#include <vector>
#include <map>
#include <assimp/mesh.h>
using namespace DirectX;
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
		std::vector<float> times;
		std::vector<XMMATRIX> mappedTransforms;
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

	struct AnimationForExport {
		std::vector<std::vector<std::vector<XMMATRIX>>> perBoneAnims;
	};
	struct VerticesIndices
	{
		std::shared_ptr<std::vector<VertexTexCoordNormal>> vertices;
		std::shared_ptr<std::vector<unsigned long>> indices;
		std::shared_ptr<Structures::AnimationForExport> animations;
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
	struct VertexThreadParemeter {
		aiMesh* mesh;
		UINT vertexStart;
		UINT vertexEnd;
		std::vector<Structures::VertexTexCoordNormalBones> vertices;
	};
}
