#include "stdafx.h"
#include "ResourceManager.h"
#include "DirectXHelper.h"

ResourceManager::~ResourceManager()
{
}

void ResourceManager::UpdateSubresource(BYTE* data, UINT size, const UINT width, const UINT height, const D3D12_RESOURCE_STATES state) const
{
	D3D12_SUBRESOURCE_DATA subresource = {};
	subresource.pData = data;
	subresource.RowPitch = width;
	subresource.SlicePitch = width * height;
	m_commandListManager->UpdateSubresource(m_resource.Get(), m_resourceUpload.Get(), &subresource);
	m_commandListManager->CreateResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, state));
}

UINT64 ResourceManager::GetRequiredIntermediateSizeForResource() const
{
	return  GetRequiredIntermediateSize(m_resource.Get(), 0, 1);

}

ResourceManager::ResourceManager(const std::shared_ptr<DX::DeviceResources> deviceResources,
	const std::shared_ptr<CommandListManager> commandListManager) :
	m_deviceResources(deviceResources),
	m_commandListManager(commandListManager)
{
}

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
