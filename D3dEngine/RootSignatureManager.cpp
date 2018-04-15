#include "stdafx.h"
#include "RootSignatureManager.h"
#include "DirectXHelper.h"
#include "DeviceResources.h"
/**
 * @brief Construct a new Root Signature:: Root Signature object
 * @detail this is a signature for what resources will be rendered
 * @param deviceResources 
 */
RootSignature::RootSignature(const std::shared_ptr<DX::DeviceResources> deviceResources):
m_deviceResources(deviceResources)
{
}
/**
 * @brief Destroy the Root Signature:: Root Signature object
 * 
 */
RootSignature::~RootSignature()
{
}

/**
 * @brief initialise static sampler from a nonstatic sampler
 * 
 * @param Register the register it will map to
 * @param NonStaticSamplerDesc the descriptor of the nmon static sampler
 * @param Visibility the shader visibility flags which decide which shaders it will be exposed to
 */
void RootSignature::InitStaticSampler(UINT Register, const D3D12_SAMPLER_DESC& NonStaticSamplerDesc,
	D3D12_SHADER_VISIBILITY Visibility)
{
	D3D12_STATIC_SAMPLER_DESC& StaticSamplerDesc = m_SamplerArray[m_NumInitializedStaticSamplers++];

	StaticSamplerDesc.Filter = NonStaticSamplerDesc.Filter;
	StaticSamplerDesc.AddressU = NonStaticSamplerDesc.AddressU;
	StaticSamplerDesc.AddressV = NonStaticSamplerDesc.AddressV;
	StaticSamplerDesc.AddressW = NonStaticSamplerDesc.AddressW;
	StaticSamplerDesc.MipLODBias = NonStaticSamplerDesc.MipLODBias;
	StaticSamplerDesc.MaxAnisotropy = NonStaticSamplerDesc.MaxAnisotropy;
	StaticSamplerDesc.ComparisonFunc = NonStaticSamplerDesc.ComparisonFunc;
	StaticSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
	StaticSamplerDesc.MinLOD = NonStaticSamplerDesc.MinLOD;
	StaticSamplerDesc.MaxLOD = NonStaticSamplerDesc.MaxLOD;
	StaticSamplerDesc.ShaderRegister = Register;
	StaticSamplerDesc.RegisterSpace = 0;
	StaticSamplerDesc.ShaderVisibility = Visibility;

}

/**
 * @brief finalise, serialise and create the root signature
 * 
 * @param name name of the rootsignature
 * @param Flags creation flags
 */
void RootSignature::Finalize(const std::wstring& name, D3D12_ROOT_SIGNATURE_FLAGS Flags)
{
	if (m_Finalized)
		return;
	
	CD3DX12_ROOT_SIGNATURE_DESC RootDesc;
	RootDesc.Init(m_NumParameters,
		(const D3D12_ROOT_PARAMETER*)m_ParamArray.get(),
		m_NumSamplers,
		(const D3D12_STATIC_SAMPLER_DESC*)m_SamplerArray.get(), Flags);
	Microsoft::WRL::ComPtr<ID3DBlob> pOutBlob, pErrorBlob;

	ThrowIfFailed(D3D12SerializeRootSignature(&RootDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		&pOutBlob, &pErrorBlob));

	ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateRootSignature(1, pOutBlob->GetBufferPointer(), pOutBlob->GetBufferSize(),
		IID_PPV_ARGS(&m_Signature)));

	m_Signature->SetName(name.c_str());

}

/**
 * @brief Construct a new Root Signature Manager:: Root Signature Manager object
 * @detail stores the root signature managers
 */
RootSignatureManager::RootSignatureManager()
{

}
/**
 * @brief Destroy the Root Signature Manager:: Root Signature Manager object
 * 
 */
RootSignatureManager::~RootSignatureManager()
{
}
/**
 * @brief add a root signature to the list
 * 
 * @param deviceResources 
 * @param NumRootParams number of root parameters
 * @param NumStaticSamplers number of static amplers
 */
void RootSignatureManager::AddSignature(const std::shared_ptr<DX::DeviceResources> deviceResources, UINT NumRootParams,
	UINT NumStaticSamplers)
{
	RootSignature* signature = new RootSignature(deviceResources);

	signature->Reset(NumRootParams, NumStaticSamplers);
	m_Signatures.push_back(signature);
}
