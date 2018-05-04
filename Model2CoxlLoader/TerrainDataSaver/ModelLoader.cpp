#include "stdafx.h"
#include "ModelLoader.h"
#include "Structures.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "DirectXHelper.h"
#include <iostream>
#include <fstream>
#define MAX_THREADS 8
Structures::VerticesIndices ModelLoader::LoadModelFromFile(std::string fileName) {

	Assimp::Importer importer;

	const auto scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
		aiProcess_FlipUVs);

	if (!scene)
	{
		auto err = importer.GetErrorString();
		return {};
	}

	InitFromScene(scene);

	return { m_finalVertices, m_indices, m_finalAnimations };
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
	ReadBoneHierarchy(scene);
	FinaliseAnimations();
	CreateFinalVertices();
}

void ModelLoader::LoadAnimations(const aiScene * scene)
{
	m_maxFrameCountPerAnim = std::vector<UINT>();
	for (auto i = 0; i < scene->mNumAnimations; i++) {
		auto animation = Structures::Animation{};

		animation.bonesWithKeys = std::vector<Structures::AnimBone>();
		for (auto j = 0; j < scene->mAnimations[i]->mNumChannels; j++) {
			auto channel = scene->mAnimations[i]->mChannels[j];
			auto transforms = std::vector<XMMATRIX>();
			auto times = std::vector<float>();
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

				times.push_back(channel->mPositionKeys[key].mTime);

				auto transform = XMMatrixAffineTransformation(scaling, zero, rotation, translation);
				transforms.push_back(transform);
			}
			if (std::string(channel->mNodeName.data) == "Cube" || std::string(channel->mNodeName.data) == "Armature") {
				continue;
			}
			auto animBone = Structures::AnimBone{ channel->mNodeName.data, transforms };
			animBone.times = times;
			animation.bonesWithKeys.push_back(animBone);
			animation.frameCount = scene->mAnimations[i]->mDuration;
		}
		m_animations.push_back(animation);
		auto maxFrameCount = 0u;
		for (auto bone : animation.bonesWithKeys) {
			maxFrameCount = (bone.transforms.size()) > maxFrameCount ? bone.transforms.size() : maxFrameCount;
		}
		m_maxFrameCountPerAnim.push_back(maxFrameCount);
	}


}

void ModelLoader::ReadBoneHierarchy(const aiScene* scene) {

	auto root = scene->mRootNode;
	root = root->FindNode("Armature");
	for (auto anim = 0; anim < m_animations.size(); anim++)
	{
		m_animations[anim].bonesWithKeys = FillTimes(root, m_animations[anim].bonesWithKeys, anim);
		m_animations[anim].bonesWithKeys = FillMissingMappedTransforms(root, m_animations[anim].bonesWithKeys, anim);

		for (auto i = 0; i < root->mNumChildren; i++) {
			auto childBoneId = FindBoneWithKeys(m_animations[anim].bonesWithKeys, root->mChildren[i]->mName.data);
			auto childTransform = m_animations[anim].bonesWithKeys[childBoneId];
			for (auto i = 0; i < childTransform.transforms.size(); i++) {
				childTransform.transforms[i] = childTransform.transforms[i];
				auto offset = childTransform.offsetMatrix;
				childTransform.finalTransforms.push_back(XMMatrixTranspose(offset) * childTransform.transforms[i]);
			}
			m_animations[anim].bonesWithKeys[childBoneId] = childTransform;
			m_animations[anim].bonesWithKeys = GetNodes(root->mChildren[i], m_animations[anim].bonesWithKeys, anim);

		}
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
						(*m_vertices)[scene->mMeshes[i]->mBones[x]->mWeights[y].mVertexId].bones.push_back({
							static_cast<UINT>(z), scene->mMeshes[i]->mBones[x]->mWeights[y].mWeight });
					}
					break;
				}
			}
		}
	}


}

