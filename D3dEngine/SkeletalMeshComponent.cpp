#include "stdafx.h"
#include "SkeletalMeshComponent.h"
#include "CommonObjects.h"

bool SkeletalMeshComponent::m_isRootSignatureInitialised = false;
UINT SkeletalMeshComponent::m_animRootSigIndex = 0u;
/**
 * @brief Construct a new Skeletal Mesh Component:: Skeletal Mesh Component object
 * creates a mesh that is animated with skel;etal animation (skinning)
 * 
 * @param filename name of file to load as mesh
 */
SkeletalMeshComponent::SkeletalMeshComponent(std::string filename) : Mesh(filename, true)
{
	m_descriptorCount += 1;
}

/**
 * @brief Destroy the Skeletal Mesh Component:: Skeletal Mesh Component object
 * 
 */
SkeletalMeshComponent::~SkeletalMeshComponent()
{
}
/**
 * @brief creates the root signature parameter for the animation constant buffer
 * 
 * @param indexOffset 
 * @return int 
 */
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
/**
 * @brief creates the constant buffer for the animation of the mesh
 * 
 * @param commandListManager 
 * @param descriptorHeapManager 
 * @param descOffset 
 * @param pso 
 */

void SkeletalMeshComponent::Init()
{
	m_animationManager = std::make_unique<AnimationManager>(m_meshData->animations);
	m_animationConstantBufferManager = std::make_unique<ConstantBufferManager<XMFLOAT4X4>>(1,
		m_animationConstantBufferManager->GetAlignedSize() * m_animationManager->GetPositioninAnim(0, 0).size(),
		CommonObjects::m_deviceResources,
		CommonObjects::m_commandListManager);
	m_cbvDescriptorSize = CommonObjects::m_descriptorHeapManager->GetDescriptorSize();
	m_animationConstantBufferManager->CreateBufferDesc(m_animationConstantBufferManager->GetAlignedSize() * m_animationManager->GetPositioninAnim(0, 0).size(), CommonObjects::m_descriptorHeapIndexOffset, CommonObjects::m_descriptorHeapManager, m_cbvDescriptorSize);
	m_animDescHeapIndex = CommonObjects::m_descriptorHeapIndexOffset;

	m_rootSignInds.push_back(m_animRootSigIndex);
	m_heapInds.push_back(m_animDescHeapIndex);
	CommonObjects::m_descriptorHeapIndexOffset++;
	Mesh::Init();
}
/**
 * @brief runs the animations
 * 
 */
void SkeletalMeshComponent::Update()
{
	m_frame++;
	if (m_frame >= m_animationManager->GetFrameCount(0, 0)) {
		m_frame = 0;
	}
	auto cbvData = m_animationManager->GetPositioninAnim(0, m_frame);
	UINT8* destination = m_animationConstantBufferManager->GetMappedData();
	std::memcpy(destination, cbvData.data(), sizeof(XMFLOAT4X4) * cbvData.size());
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