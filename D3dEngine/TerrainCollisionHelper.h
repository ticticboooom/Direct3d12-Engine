#pragma once
#include "stdafx.h"
class TerrainCollisionHelper
{
public:
	D3DENGINE_API TerrainCollisionHelper();
	D3DENGINE_API ~TerrainCollisionHelper();
	D3DENGINE_API bool GetNewYPos(XMFLOAT3 position, float * yOut);
	D3DENGINE_API static void SetTerrainHeights(std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::vector<float>>>>> heights) { m_terrainHeights = heights; };
	D3DENGINE_API static void SetTerrainOrigins(std::shared_ptr<std::vector<XMFLOAT4>> origins) { m_chunkOrigins = origins; };
	D3DENGINE_API static void SetTerrainPos(XMFLOAT3 pos) { m_terrainPos = pos; };
private:
	static std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::vector<float>>>>> m_terrainHeights; /// heights of the terrian in chunks
	static std::shared_ptr<std::vector<XMFLOAT4>> m_chunkOrigins; /// origins of the chunks
	static XMFLOAT3 m_terrainPos; /// position of the terrain
	UINT m_size; 
	float prevYPos; /// previsous y pos
	/**
	 * @brief interpolate a triangle based on a position within it.
	 * 
	 * @param p1 vertex 1
	 * @param p2 vertex 2
	 * @param p3 vertex 3
	 * @param pos x and z position
	 * @return float height of position
	 */
	float barryCentric(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3, XMFLOAT2 pos) {
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}
};

