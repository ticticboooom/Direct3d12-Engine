#pragma once
#include "Structures.h"
#include <assimp/scene.h>
#include "AnimationManager.h"
#include <assimp/matrix4x4.h>
class ModelLoader
{
public:

	D3DENGINE_API Structures::VerticesIndices LoadModelFromFile(std::string fileName);

private:
	XMMATRIX getParentTrans(XMMATRIX, aiNode* node);
	void InitFromScene(const aiScene* scene);
	void LoadAnimations(const aiScene* scene);
	void LoadBones(const aiScene * scene, const int i);
	std::vector<unsigned> loadIndices(const aiScene * scene, const int i);
	std::vector<Structures::VertexTexCoordNormalBones> loadVertices(const aiScene * scene, const int i);
	int FindBoneWithKeys(std::vector<Structures::AnimBone> bones, std::string name);

	std::vector<Structures::AnimBone> GetNodes(aiNode * node, std::vector<Structures::AnimBone> list);
	std::vector<Structures::VertexTexCoordNormalBones> m_vertices;
	std::vector<unsigned> m_indices;
	std::vector<Structures::Animation> m_animations;
	XMMATRIX aiToXmmatrix(aiMatrix4x4 mat);
};