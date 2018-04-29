#pragma once
#include "Structures.h"
#include <assimp/scene.h>

#include <assimp/matrix4x4.h>
class ModelLoader
{
public:
	Structures::VerticesIndices LoadModelFromFile(std::string fileName);
private:
	XMMATRIX getParentTrans(XMMATRIX, aiNode* node);
	void InitFromScene(const aiScene* scene);
	void InitFromSceneSmall(const aiScene* scene);
	void LoadAnimations(const aiScene* scene);
	void ReadBoneHierarchy(const aiScene* scene);
	void LoadBones(const aiScene * scene, const int i);
	void CreateFinalVertices();
	void FinaliseAnimations();
	std::shared_ptr<std::vector<unsigned long>> loadIndices(const aiScene * scene, const int i);
	std::shared_ptr<std::vector<Structures::VertexTexCoordNormalBones>> loadVertices(const aiScene * scene, const int i);
	std::shared_ptr<std::vector<Structures::VertexTexCoordNormalBones>> loadVerticesSmall(const aiScene * scene, const int i);
	int FindBoneWithKeys(std::vector<Structures::AnimBone> bones, std::string name);
	std::vector<Structures::AnimBone> GetNodes(aiNode * node, std::vector<Structures::AnimBone> list, UINT animI);
	std::vector<Structures::AnimBone> FillTimes(aiNode * node, std::vector<Structures::AnimBone> list, UINT animI);
	std::vector<Structures::AnimBone> FillMissingMappedTransforms(aiNode * node, std::vector<Structures::AnimBone> list, UINT animI);
	UINT GetNextNoneIdentity(std::vector<XMMATRIX> list, UINT current);
	XMMATRIX InterpMatrix(XMMATRIX m0, XMMATRIX m1, float f);
	std::shared_ptr<std::vector<Structures::VertexTexCoordNormalBones>> m_vertices;
	std::shared_ptr<std::vector<Structures::VertexTexCoordNormal>> m_finalVertices;
	std::shared_ptr<std::vector<unsigned long>> m_indices;
	std::vector<Structures::Animation> m_animations;
	std::shared_ptr<Structures::AnimationForExport> m_finalAnimations;
	XMMATRIX aiToXmmatrix(aiMatrix4x4 mat);
	std::vector<UINT> m_maxFrameCountPerAnim;
};