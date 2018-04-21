#pragma once
#include "stdafx.h"
#include "DeviceResources.h"
#include "CommandListManager.h"
#include "ResourceManager.h"
/**
 * @brief A base class for common data buffers (vertex and index buffers) 
 * 
 */
class D3DENGINE_API BufferManagerBase
{
public:
	 BufferManagerBase(UINT size, BYTE* data, D3D12_RESOURCE_STATES state,
	     std::shared_ptr<DX::DeviceResources> deviceResources,
	     std::shared_ptr<CommandListManager> commandListManager);
	 ~BufferManagerBase();
	 ID3D12Resource* GetResource() const { return m_resourceManager->Get(); };

private:
	std::unique_ptr<ResourceManager> m_resourceManager; /// The resource that is managed
};
