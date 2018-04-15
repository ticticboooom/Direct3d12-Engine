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
	std::unique_ptr<ResourceManager> m_resourceManager; /// resource manager
	UINT8* m_mappedBufferData; /// the data mapped to the resource
	const std::shared_ptr<DX::DeviceResources>& m_deviceResources; /// device resources
	const std::shared_ptr<CommandListManager> m_commandListManager; /// command list manager
	CD3DX12_CPU_DESCRIPTOR_HANDLE cbvCpuHandle; ///the constant buffer view cpu handle
	D3D12_GPU_VIRTUAL_ADDRESS cbvGpuAddress; ///the gpu address of the resource
	UINT sizeInBytes; /// the size of the resourc ein bytes
	bool hasCreated = false; /// has the buffer view been created
};

/**
 * @brief Construct a new Constant Buffer Manager< T Data>:: Constant Buffer Manager object
 * @detail constant buffers are for setting constant data inside of a shader (per frame) do that they can use the data without passing in seperate data every vertex
 * @tparam TData the data of the constant buffer
 * @param size the size of the constant buffer data
 * @param count the count of how many to use (usually for multi-buffering)
 * @param deviceResources 
 * @param commandListManager 
 */
template <class TData>
ConstantBufferManager<TData>::ConstantBufferManager(const UINT size, const UINT count, const std::shared_ptr<DX::DeviceResources>& deviceResources,
	const std::shared_ptr<CommandListManager> commandListManager) :
	m_deviceResources(deviceResources),
	m_commandListManager(commandListManager)
{
	// creates resource description from sixe and count
	const CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(size * count);

	// creates the reousce and gives it an upload buffer
	m_resourceManager = std::make_unique<ResourceManager>(m_deviceResources, m_commandListManager);
	m_resourceManager->UploadResource(bufferDesc);

	// map the buffer to a variable to change the data
	CD3DX12_RANGE readRange(0, 0);
	ThrowIfFailed(m_resourceManager->Get()->Map(0, &readRange, reinterpret_cast<void**>(&m_mappedBufferData)));
	ZeroMemory(m_mappedBufferData, size * count);
}

/**
 * @brief Destroy the Constant Buffer Manager< T Data>:: Constant Buffer Manager object
 * 
 * @tparam TData the data of the constant buffer
 */
template <class TData>
ConstantBufferManager<TData>::~ConstantBufferManager()
{
}

/**
 * @brief Creater the buffer view for the constant buffer
 * 
 * @tparam TData the data of the constant buffer
 * @param size the size of the buffer view (usually the aligned size of the buffer view)
 * @param heapIndex the index on the descriptor heap to use
 * @param heapManager the DescriptorHeapManager class for the buffer to use
 * @param DescriptorSize the size of the descriptor
 */
template <class TData>
void ConstantBufferManager<TData>::CreateBufferDesc(const UINT size, UINT heapIndex, std::shared_ptr<DescriptorHeapManager> heapManager, UINT DescriptorSize)
{
	if (!hasCreated) {
		// Gets the CPU habdle for the start of the descirptor heap
		auto start = heapManager->GetCPUDescriptorHandleForHeapStart();
		// Creates a CPU handle using the heap index, start and descitpor size
		CD3DX12_CPU_DESCRIPTOR_HANDLE CpuHandle(start, heapIndex, DescriptorSize);
		// gets buffer GPU address
		D3D12_GPU_VIRTUAL_ADDRESS GpuAddress(this->GetBufferAddress());
		// sets the corresponding member variables 
		cbvCpuHandle = CpuHandle;
		cbvGpuAddress = GpuAddress;
		hasCreated = true;
	}
	else {
		// ofsets the cpu handle
		cbvCpuHandle.Offset(DescriptorSize);
		cbvGpuAddress += sizeInBytes;
	}
	// describes and creates the cbv (constant buffer view)
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = cbvGpuAddress;
	desc.SizeInBytes = size;
	m_deviceResources->GetD3DDevice()->CreateConstantBufferView(&desc, cbvCpuHandle);
	sizeInBytes = desc.SizeInBytes;
}

