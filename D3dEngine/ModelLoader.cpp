#include "stdafx.h"
#include "ModelLoader.h"
#include "Structures.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "DirectXHelper.h"
#include "AnimationManager.h"
#include <iostream>

Structures::VerticesIndices ModelLoader::LoadModelFromFile(std::string fileName) {

	Assimp::Importer importer;

	const auto scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
		aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

	if (!scene)
	{
		return {};
	}
	auto vertices = std::vector<Structures::VertexTexCoordNormalBones>();
	auto indices = std::vector<unsigned int>();
	auto vertexCount = 0;
	auto indexCount = 0;

	InitFromScene(scene);
	return { m_vertices, m_indices, m_animations };
}

XMMATRIX ModelLoader::getParentTrans(XMMATRIX mat, aiNode * node)
{
	if (node->mParent) {
		auto matrix = node->mParent->mTransformation;
		auto trans = XMFLOAT4X4(
			matrix.a1, matrix.a2, matrix.a3, matrix.a4,
			matrix.b1, matrix.b2, matrix.b3, matrix.b4,
			matrix.c1, matrix.c2, matrix.c3, matrix.c4,
			matrix.d1, matrix.d2, matrix.d3, matrix.d4
		);

		return XMLoadFloat4x4(&trans);
	}
	return XMMatrixIdentity();
}

void ModelLoader::InitFromScene(const aiScene * scene)
{
	LoadAnimations(scene);
	for (auto i = 0; i < scene->mNumMeshes; i++) {
		m_vertices = loadVertices(scene, i);
		m_indices = loadIndices(scene, i);
		LoadBones(scene, i);
	}
}

void ModelLoader::LoadAnimations(const aiScene * scene)
{
	for (auto i = 0; i < scene->mNumAnimations; i++) {
		auto animation = Structures::Animation{};

		animation.bonesWithKeys = std::vector<Structures::AnimBone>();
		for (auto j = 0; j < scene->mAnimations[i]->mNumChannels; j++) {
			auto channel = scene->mAnimations[i]->mChannels[j];
			auto transforms = std::vector<XMMATRIX>();
			for (auto key = 0; key < channel->mNumPositionKeys; key++) {
				auto translation = XMVectorSet(
					channel->mPositionKeys[key].mValue.x,
					channel->mPositionKeys[key].mValue.y,
					channel->mPositionKeys[key].mValue.z, 0);
				auto rotation = XMVectorSet(
					channel->mRotationKeys[key].mValue.x,
					channel->mRotationKeys[key].mValue.y,
					channel->mRotationKeys[key].mValue.z,
					channel->mRotationKeys[key].mValue.w);
				auto scaling = XMVectorSet(
					channel->mScalingKeys[key].mValue.x,
					channel->mScalingKeys[key].mValue.y,
					channel->mScalingKeys[key].mValue.z, 1);
				auto zero = XMVectorSet(0, 0, 0, 1);

				auto transform = XMMatrixAffineTransformation(scaling, zero, rotation, translation);
				transforms.push_back(transform);
			}
			if (std::string(channel->mNodeName.data) == "Cube") {
				continue;
			}
			auto animBone = Structures::AnimBone{ channel->mNodeName.data, transforms };
			animation.bonesWithKeys.push_back(animBone);
			animation.frameCount = scene->mAnimations[i]->mDuration * 24;
		}
		m_animations.push_back(animation);
	}
}

