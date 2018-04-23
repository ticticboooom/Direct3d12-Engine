#pragma once
#include "stdafx.h"
#include "DeviceResources.h"
/**
 * @brief stores the shaders and renderer pipeline data
 *
 */
class D3DENGINE_API PSOManager
{
public:
	PSOManager(const std::shared_ptr<DX::DeviceResources> deviceResources);
	~PSOManager();
	void Finalise();
	ID3D12PipelineState* GetState() const { return m_pipelineState.Get(); };
	void SetVS(const CD3DX12_SHADER_BYTECODE shader) { m_desc.VS = shader; }; /// vertex shader
	void SetHS(const CD3DX12_SHADER_BYTECODE shader) { m_desc.HS = shader; }; /// hull shader
	void SetDS(const CD3DX12_SHADER_BYTECODE shader) { m_desc.DS = shader; }; /// domain shader
	void SetGS(const CD3DX12_SHADER_BYTECODE shader) { m_desc.GS = shader; }; /// geometry shader
	void SetPS(const CD3DX12_SHADER_BYTECODE shader) { m_desc.PS = shader; }; /// pixel shader
	void SetSignature(ID3D12RootSignature* signature) { m_desc.pRootSignature = signature; }; /// set root signature
	void SetPrimitiveTopologyType(const D3D12_PRIMITIVE_TOPOLOGY_TYPE type) { m_desc.PrimitiveTopologyType = type; }; /// set primitive topology
	void SetRTVFormats(const DXGI_FORMAT format, const int i) { m_desc.RTVFormats[i] = format; }; /// render target view format 
	void SetDSVFormat(const DXGI_FORMAT format) { m_desc.DSVFormat = format; }; /// depth stencvil view format

	void SetNumRenderTargets(const int count) { m_desc.NumRenderTargets = count; }; ///number of render targets
	void SetInputLayout(const D3D12_INPUT_LAYOUT_DESC layout) { m_desc.InputLayout = layout; };  /// input layout


private:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_desc; /// pso descriptor 
	Microsoft::WRL::ComPtr<ID3D12PipelineState>	m_pipelineState;/// the pso
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
};
