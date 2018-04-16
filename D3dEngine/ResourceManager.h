#pragma once
#include <memory>
#include "DeviceResources.h"
#include "CommandListManager.h"
/**
 * @brief used to upload and store data onto the GPU
 * 
 */
class ResourceManager
{
public:
	D3DENGINE_API ResourceManager(std::shared_ptr<DX::DeviceResources> deviceResources, std::shared_ptr<CommandListManager> commandListManager);
	D3DENGINE_API ~ResourceManager();
	D3DENGINE_API void UpdateSubresource(BYTE* data, UINT size, const UINT width, const UINT height, D3D12_RESOURCE_STATES state) const;

	D3DENGINE_API UINT64 GetRequiredIntermediateSizeForResource() const;
	D3DENGINE_API void UploadResource(D3D12_RESOURCE_DESC desc);
	D3DENGINE_API void DefaultHeapResource(D3D12_RESOURCE_DESC desc);

	D3DENGINE_API ID3D12Resource* Get() const {
		if (m_hasDefaultHeap)
			return m_resource.Get();
		return m_resourceUpload.Get();
	}
private:


	std::shared_ptr<DX::DeviceResources> m_deviceResources; 
	std::shared_ptr<CommandListManager> m_commandListManager; 
	Microsoft::WRL::ComPtr<ID3D12Resource> m_resourceUpload; /// upload resource
	Microsoft::WRL::ComPtr<ID3D12Resource> m_resource; /// defualt resource 
	bool m_hasDefaultHeap = false; /// has default resource 
};
