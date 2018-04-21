#include "stdafx.h"
#include "TerrainComponent.h"
#include "PathManager.h"
#include "CommonObjects.h"

TerrainComponent::TerrainComponent()
{

}


TerrainComponent::~TerrainComponent()
{
}

int TerrainComponent::InitRootSignatureParameters(int indexOffset)
{
	return 0;
}

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

	m_commandListManager = *commandListManager;
}

void TerrainComponent::Update()
{
}

void TerrainComponent::Render()
{
	//m_cbvSrvHeapManager->Render(m_rootSignInds.size(), m_rootSignInds.data(), m_heapInds.data(), m_commandListManager);

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