void ModelLoader::CreateFinalVertices()
{
	m_finalVertices = std::make_shared<std::vector<Structures::VertexTexCoordNormal>>();
	m_finalVertices->reserve(m_vertices->size());
	for (auto& vertex : *m_vertices)
	{
		auto bones = vertex.bones;
		auto newVertex = Structures::VertexTexCoordNormal{ vertex.pos, vertex.texCoord, vertex.normal };


		if (vertex.bones.size() > 0) {
			newVertex.isAnimated = 1;
		}
		else {
			newVertex.isAnimated = 0;
		}


		if (bones.size() >= 4)
		{
			newVertex.bones1.x = bones[0].boneIndex;
			newVertex.bones1.y = bones[1].boneIndex;
			newVertex.bones1.z = bones[2].boneIndex;
			newVertex.bones1.w = bones[3].boneIndex;
			newVertex.weights1.x = bones[0].weight;
			newVertex.weights1.y = bones[1].weight;
			newVertex.weights1.z = bones[2].weight;
			newVertex.weights1.w = bones[3].weight;
			if (bones.size() >= 5)
			{
				newVertex.bones2.x = bones[4].boneIndex;
				newVertex.weights2.x = bones[4].weight;
			}
			if (bones.size() >= 6)
			{
				newVertex.bones2.y = bones[5].boneIndex;
				newVertex.weights2.y = bones[5].weight;
			}
			if (bones.size() >= 7)
			{
				newVertex.bones2.z = bones[6].boneIndex;
				newVertex.weights2.z = bones[6].weight;
			}
			if (bones.size() >= 8)
			{
				newVertex.bones2.w = bones[7].boneIndex;
				newVertex.weights2.w = bones[7].weight;
			}
		}
		else
		{
			if (bones.size() >= 1)
			{
				newVertex.bones1.x = bones[0].boneIndex;
				newVertex.weights1.x = bones[0].weight;
			}
			if (bones.size() >= 2)
			{
				newVertex.bones1.y = bones[1].boneIndex;
				newVertex.weights1.y = bones[1].weight;
			}
			if (bones.size() >= 3)
			{
				newVertex.bones1.z = bones[2].boneIndex;
				newVertex.weights1.z = bones[2].weight;
			}
		}

		m_finalVertices->push_back(newVertex);
	}
	m_vertices->clear();
}

void ModelLoader::FinaliseAnimations()
{
	m_finalAnimations = std::make_shared<Structures::AnimationForExport>(
		Structures::AnimationForExport{
			std::vector<std::vector<std::vector<XMMATRIX>>>()
		});

	for (auto animation : m_animations) {
		auto anim = std::vector<std::vector<XMMATRIX>>();
		for (auto bone : animation.bonesWithKeys) {
			anim.push_back(bone.finalTransforms);
		}
		m_finalAnimations->perBoneAnims.push_back(anim);
	}
}


std::shared_ptr<std::vector<unsigned long>> ModelLoader::loadIndices(const aiScene* scene, const int i)
{
	auto indices = std::make_shared<std::vector<unsigned long>>();
	if (scene->mMeshes[i]->HasFaces())
	{
		for (int j = scene->mMeshes[i]->mNumFaces - 1; j > -1; j--)
		{
			for (int k = scene->mMeshes[i]->mFaces[j].mNumIndices - 1; k > -1; k--)
			{
				indices->push_back(scene->mMeshes[i]->mFaces[j].mIndices[k]);
			}
		}
	}
	return indices;
}

DWORD WINAPI vertexThread(__in LPVOID lpParameter)
{

	auto param = ((Structures::VertexThreadParemeter*)lpParameter);
	param->vertices.reserve(200000u);
	for (auto j = param->vertexStart; j < param->vertexEnd; j++)
	{
		param->vertices.push_back({
			{
				param->mesh->mVertices[j].x,
				param->mesh->mVertices[j].y,
				param->mesh->mVertices[j].z,
			},
			{
				param->mesh->mTextureCoords[0][j].x,
				param->mesh->mTextureCoords[0][j].y
			},
			{
				param->mesh->mNormals[j].x,
				param->mesh->mNormals[j].y,
				param->mesh->mNormals[j].z,
			},
			});
	}
	return 0;
}

