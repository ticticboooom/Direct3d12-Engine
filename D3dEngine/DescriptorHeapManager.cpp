#include "stdafx.h"
#include "DescriptorHeapManager.h"
#include "DeviceResources.h"
#include "DirectXHelper.h"
#include "CommandListManager.h"

/**
 * @brief Construct a new Descriptor Heap Manager:: Descriptor Heap Manager object
 *  the descriptor heap stores the descriptors of the resources used in heap which is used to get the resourc on rendering along with the root signature to give the GPU resources to render
 * @param numDescriptors the number of descriptors to be created
 * @param type the type of descriptor heap that is used
 * @param flags the flags to be used on creation
 * @param deviceResources 
 */
DescriptorHeapManager::DescriptorHeapManager(const int numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type,
	D3D12_DESCRIPTOR_HEAP_FLAGS flags, const std::shared_ptr<DX::DeviceResources> deviceResources): m_type(type), m_deviceResources(deviceResources)
{
	// describes and creates the descriptor heap
	m_desc = D3D12_DESCRIPTOR_HEAP_DESC{};
	m_desc.NumDescriptors = numDescriptors;
	m_desc.Type = type;
	m_desc.Flags = flags;
	ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateDescriptorHeap(&m_desc, IID_PPV_ARGS(&m_heap)));
	//NAME_D3D12_OBJECT(m_heap);
}
/**
 * @brief Destroy the Descriptor Heap Manager:: Descriptor Heap Manager object
 * 
 */
DescriptorHeapManager::~DescriptorHeapManager()
{
}
/**
 * @brief renders each item added to the descriptor heap and root signature
 * 
 * @param count the number of both root signature indices and descriptor heasp indices
 * @param rootSigIndex the indies from the root signature
 * @param heapIndex the indices from the descriptor heap
 * @param commandListManager 
 */
void DescriptorHeapManager::Render(UINT count, UINT* rootSigIndex, UINT* heapIndex, std::shared_ptr<CommandListManager> commandListManager)
{
	auto size = this->GetDescriptorSize();
	for (auto i = 0; i < count; i++) {
		const CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(this->GetGPUDescriptorHandleForHeapStart(), heapIndex[i], size);
		commandListManager->SetGraphicsRootDescriptorTable(rootSigIndex[i], gpuHandle);
	}
}
