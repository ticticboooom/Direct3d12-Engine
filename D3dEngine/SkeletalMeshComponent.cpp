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
SkeletalMeshComponent::SkeletalMeshComponent(std::string filename) : Mesh(filename, true),
m_anim(0), m_interpFrame(0), m_interpIndex0(0), m_interpIndex1(0), isInterping(false), m_frame(0)
{
	m_descriptorCount += m_meshData->animations->size();
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
	m_cbvDescriptorSize = CommonObjects::m_descriptorHeapManager->GetDescriptorSize();

	m_animationManager = std::make_unique<AnimationManager>(m_meshData->animations);
	auto animCount = m_animationManager->GetAnimCount();
	auto bufferSize = 0u;
	for (auto i = 0; i < animCount; i++) {
		m_perAnimBufferOffset.push_back(bufferSize);
		bufferSize += m_animationManager->GetBoneCount(i);
	}

	m_animationConstantBufferManager = std::make_unique<ConstantBufferManager<XMFLOAT4X4>>(1,
		m_animationConstantBufferManager->GetAlignedSize() * bufferSize,
		CommonObjects::m_deviceResources,
		CommonObjects::m_commandListManager);

	for (auto i = 0; i < animCount; i++) {
		m_animationConstantBufferManager->CreateBufferDesc(m_animationConstantBufferManager->GetAlignedSize() * m_animationManager->GetBoneCount(i), CommonObjects::m_descriptorHeapIndexOffset, CommonObjects::m_descriptorHeapManager, m_cbvDescriptorSize);
		m_animDescHeapIndicies.push_back(CommonObjects::m_descriptorHeapIndexOffset);
		CommonObjects::m_descriptorHeapIndexOffset++;
	}
	m_heapInds.push_back(m_animDescHeapIndicies[0]);
	m_rootSignInds.push_back(m_animRootSigIndex);
	m_animHeapIndex = m_heapInds.size() - 1;
	Mesh::Init();
}
/**
 * @brief runs the animations
 *
 */
void SkeletalMeshComponent::Update()
{
	m_frame++;
	if (m_frame >= m_animationManager->GetFrameCount(m_anim, 0)) {
		m_frame = 0;
	}
	std::vector<XMFLOAT4X4> cbvData;
	if (isInterping) {
		m_interpFrame++;
		if (m_interpFrame >= m_animationManager->GetFrameCount(m_interpIndex1, 0)) {
			m_interpFrame = 0;
		}
		cbvData = m_animationManager->BlendAnimationsAtPositions(m_anim, m_interpIndex1, m_frame, m_interpFrame, interpT);
		interpT += interpInterval;
		if (interpT >= 1.f) {
			interpT = 0;
			isInterping = false;
			m_anim = m_interpIndex1;
			m_frame = m_interpFrame;
			m_heapInds[m_animHeapIndex] = m_animDescHeapIndicies[m_anim];
		}
	}
	else {
		cbvData = m_animationManager->GetPositioninAnim(m_anim, m_frame);
	}
	


	UINT8* destination = m_animationConstantBufferManager->GetMappedData() + (m_perAnimBufferOffset[m_anim] * m_animationConstantBufferManager->GetAlignedSize());
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

void SkeletalMeshComponent::CreateDeviceDependentResources()
{
	Mesh::CreateDeviceDependentResources();
}

void SkeletalMeshComponent::SetAnimInUse(UINT index)
{
	if (m_animDescHeapIndicies.size() > index) {
		m_anim = index;
		m_heapInds[m_animHeapIndex] = m_animDescHeapIndicies[index];
	}
}

void SkeletalMeshComponent::InterpFromTo(const int index0, const int index1, const float interval, const int frameFlag)
{
	m_anim = index0;
	isInterping = true;
	interpT = 0.0f;
	interpInterval = interval;
	m_interpIndex0 = index0;
	m_interpIndex1 = index1;
	m_heapInds[m_animHeapIndex] = m_animDescHeapIndicies[index0];
	m_stationaryIndex = frameFlag;
	if (frameFlag == 0) {
		m_frame = m_interpFrame;
	}
}

