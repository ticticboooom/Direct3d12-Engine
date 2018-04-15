#include "stdafx.h"
#include "BinaryConverter.h"


BinaryConverter::BinaryConverter(Structures::VerticesIndices data, std::string outputPath)
{
	auto verticesSize = data.vertices->size();
	auto indicesSize = data.indices->size();
	auto animsCount = data.animations->perBoneAnims.size();
	auto boneCounts = std::vector<size_t>();
	auto frameCounts = std::vector<std::vector<size_t>>();
	for (auto bones : data.animations->perBoneAnims) {
		boneCounts.push_back(bones.size());
		auto perBoneFrameCounts = std::vector<size_t>();
		for (auto frames : bones) {
			perBoneFrameCounts.push_back(frames.size());
		}
		frameCounts.push_back(perBoneFrameCounts);
	}
	auto boneCountsCount = boneCounts.size();
	auto frameCountsCount = frameCounts.size();

	m_file = std::ofstream(outputPath, std::ios::binary | std::ios::out);
	m_file.write((const char*)&verticesSize, sizeof(size_t));
	m_file.write((const char*)&indicesSize, sizeof(size_t));
	m_file.write((const char*)&animsCount, sizeof(size_t));
	m_file.write((const char*)&boneCountsCount, sizeof(size_t));
	m_file.write((const char*)&frameCountsCount, sizeof(size_t));


	m_file.write(reinterpret_cast<char*>(boneCounts.data()), sizeof(size_t) * boneCounts.size());
	for (auto bone : frameCounts) {
		m_file.write(reinterpret_cast<char*>(bone.data()), sizeof(size_t) * bone.size());
	}
	WriteVertices(data.vertices);
	WriteIndices(data.indices);
	WriteAnimations(data.animations, 0);
	m_file.close();
}


BinaryConverter::~BinaryConverter()
{
}

void BinaryConverter::WriteVertices(std::shared_ptr<std::vector<Structures::VertexTexCoordNormal>> data)
{
	m_file.write(reinterpret_cast<char*>(data->data()), sizeof(Structures::VertexTexCoordNormal) * data->size());
}

void BinaryConverter::WriteIndices(std::shared_ptr<std::vector<unsigned long>> data)
{
	m_file.write(reinterpret_cast<char*>(data->data()), sizeof(unsigned long) * data->size());
}

void BinaryConverter::WriteAnimations(std::shared_ptr<Structures::AnimationForExport> data, UINT frameCount)
{
	for (auto bones : data->perBoneAnims) {
		for (auto frames : bones) {
			auto newFrames = std::vector<XMFLOAT4X4>();
			for (auto frame : frames) {
				auto float4x4 = XMFLOAT4X4{};
				XMStoreFloat4x4(&float4x4, frame);
				newFrames.push_back(float4x4);
			}
			m_file.write(reinterpret_cast<char*>(newFrames.data()), newFrames.size() * sizeof(XMFLOAT4X4));
		}
	}
}

