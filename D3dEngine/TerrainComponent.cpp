#include "stdafx.h"
#include "TerrainComponent.h"
#include "PathManager.h"
#include "CommonObjects.h"
#include "TextureResourceManager.h"
#include "Mesh.h"
#include "ComponentManager.h"
#include "TerrainCollisionHelper.h"
#include  <functional>
bool TerrainComponent::m_isRootSignatureInitialised = false;
UINT TerrainComponent::m_textureRootSigIndex = 0;
XMVECTOR TerrainComponent::m_playerPos{};
/**
 * @brief Construct a new Terrain Component:: Terrain Component object
 * creates a terrain and all required to render it
 *
 */
TerrainComponent::TerrainComponent() :
	m_terrainVertexBufferViews(),
	m_terrainVertexBufferManagers(),
	m_terrainIndexBufferManagers(),
	m_terrainIndexBufferViews(),
	m_indexCounts(),
	m_isGeneratedVectorBeingRead(false),
	m_isGeneratedVectorBeingWrittenTo(false),
	m_isPrevThreadComplete(true)
{
	m_descriptorCount += 1;
}

/**
 * @brief Destroy the Terrain Component:: Terrain Component object
 *
 */
TerrainComponent::~TerrainComponent()
{
}

/**
 * @brief adds the texture root signature parameter
 *
 * @param indexOffset
 * @return int
 */
int TerrainComponent::InitRootSignatureParameters(int indexOffset)
{

	if (m_usingTexture && !m_isRootSignatureInitialised && !Mesh::GetIsRootSignatureInisialised())
	{
		std::shared_ptr<RootParameter> param = std::make_shared<RootParameter>();
		param->InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
		(*CommonObjects::m_rootSignatureManager)[0]->AddNewParameter(param);
		m_textureRootSigIndex = indexOffset;
		indexOffset++;
		m_isRootSignatureInitialised = true;

		Mesh::SetIsRootSignatureInisialised(true);
		Mesh::SetTextureRootSignatureIndex(m_textureRootSigIndex);
	}
	else if (m_usingTexture && Mesh::GetIsRootSignatureInisialised()) {
		m_textureRootSigIndex = Mesh::GetTextureRootSignatureIndex();
	}
	return indexOffset;
}

/**
 * @brief creates the terrain and required buffers on GPU (and texture)
 *	Creates texture and puts
 * @param commandListManager
 * @param descriptorHeapManager
 * @param descOffset
 * @param pso
 */
void TerrainComponent::Init()
{
	auto pathManager = PathManager();

	m_terrainGenerator = std::make_unique<TerrainGenerationHelper>();
	TerrainCollisionHelper::SetTerrainHeights(m_terrainGenerator->GetChunks());
	TerrainCollisionHelper::SetTerrainOrigins(m_terrainGenerator->GetChunkOrigins());
	auto terrainData = m_terrainGenerator->GenTerrain(0, 0);
	auto terrainInds = std::make_shared<std::vector<unsigned int>>(terrainData->indices->begin(), terrainData->indices->end());

	auto vManager = std::make_shared<VertexBufferManager>(terrainData->vertices, CommonObjects::m_deviceResources, CommonObjects::m_commandListManager);
	auto iManager = std::make_shared<IndexBufferManager>(terrainInds, CommonObjects::m_deviceResources, CommonObjects::m_commandListManager);
	m_terrainVertexBufferManagers.push_back(vManager);
	m_terrainIndexBufferManagers.push_back(iManager);

	m_indexCounts.push_back(terrainInds->size());

	m_terrainVertexBufferViews.push_back(vManager->CreateVertexBufferView());
	m_terrainIndexBufferViews.push_back(iManager->CreateIndexBufferView());

	m_cbvDescriptorSize = CommonObjects::m_descriptorHeapManager->GetDescriptorSize();
	if (m_usingTexture) {
		const auto textureManager = new TextureResourceManager(std::wstring(pathManager.GetAssetPath() + m_texturePath).c_str(), CommonObjects::m_deviceResources, CommonObjects::m_commandListManager);
		textureManager->CreateSRVFromTextureResource(CommonObjects::m_descriptorHeapManager->Get(), m_cbvDescriptorSize, CommonObjects::m_descriptorHeapIndexOffset);
		m_textureDescHeapIndex = CommonObjects::m_descriptorHeapIndexOffset;
		m_rootSignInds.push_back(m_textureRootSigIndex);
		m_heapInds.push_back(m_textureDescHeapIndex);
		CommonObjects::m_descriptorHeapIndexOffset++;
	}
}

void TerrainComponent::Update()
{
	

	CreateTerrainPoints();
	
	if (m_newPositions.size() > 0 && m_isPrevThreadComplete) {
		m_loadingThreads.clear();
		std::function<void()> threadFunc = [&] {CreateChunks(); };
		m_loadingThreads.push_back(std::make_unique<Thread>(threadFunc));
		m_isPrevThreadComplete = false;
	}

	for (auto& thread : m_loadingThreads) {
		if (thread->GetCompleteState()) {
			m_isPrevThreadComplete = true;
			for (auto& data : m_generated) {
				CreateChunkFromData(data);
			}
			thread->CloseIfComplete();
		}
	}
	if (m_isPrevThreadComplete) {
		m_loadingThreads.clear();
	}
}



