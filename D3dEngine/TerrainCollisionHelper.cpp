#include "stdafx.h"
#include "TerrainCollisionHelper.h"
#include "Structures.h"
#include "TerrainGenerationHelper.h"
std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::vector<float>>>>> TerrainCollisionHelper::m_terrainHeights = std::make_shared<std::vector<std::shared_ptr<std::vector<std::vector<float>>>>>();
std::shared_ptr<std::vector<XMFLOAT4>> TerrainCollisionHelper::m_chunkOrigins = std::make_shared<std::vector<XMFLOAT4>>();
XMFLOAT3 TerrainCollisionHelper::m_terrainPos{};
/**
 * @brief Construct a new Terrain Collision Helper:: Terrain Collision Helper object
 *  controls the collision of any object with terrain
 */
TerrainCollisionHelper::TerrainCollisionHelper() :
	prevYPos(0)
{
	m_size = m_terrainHeights->size();
}

/**
 * @brief Destroy the Terrain Collision Helper:: Terrain Collision Helper object
 * 
 */
TerrainCollisionHelper::~TerrainCollisionHelper()
{

}

/**
 * @brief gets the new minimum y position that a vector / obejct can go to at the given location on the terrain
 * 
 * @param position the current poisiton of the object
 * @param yOut the output parameter that is the new minimum y position
 * @return true it has fully run correctly
 * @return false it has missing data / parameters
 */
