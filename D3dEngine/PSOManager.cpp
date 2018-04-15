#include "stdafx.h"
#include "PSOManager.h"
#include "DirectXHelper.h"
#include "DeviceResources.h"

/**
 * @brief Construct a new PSOManager::PSOManager object
 *  init with default values, this stores the shaders and renderer pipeline data
 * @param deviceResources 
 */
PSOManager::PSOManager(const std::shared_ptr<DX::DeviceResources> deviceResources) :
	m_deviceResources(deviceResources)
{
	m_desc = {};
	m_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	m_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	m_desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	m_desc.SampleMask = UINT_MAX;
	m_desc.SampleDesc.Count = 1;
}
/**
 * @brief Destroy the PSOManager::PSOManager object
 * 
 */
PSOManager::~PSOManager()
{
}
/**
 * @brief finalises and creates the PSO
 * 
 */
void PSOManager::Finalise()
{
	ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateGraphicsPipelineState(&m_desc, IID_PPV_ARGS(&m_pipelineState)));

}