/**
 * @brief renders texture and terrain
 *
 */
void TerrainComponent::Render()
{


	CommonObjects::m_descriptorHeapManager->Render(m_rootSignInds.size(), m_rootSignInds.data(), m_heapInds.data(), CommonObjects::m_commandListManager);

	CommonObjects::m_commandListManager->CreateResourceBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	CommonObjects::m_commandListManager->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto i = 0; i < m_terrainIndexBufferManagers.size(); i++) {
		CommonObjects::m_commandListManager->SetVertexBuffers(0, 1, &m_terrainVertexBufferViews[i]);
		CommonObjects::m_commandListManager->SetIndexBuffer(&m_terrainIndexBufferViews[i]);
		CommonObjects::m_commandListManager->DrawIndexedInstanced(m_indexCounts[i], 1, 0, 0, 0);
	}

	CommonObjects::m_commandListManager->CreateResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
}

void TerrainComponent::OnKeyDown(UINT key)
{
}

void TerrainComponent::OnKeyUp(UINT key)
{
}

void TerrainComponent::OnMouseMoved(float x, float y)
{
}

void TerrainComponent::OnDeviceRemoved()
{
}

void TerrainComponent::CreateWindowSizeDependentResources()
{
}

void TerrainComponent::CreateDeviceDependentResoures()
{
}

/**
 * @brief defines whether and what texture to use
 *
 * @param filename the filename to the texture file
 */
void TerrainComponent::UseTexture(std::wstring filename)
{
	m_usingTexture = true;
	m_texturePath = filename;
}

void TerrainComponent::CreateTerrainPoints()
{

	auto pos = XMFLOAT3{};
	XMStoreFloat3(&pos, m_playerPos);

	float gridSquareSize = TERRAIN_STEP_SIZE;
	// grid positions X and Z 
	auto gridX = static_cast<int>(floorf(pos.x / gridSquareSize));
	auto gridZ = static_cast<int>(floorf(pos.z / gridSquareSize));

	auto chunkSize = m_terrainGenerator->GetChunkSize();
	auto gridCorner = chunkSize / gridSquareSize;

	auto relGridX = fmod(gridX, chunkSize / gridSquareSize);
	auto relGridZ = fmod(gridZ, chunkSize / gridSquareSize);

	if (relGridX < 0) {
		relGridX = gridCorner - abs(relGridX);
	}

	if (relGridZ < 0) {
		relGridZ = gridCorner - abs(relGridZ);
	}

	auto cornerX = (gridX - relGridX) * gridSquareSize;
	auto cornerZ = (gridZ - relGridZ) * gridSquareSize;


	auto positions = std::vector<XMFLOAT2>();
	// all are for looking X+
	{
		//Front
		float x = cornerX + chunkSize;
		float z = cornerZ;
		positions.push_back({ x,z });
	}
	{
		//Back
		float x = cornerX - chunkSize;
		float z = cornerZ;
		positions.push_back({ x,z });
	}
	{
		//Left
		float x = cornerX;
		float z = cornerZ - chunkSize;
		positions.push_back({ x,z });
	}
	{
		//Right
		float x = cornerX;
		float z = cornerZ + chunkSize;
		positions.push_back({ x,z });
	}

	// corners
	{
		//Front - Left
		float x = cornerX + chunkSize;
		float z = cornerZ - chunkSize;
		positions.push_back({ x,z });
	}
	{
		//Back
		float x = cornerX - chunkSize;
		float z = cornerZ + chunkSize;
		positions.push_back({ x,z });
	}
	{
		//Left
		float x = cornerX - chunkSize;
		float z = cornerZ - chunkSize;
		positions.push_back({ x,z });
	}
	{
		//Right
		float x = cornerX + chunkSize;
		float z = cornerZ + chunkSize;
		positions.push_back({ x,z });
	}

	m_newPositions.clear();
	auto chunkExtents = m_terrainGenerator->GetChunkOrigins();
	for (auto& position : positions) {
		auto exists = false;
		for (auto& extent : *chunkExtents) {
			if (position.x == extent.x && position.y == extent.y) {
				exists = true;
				break;
			}
		}
		if (!exists) {
			m_newPositions.push_back(position);
		}
	}
}

void TerrainComponent::CreateChunkFromData(std::shared_ptr<Structures::VerticesIndicesFromBin> data)
{
	auto terrainInds = std::make_shared<std::vector<unsigned int>>(data->indices->begin(), data->indices->end());

	auto vManager = std::make_shared<VertexBufferManager>(data->vertices, CommonObjects::m_deviceResources, CommonObjects::m_commandListManager);
	auto iManager = std::make_shared<IndexBufferManager>(terrainInds, CommonObjects::m_deviceResources, CommonObjects::m_commandListManager);
	m_terrainVertexBufferManagers.push_back(vManager);
	m_terrainIndexBufferManagers.push_back(iManager);

	m_indexCounts.push_back(terrainInds->size());

	m_terrainVertexBufferViews.push_back(vManager->CreateVertexBufferView());
	m_terrainIndexBufferViews.push_back(iManager->CreateIndexBufferView());
}

void TerrainComponent::CreateChunks()
{
	auto positions = m_newPositions;
	for (auto pos : positions) {
		m_generated.push_back(m_terrainGenerator->GenTerrain(pos.x, pos.y));
	}
}
