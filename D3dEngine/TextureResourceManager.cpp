#include "stdafx.h"
#include "TextureResourceManager.h"
#include "TextureLoader.h"
/**
 * @brief Construct a new Texture Resource Manager:: Texture Resource Manager object
 * 	Creates the resource for putting the texture onto the graphics card
 * @param fileName path and filename of the texture
 * @param deviceResources 
 * @param commandListManager 
 */
TextureResourceManager::TextureResourceManager(std::wstring fileName,
	std::shared_ptr<DX::DeviceResources> deviceResources, std::shared_ptr<CommandListManager> commandListManager) :
	m_deviceResources(deviceResources)
{
	//load texture
	const auto loadedData = LoadTextureFromFile(fileName);

	const auto resourceDesc = static_cast<CD3DX12_RESOURCE_DESC>(m_textureDesc);
	// create resource
	m_resourceManager = std::make_unique<ResourceManager>(m_deviceResources, commandListManager);
	m_resourceManager->DefaultHeapResource(m_textureDesc);
	// get the requires size
	UINT size = m_resourceManager->GetRequiredIntermediateSizeForResource();
	// upload the resource / texture
	m_resourceManager->UploadResource(CD3DX12_RESOURCE_DESC::Buffer(size));
	m_resourceManager->UpdateSubresource(loadedData.textureData, size, loadedData.imageBytesPerRow, m_textureDesc.Height, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	delete loadedData.textureData;
}

/**
 * @brief Destroy the Texture Resource Manager:: Texture Resource Manager object
 * 
 */
TextureResourceManager::~TextureResourceManager()
{
	m_textureDesc = {};
	m_resourceManager.release();
	m_resourceManager = nullptr;
}

/**
 * @brief create the shader resource view from the texture
 * 
 * @param srvHeap the descriptor heap which is used for the srv creation
 * @param srvHeapSize the heap size
 * @param descriptorSlot the slot of the texture resource
 */
void TextureResourceManager::CreateSRVFromTextureResource(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap, const UINT srvHeapSize,
	const int descriptorSlot) const
{
	auto d3dDevice = m_deviceResources->GetD3DDevice();
	//describe and create the srv
	auto srvDesc = D3D12_SHADER_RESOURCE_VIEW_DESC{};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = m_textureDesc.Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	d3dDevice->CreateShaderResourceView(m_resourceManager->Get(), &srvDesc, CD3DX12_CPU_DESCRIPTOR_HANDLE(srvHeap->GetCPUDescriptorHandleForHeapStart(), descriptorSlot, srvHeapSize));
}
/**
 * @brief load a texture from file and put data into a struct
 * 
 * @param fileName poath and filename of the texture
 * @return TextureResourceManager::LoadedData the data that is loaded from the texture
 */
TextureResourceManager::LoadedData TextureResourceManager::LoadTextureFromFile(std::wstring fileName)
{
	BYTE* textureData;
	TextureLoader* tl = new TextureLoader();
	int imageBytesPerRow;
	tl->LoadImageDataFromFile(&textureData, m_textureDesc, fileName.c_str(), imageBytesPerRow);
	delete tl;
	return { textureData, imageBytesPerRow };
}
