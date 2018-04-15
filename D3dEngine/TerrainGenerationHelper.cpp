#include "stdafx.h"
#include "TerrainGenerationHelper.h"

/**
 * @brief Construct a new Terrain Generation Helper:: Terrain Generation Helper object
 *  this used the PerlinNoise class to generate terrian based on a random seed
 */
TerrainGenerationHelper::TerrainGenerationHelper()
{
	m_noiseGenerator = std::make_unique<PerlinNoise>(4, 0.00013, 5, 5.8, 0);
	m_chunks = std::make_shared<std::vector<std::shared_ptr<std::vector<std::vector<float>>>>>();
	m_chunkOrigins = std::make_shared<std::vector<XMFLOAT4>>();
}

/**
 * @brief Destroy the Terrain Generation Helper:: Terrain Generation Helper object
 * 
 */
TerrainGenerationHelper::~TerrainGenerationHelper()
{
}

/**
 * @brief generates a chunk with a heightmap anpd vertices
 * 
 * @param originX the start X point
 * @param originZ the start point
 * @return Structures::VerticesIndicesFromBin the vertices and indices from the terrain
 */
Structures::VerticesIndicesFromBin TerrainGenerationHelper::GenTerrain(float originX, float originZ)
{
	// fill 2D vector with 0s 
	auto heights = std::make_shared<std::vector<std::vector<float>>>(c_size / TERRAIN_STEP_SIZE + 1);
	std::fill(heights->begin(), heights->end(), std::vector<float>(c_size / TERRAIN_STEP_SIZE + 1));

	// add the heights, chunks and origins the the corresponding vector
	m_chunks->push_back(heights);
	m_chunkOrigins->push_back({ 
		originX,
		originZ,
		originX + c_size,
		originZ + c_size });
	// generate vertices
	return GenVertices(heights, originX, originZ);
}
/**
 * @brief generate the vertex data for an entire chunk, generate the heights
 * 
 * @param heights the pointer to add the heights to
 * @param originX the start X point
 * @param originZ the start point
 * @return Structures::VerticesIndicesFromBin the vertex data
 */
Structures::VerticesIndicesFromBin TerrainGenerationHelper::GenVertices(std::shared_ptr<std::vector<std::vector<float>>> heights, float originX, float originZ)
{
	// create the required vectors
	auto vertices = std::make_shared<std::vector<Structures::VertexTexCoordNormal>>();
	auto indices = std::make_shared<std::vector<unsigned long>>();
	auto index = m_index;
	auto deltaIndex = 0;
	// create the 2D plane of heights and triangles
	for (auto x = originX; x < originX + c_size; x += TERRAIN_STEP_SIZE) {
		for (auto z = originZ; z < originZ + c_size; z += TERRAIN_STEP_SIZE) {
			// get the relative X and Z
			auto deltaX = x - originX;
			auto deltaZ = z - originZ;

			//generate the heights for each corner
			auto bottomRight = m_noiseGenerator->GetHeight(x + TERRAIN_STEP_SIZE, z);
			auto topLeft = m_noiseGenerator->GetHeight(x, z + TERRAIN_STEP_SIZE);
			auto bottomLeft = m_noiseGenerator->GetHeight(x, z);
			auto topRight = m_noiseGenerator->GetHeight(x + TERRAIN_STEP_SIZE, z + TERRAIN_STEP_SIZE);

			// add the heights to the heights vector
			(*heights)[deltaX / TERRAIN_STEP_SIZE + 1][deltaZ / TERRAIN_STEP_SIZE] = bottomRight;
			(*heights)[deltaX / TERRAIN_STEP_SIZE][deltaZ / TERRAIN_STEP_SIZE + 1] = topLeft;
			(*heights)[deltaX / TERRAIN_STEP_SIZE][deltaZ / TERRAIN_STEP_SIZE] = bottomLeft;
			(*heights)[deltaX / TERRAIN_STEP_SIZE + 1][deltaZ / TERRAIN_STEP_SIZE + 1] = topRight;

			// triangle 1

			//bottom right
			vertices->push_back(GenVertex(bottomRight, x + TERRAIN_STEP_SIZE, z));
			indices->push_back(index);
			index++;
			deltaIndex++;

			//top left
			vertices->push_back(GenVertex(topLeft, x, z + TERRAIN_STEP_SIZE));
			indices->push_back(index);
			index++;
			deltaIndex++;

			// bottm left
			vertices->push_back(GenVertex(bottomLeft, x, z));
			indices->push_back(index);
			index++;
			deltaIndex++;
			
			GenNormalsPerTri(&(*vertices)[deltaIndex - 1], &(*vertices)[deltaIndex - 2], &(*vertices)[deltaIndex - 3]);

			// triangle 2

			//bottom right
			vertices->push_back(GenVertex(bottomRight, x + TERRAIN_STEP_SIZE, z));
			indices->push_back(index);
			index++;
			deltaIndex++;
			
			// top right
			vertices->push_back(GenVertex(topRight, x + TERRAIN_STEP_SIZE, z + TERRAIN_STEP_SIZE));
			indices->push_back(index);
			index++;
			deltaIndex++;

			// top left
			vertices->push_back(GenVertex(topLeft, x,z + TERRAIN_STEP_SIZE));
			indices->push_back(index);
			index++;
			deltaIndex++;

			

			GenNormalsPerTri(&(*vertices)[deltaIndex - 1], &(*vertices)[deltaIndex - 2], &(*vertices)[deltaIndex - 3]);
		}
	}
	m_index = index;
	return { vertices, indices };
}
/**
 * @brief Initialise the vertex with basic data like the position and isAnimated flag
 * 
 * @param height the y coord
 * @param x the x coord
 * @param z the z coord
 * @return Structures::VertexTexCoordNormal 
 */
Structures::VertexTexCoordNormal TerrainGenerationHelper::GenVertex(float height, float x, float z)
{
	auto v = Structures::VertexTexCoordNormal{};
	v.pos = { x, height, z };
	v.isAnimated = 0;
	return v;
}
/**
 * @brief generate the normals for a given triangle
 * 
 * @param v0 vertex 0
 * @param v1 vertex 1
 * @param v2 vertex 2
 */
void TerrainGenerationHelper::GenNormalsPerTri(Structures::VertexTexCoordNormal * v0, Structures::VertexTexCoordNormal * v1, Structures::VertexTexCoordNormal * v2)
{
	// some maths to do with creating normals
	auto vec0 = XMLoadFloat3(&v1->pos) - XMLoadFloat3(&v0->pos);
	auto vec1 = XMLoadFloat3(&v2->pos) - XMLoadFloat3(&v0->pos);
	
	auto x = (XMVectorGetY(vec0) * XMVectorGetZ(vec1)) - (XMVectorGetZ(vec0) * XMVectorGetY(vec1));
	auto y = (XMVectorGetZ(vec0) * XMVectorGetX(vec1)) - (XMVectorGetX(vec0) * XMVectorGetZ(vec1));
	auto z = (XMVectorGetX(vec0) * XMVectorGetY(vec1)) - (XMVectorGetY(vec0) * XMVectorGetX(vec1));

	auto normal = XMFLOAT3{ x,y,z };

	v0->normal = normal;
	v1->normal = normal;
	v2->normal = normal;
}
