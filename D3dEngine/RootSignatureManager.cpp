#include "stdafx.h"
#include "RootSignatureManager.h"
#include "DirectXHelper.h"
#include "DeviceResources.h"

RootSignature::RootSignature(const std::shared_ptr<DX::DeviceResources> deviceResources):
m_deviceResources(deviceResources)
{
}

RootSignature::~RootSignature()
{
}

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

RootSignatureManager::RootSignatureManager()
{

}

RootSignatureManager::~RootSignatureManager()
{
}

void RootSignatureManager::AddSignature(const std::shared_ptr<DX::DeviceResources> deviceResources, UINT NumRootParams,
	UINT NumStaticSamplers)
{
	RootSignature* signature = new RootSignature(deviceResources);

	signature->Reset(NumRootParams, NumStaticSamplers);
	m_Signatures.push_back(signature);
}
