#include "stdafx.h"
#include "DescriptorHeapManager.h"
#include "DeviceResources.h"
#include "DirectXHelper.h"
#include "CommandListManager.h"

DescriptorHeapManager::DescriptorHeapManager(const int numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type,
	D3D12_DESCRIPTOR_HEAP_FLAGS flags, const std::shared_ptr<DX::DeviceResources> deviceResources): m_type(type), m_deviceResources(deviceResources)
{
	m_desc = D3D12_DESCRIPTOR_HEAP_DESC{};
	m_desc.NumDescriptors = numDescriptors;
	m_desc.Type = type;
	m_desc.Flags = flags;
	ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateDescriptorHeap(&m_desc, IID_PPV_ARGS(&m_heap)));
	//NAME_D3D12_OBJECT(m_heap);
}

DescriptorHeapManager::~DescriptorHeapManager()
{
}

void DescriptorHeapManager::Render(UINT count, UINT* rootSigIndex, UINT* heapIndex, std::shared_ptr<CommandListManager> commandListManager)
{
	auto size = this->GetDescriptorSize();
	for (auto i = 0; i < count; i++) {
		const CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(this->GetGPUDescriptorHandleForHeapStart(), heapIndex[i], size);
		commandListManager->SetGraphicsRootDescriptorTable(rootSigIndex[i], gpuHandle);
	}
}
