#include "TerrainRenderer.h"
#include "Structures.h"
#include "DirectXHelper.h"
#include "ShaderLoader.h"
#include "VertexBufferManager.h"
#include "IndexBufferManager.h"
#include "TextureResourceManager.h"
#include "BinaryModelLoader.h"
#include "PlayerRenderer.h"
#include "PathManager.h"
#include "RendererRequiredStructures.h"
TerrainRenderer::TerrainRenderer(Structures::RequiredRendererObjects objects) :
	m_deviceResources(objects.deviceResources),
	m_rootSignatureManager(objects.rootSignatureManager),
	m_cameraHelper(objects.cameraHelper),
	m_cbvDescriptorSize(0),
	m_indexCount(0)
{
}

TerrainRenderer::~TerrainRenderer()
{

}

int TerrainRenderer::InitRootSignatureParameters(int indexOffset)
{
	m_rootSigIndexOffset = indexOffset;
	return indexOffset;
}

void TerrainRenderer::RunCommandLists()
{
}

std::shared_ptr<CommandListManager> TerrainRenderer::Init(std::shared_ptr<CommandListManager> commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT* descOffset, std::shared_ptr<PSOManager>* pso)
{
	m_psoManager = *pso;
	m_descHeapOffset = *descOffset;
	m_cbvSrvHeapManager = descriptorHeapManager;
	m_commandListManager = commandListManager;
	CreateDeviceDependentResoures();
	CreateWindowSizeDependentResources();
	*descOffset += 4;
	return m_commandListManager;
}

void TerrainRenderer::Update()
{
	auto rotator = XMFLOAT4X4{};
	XMStoreFloat4x4(&rotator, XMMatrixTranspose(XMMatrixIdentity()));
	std::memcpy(m_rotatorConstantBuffer->GetMappedData(), &rotator, sizeof(XMFLOAT4X4));

	UINT8* destination = m_mvpConstantBufferManager->GetMappedData() + (m_deviceResources->GetCurrentFrameIndex() * m_mvpConstantBufferManager->GetAlignedSize());
	auto cbvData = m_cameraHelper->GetCbvData();
	auto transform = XMMatrixTranspose(XMMatrixIdentity());
	XMStoreFloat4x4(&cbvData.model, transform);
	std::memcpy(destination, &cbvData, sizeof(cbvData));
}

void TerrainRenderer::Render()
{

	UINT rootSigIds[] = {
		0,
		1,
		3 };
	UINT heapIds[] = { m_deviceResources->GetCurrentFrameIndex() + m_descHeapOffset, 3 + m_descHeapOffset, 10 };
	m_cbvSrvHeapManager->Render(3, rootSigIds, heapIds, m_commandListManager);

	m_commandListManager->CreateResourceBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	m_commandListManager->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_commandListManager->SetVertexBuffers(0, 1, &m_terrainVertexBufferView);
	m_commandListManager->SetIndexBuffer(&m_terrainIndexBufferView);
	m_commandListManager->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);

	m_commandListManager->CreateResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	m_commandListManager->CloseAndExcecute();
}

void TerrainRenderer::OnKeyDown(UINT key)
{

}

void TerrainRenderer::OnKeyUp(UINT key)
{

}

void TerrainRenderer::OnMouseMoved(float x, float y)
{
}

void TerrainRenderer::OnDeviceRemoved()
{
}

void TerrainRenderer::CreateDeviceDependentResoures()
{
	auto pathManager = PathManager();

	m_terrainGenerator = std::make_unique<TerrainGenerationHelper>();
	TerrainCollisionHelper::SetTerrainHeights(m_terrainGenerator->GetChunks());
	TerrainCollisionHelper::SetTerrainOrigins(m_terrainGenerator->GetChunkOrigins());
	auto terrainData = m_terrainGenerator->GenTerrain(0, 0);
	auto terrainData1 = m_terrainGenerator->GenTerrain(250, 0);
	auto terrainData2 = m_terrainGenerator->GenTerrain(0, 250);
	auto terrainData3 = m_terrainGenerator->GenTerrain(250, 250);

	auto terrainInds = std::make_shared<std::vector<unsigned int>>(terrainData.indices->begin(), terrainData.indices->end());
	auto terrainInds1 = std::make_shared<std::vector<unsigned int>>(terrainData1.indices->begin(), terrainData1.indices->end());
	auto terrainInds2 = std::make_shared<std::vector<unsigned int>>(terrainData2.indices->begin(), terrainData2.indices->end());
	auto terrainInds3 = std::make_shared<std::vector<unsigned int>>(terrainData3.indices->begin(), terrainData3.indices->end());


	terrainData.vertices->insert(terrainData.vertices->end(), terrainData1.vertices->begin(), terrainData1.vertices->end());
	terrainData.vertices->insert(terrainData.vertices->end(), terrainData2.vertices->begin(), terrainData2.vertices->end());
	terrainData.vertices->insert(terrainData.vertices->end(), terrainData3.vertices->begin(), terrainData3.vertices->end());


	terrainInds->insert(terrainInds->end(), terrainInds1->begin(), terrainInds1->end());
	terrainInds->insert(terrainInds->end(), terrainInds2->begin(), terrainInds2->end());
	terrainInds->insert(terrainInds->end(), terrainInds3->begin(), terrainInds3->end());


	m_cbvDescriptorSize = m_cbvSrvHeapManager->GetDescriptorSize();


	m_mvpConstantBufferManager = std::make_unique<ConstantBufferManager<Structures::ModelViewProjectionConstantBuffer>>(DX::c_frameCount, m_mvpConstantBufferManager->GetAlignedSize(), m_deviceResources, m_commandListManager);
	for (int n = 0; n < DX::c_frameCount; n++)
	{
		m_mvpConstantBufferManager->CreateBufferDesc(m_mvpConstantBufferManager->GetAlignedSize(), n + m_descHeapOffset, m_cbvSrvHeapManager, m_cbvDescriptorSize);
	}

	m_rotatorConstantBuffer = std::make_unique<ConstantBufferManager<XMFLOAT4X4>>(1, m_rotatorConstantBuffer->GetAlignedSize(), m_deviceResources, m_commandListManager);
	m_rotatorConstantBuffer->CreateBufferDesc(m_rotatorConstantBuffer->GetAlignedSize(), 10, m_cbvSrvHeapManager, m_cbvDescriptorSize);

	m_terrainVertexBufferManager = std::make_unique<VertexBufferManager>(terrainData.vertices, m_deviceResources, m_commandListManager);
	m_terrainIndexBufferManager = std::make_unique<IndexBufferManager>(terrainInds, m_deviceResources, m_commandListManager);
	m_indexCount = terrainInds->size();

	const auto textureManager = new TextureResourceManager(std::wstring(pathManager.GetAssetPath() + std::wstring(L"\\terrain.png")).c_str(), m_deviceResources, m_commandListManager);
	textureManager->CreateSRVFromTextureResource(m_cbvSrvHeapManager->Get(), m_cbvDescriptorSize, 3 + m_descHeapOffset);

	m_terrainVertexBufferView = m_terrainVertexBufferManager->CreateVertexBufferView();
	m_terrainIndexBufferView = m_terrainIndexBufferManager->CreateIndexBufferView();

	terrainInds->clear();
	std::vector<unsigned int>().swap(*terrainInds);

	terrainData.vertices->clear();
	std::vector<Structures::VertexTexCoordNormal>().swap(*terrainData.vertices);

	terrainData.vertices->clear();
	std::vector<unsigned long>().swap(*terrainData.indices);
}

void TerrainRenderer::CreateWindowSizeDependentResources()
{
}
