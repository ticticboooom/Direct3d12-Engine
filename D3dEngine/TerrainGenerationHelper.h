#pragma once
#include "stdafx.h"
#include "PerlinNoise.h"
#include "Structures.h"
#define TERRAIN_STEP_SIZE 5
class TerrainGenerationHelper
{
public:
	D3DENGINE_API TerrainGenerationHelper();
	D3DENGINE_API ~TerrainGenerationHelper();
	D3DENGINE_API Structures::VerticesIndicesFromBin GenTerrain(float originX, float originY);
	D3DENGINE_API std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::vector<float>>>>> GetChunks() const { return m_chunks; };
	D3DENGINE_API std::shared_ptr<std::vector<XMFLOAT4>> GetChunkOrigins() const { return m_chunkOrigins; };
private:
	std::unique_ptr<PerlinNoise> m_noiseGenerator;
	Structures::VerticesIndicesFromBin GenVertices(std::shared_ptr<std::vector<std::vector<float>>> heights, float originX, float originZ);
	Structures::VertexTexCoordNormal GenVertex(float height, float x, float z);
	void GenNormalsPerTri(Structures::VertexTexCoordNormal* v0, Structures::VertexTexCoordNormal* v1, Structures::VertexTexCoordNormal* v2);
	std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::vector<float>>>>> m_chunks; /// chunks of the terrain
	std::shared_ptr<std::vector<XMFLOAT4>> m_chunkOrigins; /// chunk origins
	const int c_size = 250; // constant size of chunk
	ULONG m_index; /// vertex index 
};

