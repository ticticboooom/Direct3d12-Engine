#include "stdafx.h"
#include "BufferManagerBase.h"

BufferManagerBase::BufferManagerBase(UINT size, BYTE* data, D3D12_RESOURCE_STATES state, std::shared_ptr<DX::DeviceResources> deviceResources,
	std::shared_ptr<CommandListManager> commandListManager)
{
	const CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(size);
	m_resourceManager = std::make_unique<ResourceManager>(
		ResourceManager(deviceResources, commandListManager));
	m_resourceManager->DefaultHeapResource(bufferDesc);
	m_resourceManager->UploadResource(bufferDesc);
	
	m_resourceManager->UpdateSubresource(data, size, size, size, state);
}

BufferManagerBase::~BufferManagerBase()
{

	m_resourceManager.release();
	m_resourceManager = nullptr;
}

