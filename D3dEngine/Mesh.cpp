#include "stdafx.h"
#include "Mesh.h"
#include "BinaryModelLoader.h"
#include "PathManager.h"
#include "CommonObjects.h"
#include "UtilityRenderer.h"

bool Mesh::m_isRootSignatureInitialised = false;
UINT Mesh::m_textureRootSigIndex = 0;
/**
 * @brief Construct a new Mesh:: Mesh object
 *  base class for  model / 3d object (coxl)
 * 
 * @param filename the filename of the model to load
 * @param isAnimated is the model animated
 * @param isZUp is 
 */
Mesh::Mesh(std::string filename, bool isAnimated) : Component()
{
	auto modelLoader = std::make_unique<BinaryModelLoader>(filename.c_str(), isAnimated);
	m_meshData = modelLoader->GetFinalData();
	m_indexCount = m_meshData->indices->size();
	m_meshLoaded = m_meshData->vertices->size() > 0;
	m_descriptorCount += 1;
}


Mesh::~Mesh()
{

}
/**
 * @brief adds texture root signature parameter
 * 
 * @param indexOffset 
 * @return int 
 */
int Mesh::InitRootSignatureParameters(int indexOffset)
{
	if (m_usingTexture && !m_isRootSignatureInitialised)
	{
		std::shared_ptr<RootParameter> param = std::make_shared<RootParameter>();
		param->InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
		(*CommonObjects::m_rootSignatureManager)[0]->AddNewParameter(param);
		m_textureRootSigIndex = indexOffset;
		indexOffset++;
		m_isRootSignatureInitialised = true;
	}
	return indexOffset;
}

/**
 * @brief loads texture and mesh into GPU
 * 
 * @param commandListManager 
 * @param descriptorHeapManager 
 * @param descOffset 
 * @param pso 
 */
void Mesh::Init()
{
	m_cbvDescriptorSize = CommonObjects::m_descriptorHeapManager->GetDescriptorSize();

	auto pathManager = PathManager();
	if (m_usingTexture) {
		const auto textureManager = new TextureResourceManager(std::wstring(pathManager.GetAssetPath() + m_texturePath).c_str(), CommonObjects::m_deviceResources, CommonObjects::m_commandListManager);
		textureManager->CreateSRVFromTextureResource(CommonObjects::m_descriptorHeapManager->Get(), m_cbvDescriptorSize, CommonObjects::m_descriptorHeapIndexOffset);
		m_textureDescHeapIndex = CommonObjects::m_descriptorHeapIndexOffset;
		m_rootSignInds.push_back(m_textureRootSigIndex);
		m_heapInds.push_back(CommonObjects::m_descriptorHeapIndexOffset);
		CommonObjects::m_descriptorHeapIndexOffset++;
	}
	if (m_meshLoaded) {
		auto inds = std::make_shared<std::vector<unsigned int>>(m_meshData->indices->begin(), m_meshData->indices->end());
		m_vertexBufferManager = std::make_unique<VertexBufferManager>(m_meshData->vertices, CommonObjects::m_deviceResources, CommonObjects::m_commandListManager);
		m_indexBufferManager = std::make_unique<IndexBufferManager>(inds, CommonObjects::m_deviceResources, CommonObjects::m_commandListManager);

		m_meshData->indices->clear();
		m_meshData->vertices->clear();
		inds->clear();

		m_vertexBufferView = m_vertexBufferManager->CreateVertexBufferView();
		m_indexBufferView = m_indexBufferManager->CreateIndexBufferView();
	}
}

void Mesh::Update()
{

}
/**
 * @brief renders mesh with texture
 * 
 */
void Mesh::Render()
{
	CommonObjects::m_descriptorHeapManager->Render(m_rootSignInds.size(), m_rootSignInds.data(), m_heapInds.data(), CommonObjects::m_commandListManager);

	if (m_meshLoaded) {
		CommonObjects::m_commandListManager->CreateResourceBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

		CommonObjects::m_commandListManager->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		CommonObjects::m_commandListManager->SetVertexBuffers(0, 1, &m_vertexBufferView);
		CommonObjects::m_commandListManager->SetIndexBuffer(&m_indexBufferView);
		CommonObjects::m_commandListManager->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);

		CommonObjects::m_commandListManager->CreateResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	}
}

void Mesh::OnKeyDown(UINT key)
{
}

void Mesh::OnKeyUp(UINT key)
{
}

void Mesh::OnMouseMoved(float x, float y)
{
}

void Mesh::OnDeviceRemoved()
{
}

void Mesh::CreateWindowSizeDependentResources()
{
}

void Mesh::CreateDeviceDependentResources()
{
}

/**
 * @brief defines whether and where to load texture on Init
 * 
 * @param filename 
 */
void Mesh::UseTexture(std::wstring filename)
{
	m_usingTexture = true;
	m_texturePath = filename;
}
