#include "stdafx.h"
#include "Node.h"
#include "CommonObjects.h"
UINT Node::m_mvpRootSigIndex = 0;
bool Node::m_isRootSignatureInitialised = false;
/**
 * @brief Construct a new Node:: Node object
 * 	stores components and has transform 
 * 	used for each thing that can be moved seperately
 * 
 */
Node::Node() : Component(), m_compManager(true)
{
	m_transform = std::make_shared<Structures::Transform>();
	m_mvpManager = std::make_shared<ModelViewProjectionManager>();
}

Node::~Node()
{
}
/**
 * @brief adds mvp root signature index parameter
 * 
 * @param indexOffset 
 * @return int 
 */
int Node::InitRootSignatureParameters(int indexOffset)
{
	if (!m_isRootSignatureInitialised) {
		std::shared_ptr<RootParameter> param = std::make_shared<RootParameter>();
		param->InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 0, 1, D3D12_SHADER_VISIBILITY_VERTEX);
		(*CommonObjects::m_rootSignatureManager)[0]->AddNewParameter(param);
		m_mvpRootSigIndex = indexOffset;
		indexOffset++;
		m_isRootSignatureInitialised = true;
	}

	return m_compManager.InitRootSignatureParameters(indexOffset);
}

/**
 * @brief loads the mvp into GPU
 * 
 * @param commandListManager 
 * @param descriptorHeapManager 
 * @param descOffset 
 * @param pso 
 */
void Node::Init(std::shared_ptr<CommandListManager>* commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT * descOffset, std::shared_ptr<PSOManager>* pso)
{
	m_commandListManager = *commandListManager;
	m_cbvSrvHeapManager = descriptorHeapManager;
	m_cbvDescriptorSize = descriptorHeapManager->GetDescriptorSize();

	m_mvpConstantBufferManager = std::make_unique<ConstantBufferManager<Structures::ModelViewProjectionConstantBuffer>>(1, m_mvpConstantBufferManager->GetAlignedSize(), CommonObjects::m_deviceResources, m_commandListManager);
	m_mvpConstantBufferManager->CreateBufferDesc(m_mvpConstantBufferManager->GetAlignedSize(), *descOffset, descriptorHeapManager, m_cbvDescriptorSize);
	m_mvpDescHeapIndex = *descOffset;
	m_rootSignInds.push_back(m_mvpRootSigIndex);
	m_heapInds.push_back(m_mvpDescHeapIndex);
	(*descOffset)++;
	m_compManager.Init(commandListManager, descriptorHeapManager, descOffset, pso);
}
/**
 * @brief sets the mvp data to mapped GPU address
 * 
 */
void Node::Update()
{
	m_compManager.m_transform = m_transform;

	m_mvpManager->SetMatrix(MATKEY_MODEL, XMMatrixTranspose(XMMatrixAffineTransformation(m_transform->scale, XMVectorSet(0, 0, 0, 0), m_transform->rotationQuat, m_transform->position)));
	m_mvpManager->SetMatrix(MATKEY_VIEW, XMLoadFloat4x4(&m_view));
	m_mvpManager->SetMatrix(MATKEY_PROJECTION, XMLoadFloat4x4(&m_projection));

	auto cbvData = m_mvpManager->GetCbvData();
	UINT8* destination = m_mvpConstantBufferManager->GetMappedData();
	std::memcpy(destination, &cbvData, sizeof(cbvData));
	m_compManager.Update();
}

/**
 * @brief renders the mvp to be used by node
 * 
 */
void Node::Render()
{
	m_cbvSrvHeapManager->Render(m_rootSignInds.size(), m_rootSignInds.data(), m_heapInds.data(), m_commandListManager);
	m_compManager.Render();
}

void Node::OnKeyDown(UINT key)
{
	m_compManager.OnKeyDown(key);
}

void Node::OnKeyUp(UINT key)
{
	m_compManager.OnKeyUp(key);
}

void Node::OnMouseMoved(float x, float y)
{
	m_compManager.OnMouseMoved(x, y);
}

void Node::OnDeviceRemoved()
{
	m_compManager.OnDeviceRemoved();
}

void Node::CreateWindowSizeDependentResources()
{
	m_compManager.CreateWindowSizeDependentResources();
}

void Node::CreateDeviceDependentResoures()
{
	m_compManager.CreateDeviceDependentResoures();
}

void Node::AddComponent(std::shared_ptr<Component> comp)
{
	m_compManager.AddComponent(comp);
}