std::shared_ptr<std::vector<Structures::VertexTexCoordNormalBones>> ModelLoader::loadVertices(const aiScene* scene, const int i)
{
	auto threads = std::vector<HANDLE>();
	auto threadIds = std::vector<DWORD*>();
	auto params = std::vector<Structures::VertexThreadParemeter>();
	const auto size = 200000u;
	auto vertexCounter = 0u;
	auto counter = 0u;
	while (vertexCounter < scene->mMeshes[i]->mNumVertices) {
		if (vertexCounter + size > scene->mMeshes[i]->mNumVertices) {
			vertexCounter = scene->mMeshes[i]->mNumVertices;
		}
		else {
			vertexCounter += size;
		}
		auto param = Structures::VertexThreadParemeter{
			scene->mMeshes[i],
			counter * size,
			vertexCounter,
			std::vector<Structures::VertexTexCoordNormalBones>()
		};
		params.push_back(param);
		counter++;
	}
	auto currentThreadCount = 0;
	for (auto j = 0; j < counter; j++) {
		while (currentThreadCount >= MAX_THREADS) {
			for (auto k = 0; k < threads.size(); k++) {
				DWORD result = WaitForSingleObject(threads[k], 0);
				if (result == WAIT_OBJECT_0) {
					CloseHandle(threads[k]);
					currentThreadCount--;
				}
			}
		}
		auto threadId = DWORD{};
		auto threadHandle = CreateThread(0, 0, vertexThread, &params[j], 0, &threadId);
		threads.push_back(threadHandle);
		threadIds.push_back(&threadId);
		currentThreadCount++;
	}
	while (currentThreadCount > 0) {
		for (auto k = 0; k < threads.size(); k++) {
			DWORD result = WaitForSingleObject(threads[k], 0);
			if (result == WAIT_OBJECT_0) {
				CloseHandle(threads[k]);
				currentThreadCount--;
			}
		}
	}
	auto vertices = std::make_shared<std::vector<Structures::VertexTexCoordNormalBones>>();
	vertices->reserve(scene->mMeshes[i]->mNumVertices);
	for (auto param : params) {
		vertices->insert(vertices->end(), param.vertices.begin(), param.vertices.end());
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

std::vector<Structures::AnimBone>  ModelLoader::GetNodes(aiNode * node, std::vector<Structures::AnimBone> list, UINT animI) {
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
			list = GetNodes(node->mChildren[i], list, animI);
		}
	}
	return list;
}

std::vector<Structures::AnimBone> ModelLoader::FillTimes(aiNode * node, std::vector<Structures::AnimBone> list, UINT animI)
{
	for (auto& bone : list) {
		bone.mappedTransforms = std::vector<XMMATRIX>(m_maxFrameCountPerAnim[animI]);
		std::fill(bone.mappedTransforms.begin(), bone.mappedTransforms.end(), XMMatrixIdentity());
		for (auto i = 0; i < m_maxFrameCountPerAnim[animI]; i++) {
			for (auto j = 0; j < bone.times.size(); j++) {
				if (bone.times[j] == m_maxFrameCountPerAnim[animI] / m_animations[animI].frameCount * i) {
					bone.mappedTransforms[i] = bone.transforms[j];
					break;
				}
			}
		}
	}
	return list;
}

std::vector<Structures::AnimBone> ModelLoader::FillMissingMappedTransforms(aiNode * node, std::vector<Structures::AnimBone> list, UINT animI)
{
	for (auto& bone : list) {
		auto prevKey = 0u;
		for (auto i = 1; i < bone.mappedTransforms.size(); i++) {
			if (XMMatrixIsIdentity(bone.mappedTransforms[i])) {
				auto prev = bone.mappedTransforms[i - 1];
				auto nextIndex = GetNextNoneIdentity(bone.mappedTransforms, i);

				float f =  i / (nextIndex - prevKey);
				auto matrix = InterpMatrix(prev, bone.mappedTransforms[nextIndex], f);
				bone.mappedTransforms[i] = matrix;
			}
			else {
				prevKey = i;
			}
		}
		bone.transforms = bone.mappedTransforms;
	}
	return list;
}

UINT ModelLoader::GetNextNoneIdentity(std::vector<XMMATRIX> list, UINT current)
{
	for (auto i = current + 1; i < list.size(); i++) {
		if (XMMatrixIsIdentity(list[i])) {
			continue;
		}
		else {
			return i;
		}
	}
}

XMMATRIX ModelLoader::InterpMatrix(XMMATRIX m0, XMMATRIX m1, float f)
{
	XMVECTOR pos0;
	XMVECTOR rot0;
	XMVECTOR scale0;
	XMMatrixDecompose(&scale0, &rot0, &pos0, m0);
	
	XMVECTOR pos1;
	XMVECTOR rot1;
	XMVECTOR scale1;
	XMMatrixDecompose(&scale1, &rot1, &pos1, m1);

	auto newPos = XMVectorLerp(pos0, pos1, f);
	auto newRot = XMQuaternionSlerp(rot0, rot1, f);
	auto newScale = XMVectorLerp(scale0, scale1, f);

	auto outMat = XMMatrixAffineTransformation(newScale, {0,0,0,0}, newRot, newPos);
	return outMat;
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