void ModelLoader::LoadBones(const aiScene * scene, const int i)
{
	for (auto x = 0; x < scene->mMeshes[i]->mNumBones; x++)
	{
		for (auto &animation : m_animations)
		{
			for (auto z = 0; z < m_animations[0].bonesWithKeys.size(); z++)
			{
				if (animation.bonesWithKeys[z].name == std::string(scene->mMeshes[i]->mBones[x]->mName.data))
				{
					const auto mat = scene->mMeshes[i]->mBones[x]->mOffsetMatrix;
					animation.bonesWithKeys[z].offsetMatrix = aiToXmmatrix(mat);
					for (auto y = 0; y < scene->mMeshes[i]->mBones[x]->mNumWeights; y++)
					{
						m_vertices[scene->mMeshes[i]->mBones[x]->mWeights[y].mVertexId].bones.push_back({
							static_cast<UINT>(z), scene->mMeshes[i]->mBones[x]->mWeights[y].mWeight });
					}
					break;
				}
			}
		}
	}

	auto root = scene->mRootNode;
	root = root->FindNode("Armature");
	for (auto anim = 0; anim < m_animations.size(); anim++)
	{
		for (auto i = 0; i < root->mNumChildren; i++) {
			auto childBoneId = FindBoneWithKeys(m_animations[anim].bonesWithKeys, root->mChildren[i]->mName.data);
			auto childTransform = m_animations[anim].bonesWithKeys[childBoneId];
			for (auto i = 0; i < childTransform.transforms.size(); i++) {
				childTransform.transforms[i] = childTransform.transforms[i];
				auto offset = childTransform.offsetMatrix;
				childTransform.finalTransforms.push_back(XMMatrixTranspose(offset) * childTransform.transforms[i]);
			}
			m_animations[anim].bonesWithKeys[childBoneId] = childTransform;
			m_animations[anim].bonesWithKeys = GetNodes(root->mChildren[i], m_animations[anim].bonesWithKeys);

		}
	}
}


std::vector<unsigned> ModelLoader::loadIndices(const aiScene* scene, const int i)
{
	std::vector<unsigned> indices = std::vector<unsigned>();
	if (scene->mMeshes[i]->HasFaces())
	{
		for (auto j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
		{
			for (auto k = 0; k < scene->mMeshes[i]->mFaces[j].mNumIndices; k++)
			{
				indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[k]);
			}
		}
	}
	std::reverse(indices.begin(), indices.end());
	return indices;
}

std::vector<Structures::VertexTexCoordNormalBones> ModelLoader::loadVertices(const aiScene* scene, const int i)
{
	auto vertices = std::vector<Structures::VertexTexCoordNormalBones>();
	for (auto j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
	{

		vertices.push_back({
			{
				scene->mMeshes[i]->mVertices[j].x,
				scene->mMeshes[i]->mVertices[j].y,
				scene->mMeshes[i]->mVertices[j].z,
			},
				{
					scene->mMeshes[i]->mTextureCoords[0][j].x,
					scene->mMeshes[i]->mTextureCoords[0][j].y
				},
				{
					scene->mMeshes[i]->mNormals[j].x,
					scene->mMeshes[i]->mNormals[j].y,
					scene->mMeshes[i]->mNormals[j].z,
				},
			});
	}
	return vertices;
}

int ModelLoader::FindBoneWithKeys(std::vector<Structures::AnimBone> bones, std::string name)
{
	for (auto bone = 0; bone < bones.size(); bone++)
	{
		if (bones[bone].name == name)
		{
			return bone;
		}
	}
	return -1;
}

std::vector<Structures::AnimBone>  ModelLoader::GetNodes(aiNode * node, std::vector<Structures::AnimBone> list) {
	if (node->mNumChildren > 0) {
		for (auto i = 0; i < node->mNumChildren; i++) {
			auto childBoneId = FindBoneWithKeys(list, node->mChildren[i]->mName.data);
			auto childTransform = list[childBoneId];
			auto parentTransform = list[FindBoneWithKeys(list, node->mName.data)];
			auto trans = aiToXmmatrix(node->mChildren[i]->mTransformation);
			for (auto i = 0; i < childTransform.transforms.size(); i++) {
				childTransform.transforms[i] = (childTransform.transforms[i] * parentTransform.transforms[i]);
				auto offset = childTransform.offsetMatrix;
				childTransform.finalTransforms.push_back(XMMatrixTranspose(offset) * ((childTransform.transforms[i])));
			}
			list[childBoneId] = childTransform;
			list = GetNodes(node->mChildren[i], list);
		}
	}
	return list;
}

XMMATRIX ModelLoader::aiToXmmatrix(aiMatrix4x4 mat)
{
	auto matrix = mat;
	auto trans = XMFLOAT4X4(
		matrix.a1, matrix.a2, matrix.a3, matrix.a4,
		matrix.b1, matrix.b2, matrix.b3, matrix.b4,
		matrix.c1, matrix.c2, matrix.c3, matrix.c4,
		matrix.d1, matrix.d2, matrix.d3, matrix.d4
	);
	return XMLoadFloat4x4(&trans);
}
