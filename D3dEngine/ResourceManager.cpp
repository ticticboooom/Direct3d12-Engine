#include "stdafx.h"
#include "ResourceManager.h"
#include "DirectXHelper.h"
/**
 * @brief Destroy the Resource Manager:: Resource Manager object
 * 
 */
ResourceManager::~ResourceManager()
{
}

/**
 * @brief update the subresource of the resouce 
 * 
 * @param data the subresource's data
 * @param size the size of the data
 * @param width the width of the data
 * @param height the height of the data
 * @param state the state of the subresource
 */
void ResourceManager::UpdateSubresource(BYTE* data, UINT size, const UINT width, const UINT height, const D3D12_RESOURCE_STATES state) const
{
	D3D12_SUBRESOURCE_DATA subresource = {};
	subresource.pData = data;
	subresource.RowPitch = width;
	subresource.SlicePitch = width * height;
	m_commandListManager->UpdateSubresource(m_resource.Get(), m_resourceUpload.Get(), &subresource);
	m_commandListManager->CreateResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, state));
}

/**
 * @brief get the required size for a resource
 * 
 * @return UINT64 the required size
 */
UINT64 ResourceManager::GetRequiredIntermediateSizeForResource() const
{
	return  GetRequiredIntermediateSize(m_resource.Get(), 0, 1);

}

/**
 * @brief Construct a new Resource Manager:: Resource Manager object
 * 
 * @param deviceResources 
 * @param commandListManager 
 */
ResourceManager::ResourceManager(const std::shared_ptr<DX::DeviceResources> deviceResources,
	const std::shared_ptr<CommandListManager> commandListManager) :
	m_deviceResources(deviceResources),
	m_commandListManager(commandListManager)
{
}

/**
 * @brief create upload resource with the upload heap to upload the data
 * 
 * @param desc  resource descriptor
 */
void ResourceManager::UploadResource(D3D12_RESOURCE_DESC desc)
{
	CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	auto result = m_deviceResources->GetD3DDevice()->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_resourceUpload));
		ThrowIfFailed(result);
}

/**
 * @brief creates a resource that cannot be mapped to a vriable and must be changed using update subresource
 * 
 * @param desc 
 */
void ResourceManager::DefaultHeapResource(D3D12_RESOURCE_DESC desc)
{
	CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateCommittedResource(
		&defaultHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_resource)));
	m_hasDefaultHeap = true;
}
