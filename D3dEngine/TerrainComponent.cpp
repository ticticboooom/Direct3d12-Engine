#include "stdafx.h"
#include "TerrainComponent.h"
#include "PathManager.h"
#include "CommonObjects.h"
#include "TextureResourceManager.h"
#include "Mesh.h"
#include "ComponentManager.h"
#include "TerrainCollisionHelper.h"

bool TerrainComponent::m_isRootSignatureInitialised = false;
UINT TerrainComponent::m_textureRootSigIndex = 0;
/**
 * @brief Construct a new Terrain Component:: Terrain Component object
 * creates a terrain and all required to render it
 * 
 */
TerrainComponent::TerrainComponent()
{

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
void TerrainComponent::Init(std::shared_ptr<CommandListManager>* commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT * descOffset, std::shared_ptr<PSOManager>* pso)
{
	m_commandListManager = *commandListManager;
	auto pathManager = PathManager();

	m_terrainGenerator = std::make_unique<TerrainGenerationHelper>();
	TerrainCollisionHelper::SetTerrainHeights(m_terrainGenerator->GetChunks());
	TerrainCollisionHelper::SetTerrainOrigins(m_terrainGenerator->GetChunkOrigins());
	auto terrainData = m_terrainGenerator->GenTerrain(0, 0);
	auto terrainData1 = m_terrainGenerator->GenTerrain(250, 0);
	auto terrainData2 = m_terrainGenerator->GenTerrain(0, 250);
	auto terrainData3 = m_terrainGenerator->GenTerrain(250, 250);

	auto terrainInds = std::make_shared<std::vector<unsigned int>>(terrainData->indices->begin(), terrainData->indices->end());
	auto terrainInds1 = std::make_shared<std::vector<unsigned int>>(terrainData1->indices->begin(), terrainData1->indices->end());
	auto terrainInds2 = std::make_shared<std::vector<unsigned int>>(terrainData2->indices->begin(), terrainData2->indices->end());
	auto terrainInds3 = std::make_shared<std::vector<unsigned int>>(terrainData3->indices->begin(), terrainData3->indices->end());


	terrainData->vertices->insert(terrainData->vertices->end(), terrainData1->vertices->begin(), terrainData1->vertices->end());
	terrainData->vertices->insert(terrainData->vertices->end(), terrainData2->vertices->begin(), terrainData2->vertices->end());
	terrainData->vertices->insert(terrainData->vertices->end(), terrainData3->vertices->begin(), terrainData3->vertices->end());


	terrainInds->insert(terrainInds->end(), terrainInds1->begin(), terrainInds1->end());
	terrainInds->insert(terrainInds->end(), terrainInds2->begin(), terrainInds2->end());
	terrainInds->insert(terrainInds->end(), terrainInds3->begin(), terrainInds3->end());

	m_terrainVertexBufferManager = std::make_unique<VertexBufferManager>(terrainData->vertices, CommonObjects::m_deviceResources, m_commandListManager);
	m_terrainIndexBufferManager = std::make_unique<IndexBufferManager>(terrainInds, CommonObjects::m_deviceResources, m_commandListManager);
	m_indexCount = terrainInds->size();

	m_terrainVertexBufferView = m_terrainVertexBufferManager->CreateVertexBufferView();
	m_terrainIndexBufferView = m_terrainIndexBufferManager->CreateIndexBufferView();

	m_descHeapOffset = *descOffset;
	m_cbvSrvHeapManager = descriptorHeapManager;
	m_cbvDescriptorSize = descriptorHeapManager->GetDescriptorSize();
	if (m_usingTexture) {
		const auto textureManager = new TextureResourceManager(std::wstring(pathManager.GetAssetPath() + m_texturePath).c_str(), CommonObjects::m_deviceResources, m_commandListManager);
		textureManager->CreateSRVFromTextureResource(m_cbvSrvHeapManager->Get(), m_cbvDescriptorSize, m_descHeapOffset);
		m_textureDescHeapIndex = m_descHeapOffset;
		m_rootSignInds.push_back(m_textureRootSigIndex);
		m_heapInds.push_back(m_descHeapOffset);
		m_descHeapOffset++;
		(*descOffset)++;
	}

	m_commandListManager = *commandListManager;
}

void TerrainComponent::Update()
{
}

/**
 * @brief renders texture and terrain
 * 
 */
void TerrainComponent::Render()
{
	m_cbvSrvHeapManager->Render(m_rootSignInds.size(), m_rootSignInds.data(), m_heapInds.data(), m_commandListManager);

	m_commandListManager->CreateResourceBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	m_commandListManager->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_commandListManager->SetVertexBuffers(0, 1, &m_terrainVertexBufferView);
	m_commandListManager->SetIndexBuffer(&m_terrainIndexBufferView);
	m_commandListManager->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);

	m_commandListManager->CreateResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
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