bool TerrainCollisionHelper::GetNewYPos(XMFLOAT3 position, float* yOut)
{
	// Players location
	auto terrainXZ = XMFLOAT2{
		position.x,
		position.z
	};

	// the terrain plane size 
	float gridSquareSize = TERRAIN_STEP_SIZE;
	// grid positions X and Z 
	int gridX = static_cast<int>(floorf(terrainXZ.x / gridSquareSize));
	int gridZ = static_cast<int>(floorf(terrainXZ.y / gridSquareSize));

	// chunks that will be used
	auto chunkIndex = -1;
	auto nextChunkIndex = -1;
	auto prevChunkIndex = -1;

	// get the current chunk based on gridX, GridZ and the chunk origins global
	for (auto i = 0; i < m_chunkOrigins->size(); i++) {
		if (gridX >= (*m_chunkOrigins)[i].x / gridSquareSize && gridX <= (*m_chunkOrigins)[i].z / gridSquareSize &&
			gridZ >= (*m_chunkOrigins)[i].y / gridSquareSize && gridZ <= (*m_chunkOrigins)[i].w / gridSquareSize) {
			chunkIndex = i;
		}
		if (gridX + 1 >= (*m_chunkOrigins)[i].x / gridSquareSize && gridX + 1 <= (*m_chunkOrigins)[i].z / gridSquareSize &&
			gridZ + 1 >= (*m_chunkOrigins)[i].y / gridSquareSize && gridZ + 1 <= (*m_chunkOrigins)[i].w / gridSquareSize) {
			nextChunkIndex = i;
		}
		if (chunkIndex != -1 && nextChunkIndex != -1) {
			break;
		}
	}

	if (chunkIndex == -1 || nextChunkIndex == -1) {
		*yOut = prevYPos;
		return false;
	}

	// relative coordinates inside the current tile
	float xCoord = (fmodf(terrainXZ.x, gridSquareSize)) / gridSquareSize;
	float zCoord = (fmodf(terrainXZ.y, gridSquareSize)) / gridSquareSize;


	float answer = 0;
	// in terpolate the triangles to get the new min Y position
	if (xCoord <= (1 - zCoord)) {
		if (gridX + 1 > gridSquareSize || gridZ + 1 > gridSquareSize) {
			answer = barryCentric(
				XMFLOAT3{ 0, (*(*m_terrainHeights)[chunkIndex])[gridX - (*m_chunkOrigins)[chunkIndex].x / gridSquareSize][gridZ - (*m_chunkOrigins)[chunkIndex].y / gridSquareSize], 0 },
				XMFLOAT3{ 1, (*(*m_terrainHeights)[nextChunkIndex])[gridX + 1 - (*m_chunkOrigins)[nextChunkIndex].x / gridSquareSize][gridZ - (*m_chunkOrigins)[chunkIndex].y / gridSquareSize], 0 },
				XMFLOAT3{ 0, (*(*m_terrainHeights)[nextChunkIndex])[gridX - (*m_chunkOrigins)[chunkIndex].x / gridSquareSize][gridZ + 1 - (*m_chunkOrigins)[nextChunkIndex].y / gridSquareSize], 1 },
				XMFLOAT2{ xCoord, zCoord }
			);
		}
		else {
			answer = barryCentric(
				XMFLOAT3{ 0, (*(*m_terrainHeights)[chunkIndex])[gridX - (*m_chunkOrigins)[chunkIndex].x / gridSquareSize][gridZ - (*m_chunkOrigins)[chunkIndex].y / gridSquareSize], 0 },
				XMFLOAT3{ 1, (*(*m_terrainHeights)[chunkIndex])[gridX + 1 - (*m_chunkOrigins)[chunkIndex].x / gridSquareSize][gridZ - (*m_chunkOrigins)[chunkIndex].y / gridSquareSize], 0 },
				XMFLOAT3{ 0, (*(*m_terrainHeights)[chunkIndex])[gridX - (*m_chunkOrigins)[chunkIndex].x / gridSquareSize][gridZ + 1 - (*m_chunkOrigins)[chunkIndex].y / gridSquareSize], 1 },
				XMFLOAT2{ xCoord, zCoord }
			);
		}
	}
	else {
		if (gridX + 1 > gridSquareSize || gridZ + 1 > gridSquareSize) {
			answer = barryCentric(
				XMFLOAT3{ 1,(*(*m_terrainHeights)[nextChunkIndex])[gridX + 1 - (*m_chunkOrigins)[nextChunkIndex].x / gridSquareSize][gridZ - (*m_chunkOrigins)[nextChunkIndex].y / gridSquareSize], 0 },
				XMFLOAT3{ 1,(*(*m_terrainHeights)[nextChunkIndex])[gridX + 1 - (*m_chunkOrigins)[nextChunkIndex].x / gridSquareSize][gridZ + 1 - (*m_chunkOrigins)[chunkIndex].y / gridSquareSize], 1 },
				XMFLOAT3{ 0,(*(*m_terrainHeights)[chunkIndex])[gridX - (*m_chunkOrigins)[chunkIndex].x / gridSquareSize][gridZ + 1 - (*m_chunkOrigins)[chunkIndex].y / gridSquareSize], 1 },
				XMFLOAT2{ xCoord, zCoord }
			);
		}
		else {
			answer = barryCentric(
				XMFLOAT3{ 1,(*(*m_terrainHeights)[chunkIndex])[gridX + 1 - (*m_chunkOrigins)[chunkIndex].x / gridSquareSize][gridZ - (*m_chunkOrigins)[chunkIndex].y / gridSquareSize], 0 },
				XMFLOAT3{ 1,(*(*m_terrainHeights)[chunkIndex])[gridX + 1 - (*m_chunkOrigins)[chunkIndex].x / gridSquareSize][gridZ + 1 - (*m_chunkOrigins)[chunkIndex].y / gridSquareSize], 1 },
				XMFLOAT3{ 0,(*(*m_terrainHeights)[chunkIndex])[gridX - (*m_chunkOrigins)[chunkIndex].x / gridSquareSize][gridZ + 1 - (*m_chunkOrigins)[chunkIndex].y / gridSquareSize], 1 },
				XMFLOAT2{ xCoord, zCoord }
			);
		}
	}
	*yOut = answer;
	prevYPos = *yOut;
	return true;
}