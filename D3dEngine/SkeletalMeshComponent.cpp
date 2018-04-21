#include "stdafx.h"
#include "SkeletalMeshComponent.h"
#include "CommonObjects.h"

bool SkeletalMeshComponent::m_isRootSignatureInitialised = false;
SkeletalMeshComponent::SkeletalMeshComponent(std::string filename) : Mesh(filename, true)
{

}


SkeletalMeshComponent::~SkeletalMeshComponent()
{
}

int SkeletalMeshComponent::InitRootSignatureParameters(int indexOffset)
{
	if (!m_isRootSignatureInitialised) {
		std::shared_ptr<RootParameter> param = std::make_shared<RootParameter>();
		param->InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1, D3D12_SHADER_VISIBILITY_VERTEX);
		(*CommonObjects::m_rootSignatureManager)[0]->AddNewParameter(param);
		m_animRootSigIndex = indexOffset;
		indexOffset++;
		m_isRootSignatureInitialised = true;
	}
	return Mesh::InitRootSignatureParameters(indexOffset);
}

void SkeletalMeshComponent::Init(std::shared_ptr<CommandListManager>* commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT * descOffset, std::shared_ptr<PSOManager>* pso)
{
	m_animationManager = std::make_unique<AnimationManager>(m_meshData->animations);
	m_commandListManager = *commandListManager;
	m_cbvSrvHeapManager = descriptorHeapManager;
	m_descHeapOffset = *descOffset;
	m_animationConstantBufferManager = std::make_unique<ConstantBufferManager<XMFLOAT4X4>>(1,
		m_animationConstantBufferManager->GetAlignedSize() * m_animationManager->GetPositioninAnim(0, 0).size(),
		CommonObjects::m_deviceResources,
		m_commandListManager);
	m_cbvDescriptorSize = m_cbvSrvHeapManager->GetDescriptorSize();
	m_animationConstantBufferManager->CreateBufferDesc(m_animationConstantBufferManager->GetAlignedSize() * m_animationManager->GetPositioninAnim(0, 0).size(), m_descHeapOffset, m_cbvSrvHeapManager, m_cbvDescriptorSize);
	m_animDescHeapIndex = m_descHeapOffset;

	m_rootSignInds.push_back(m_animRootSigIndex);
	m_heapInds.push_back(m_animDescHeapIndex);
	(*descOffset)++;
	Mesh::Init(commandListManager, descriptorHeapManager, descOffset, pso);
}

void SkeletalMeshComponent::Update()
{
	m_frame++;
	if (m_frame >= m_animationManager->GetFrameCount(0, 0)) {
		m_frame = 0;
	}
	auto cbvData = m_animationManager->GetPositioninAnim(0, m_frame);
	UINT8* destination = m_animationConstantBufferManager->GetMappedData();
	std::memcpy(destination, &cbvData, sizeof(cbvData));
	Mesh::Update();
}

void SkeletalMeshComponent::Render()
{
	Mesh::Render();
}

void SkeletalMeshComponent::OnKeyDown(UINT key)
{
	Mesh::OnKeyDown(key);
}

void SkeletalMeshComponent::OnKeyUp(UINT key)
{
	Mesh::OnKeyUp(key);
}

void SkeletalMeshComponent::OnMouseMoved(float x, float y)
{
	Mesh::OnMouseMoved(x, y);
}

void SkeletalMeshComponent::OnDeviceRemoved()
{
	Mesh::OnDeviceRemoved();
}

void SkeletalMeshComponent::CreateWindowSizeDependentResources()
{
	Mesh::CreateWindowSizeDependentResources();
}

void SkeletalMeshComponent::CreateDeviceDependentResoures()
{
	Mesh::CreateDeviceDependentResoures();
}