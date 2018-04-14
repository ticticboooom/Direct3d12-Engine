#pragma once
#include "stdafx.h"
#include "ResourceManager.h"
#include "DirectXHelper.h"

template <class TData>
class ConstantBufferManager
{
public:
	ConstantBufferManager(const UINT size, const UINT count, const std::shared_ptr<DX::DeviceResources>& deviceResources,
		const std::shared_ptr<CommandListManager> commandListManager);
	~ConstantBufferManager();
	void CreateBufferDesc(const UINT size, UINT heapIndex, std::shared_ptr<DescriptorHeapManager> heapManager, UINT DescriptorSize);
	static UINT GetAlignedSize() { return c_alignedBufferSize; };
	UINT GetBufferAddress() const { return m_resourceManager->Get()->GetGPUVirtualAddress(); };
	UINT8* GetMappedData() const { return m_mappedBufferData; };

private:
	static const UINT c_alignedBufferSize = (sizeof(TData) + 255) & ~255;
	std::unique_ptr<ResourceManager> m_resourceManager;
	UINT8* m_mappedBufferData;
	const std::shared_ptr<DX::DeviceResources>& m_deviceResources;
	const std::shared_ptr<CommandListManager> m_commandListManager;
	CD3DX12_CPU_DESCRIPTOR_HANDLE cbvCpuHandle;
	D3D12_GPU_VIRTUAL_ADDRESS cbvGpuAddress;
	UINT sizeInBytes;
	bool hasCreated = false;
};

template <class TData>
ConstantBufferManager<TData>::ConstantBufferManager(const UINT size, const UINT count, const std::shared_ptr<DX::DeviceResources>& deviceResources,
	const std::shared_ptr<CommandListManager> commandListManager) :
	m_deviceResources(deviceResources),
	m_commandListManager(commandListManager)
{
	const CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(size * count);

	m_resourceManager = std::make_unique<ResourceManager>(m_deviceResources, m_commandListManager);
	m_resourceManager->UploadResource(bufferDesc);

	CD3DX12_RANGE readRange(0, 0);
	ThrowIfFailed(m_resourceManager->Get()->Map(0, &readRange, reinterpret_cast<void**>(&m_mappedBufferData)));
	ZeroMemory(m_mappedBufferData, size * count);
}

template <class TData>
ConstantBufferManager<TData>::~ConstantBufferManager()
{
}

template <class TData>
void ConstantBufferManager<TData>::CreateBufferDesc(const UINT size, UINT heapIndex, std::shared_ptr<DescriptorHeapManager> heapManager, UINT DescriptorSize)
{
	if (!hasCreated) {
		auto start = heapManager->GetCPUDescriptorHandleForHeapStart();
		CD3DX12_CPU_DESCRIPTOR_HANDLE CpuHandle(start, heapIndex, DescriptorSize);
		D3D12_GPU_VIRTUAL_ADDRESS GpuAddress(this->GetBufferAddress());
		cbvCpuHandle = CpuHandle;
		cbvGpuAddress = GpuAddress;
		hasCreated = true;
	}
	else {
		cbvCpuHandle.Offset(DescriptorSize);
		cbvGpuAddress += sizeInBytes;
	}
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = cbvGpuAddress;
	desc.SizeInBytes = size;
	m_deviceResources->GetD3DDevice()->CreateConstantBufferView(&desc, cbvCpuHandle);
	sizeInBytes = desc.SizeInBytes;
}

