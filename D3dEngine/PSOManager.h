#pragma once
#include "stdafx.h"
#include "DeviceResources.h"
/**
 * @brief stores the shaders and renderer pipeline data
 * 
 */
class PSOManager
{
public:
	D3DENGINE_API PSOManager(const std::shared_ptr<DX::DeviceResources> deviceResources);
	D3DENGINE_API ~PSOManager();
	D3DENGINE_API void Finalise();
	D3DENGINE_API ID3D12PipelineState* GetState() const { return m_pipelineState.Get(); };
	D3DENGINE_API void SetVS(const CD3DX12_SHADER_BYTECODE shader) { m_desc.VS = shader; }; /// vertex shader
	D3DENGINE_API void SetHS(const CD3DX12_SHADER_BYTECODE shader) { m_desc.HS = shader; }; /// hull shader
	D3DENGINE_API void SetDS(const CD3DX12_SHADER_BYTECODE shader) { m_desc.DS = shader; }; /// domain shader
	D3DENGINE_API void SetGS(const CD3DX12_SHADER_BYTECODE shader) { m_desc.GS = shader; }; /// geometry shader
	D3DENGINE_API void SetPS(const CD3DX12_SHADER_BYTECODE shader) { m_desc.PS = shader; }; /// pixel shader
	D3DENGINE_API void SetSignature(ID3D12RootSignature* signature) { m_desc.pRootSignature = signature; }; /// set root signature
	D3DENGINE_API void SetPrimitiveTopologyType(const D3D12_PRIMITIVE_TOPOLOGY_TYPE type) { m_desc.PrimitiveTopologyType = type; }; /// set primitive topology
	D3DENGINE_API void SetRTVFormats(const DXGI_FORMAT format, const int i) { m_desc.RTVFormats[i] = format; }; /// render target view format 
	D3DENGINE_API void SetDSVFormat(const DXGI_FORMAT format) { m_desc.DSVFormat = format; }; /// depth stencvil view format

	D3DENGINE_API void SetNumRenderTargets(const int count) { m_desc.NumRenderTargets = count; }; ///number of render targets
	D3DENGINE_API void SetInputLayout(const D3D12_INPUT_LAYOUT_DESC layout) { m_desc.InputLayout = layout; };  /// input layout


private:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_desc; /// pso descriptor 
	Microsoft::WRL::ComPtr<ID3D12PipelineState>	m_pipelineState;/// the pso
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
};
