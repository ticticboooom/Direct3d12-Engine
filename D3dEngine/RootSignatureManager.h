#pragma once
#include "stdafx.h"
#include "DeviceResources.h"
#include <memory>
#include <vector>

class RootParameter
{
	friend class RootSignature;
public:

	D3DENGINE_API RootParameter()
	{
		m_RootParam.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
	}

	D3DENGINE_API ~RootParameter()
	{
		Clear();
	}

	D3DENGINE_API void Clear()
	{
		if (m_RootParam.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
			delete[] m_RootParam.DescriptorTable.pDescriptorRanges;

		m_RootParam.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
	}

	D3DENGINE_API void InitAsConstants(UINT Register, UINT NumDwords, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		m_RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		m_RootParam.ShaderVisibility = Visibility;
		m_RootParam.Constants.Num32BitValues = NumDwords;
		m_RootParam.Constants.ShaderRegister = Register;
		m_RootParam.Constants.RegisterSpace = 0;
	}

	D3DENGINE_API void InitAsConstantBuffer(UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		m_RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		m_RootParam.ShaderVisibility = Visibility;
		m_RootParam.Descriptor.ShaderRegister = Register;
		m_RootParam.Descriptor.RegisterSpace = 0;
	}

	D3DENGINE_API void InitAsBufferSRV(UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		m_RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
		m_RootParam.ShaderVisibility = Visibility;
		m_RootParam.Descriptor.ShaderRegister = Register;
		m_RootParam.Descriptor.RegisterSpace = 0;
	}

	D3DENGINE_API void InitAsBufferUAV(UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		m_RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
		m_RootParam.ShaderVisibility = Visibility;
		m_RootParam.Descriptor.ShaderRegister = Register;
		m_RootParam.Descriptor.RegisterSpace = 0;
	}

	D3DENGINE_API void InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE Type, UINT Register, UINT Count, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsDescriptorTable(1, Visibility);
		SetTableRange(0, Type, Register, Count);
	}

	D3DENGINE_API void InitAsDescriptorTable(UINT RangeCount, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		m_RootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		m_RootParam.ShaderVisibility = Visibility;
		m_RootParam.DescriptorTable.NumDescriptorRanges = RangeCount;
		m_RootParam.DescriptorTable.pDescriptorRanges = new D3D12_DESCRIPTOR_RANGE[RangeCount];
	}

	D3DENGINE_API void SetTableRange(UINT RangeIndex, D3D12_DESCRIPTOR_RANGE_TYPE Type, UINT Register, UINT Count, UINT Space = 0)
	{
		D3D12_DESCRIPTOR_RANGE* range = const_cast<D3D12_DESCRIPTOR_RANGE*>(m_RootParam.DescriptorTable.pDescriptorRanges + RangeIndex);
		range->RangeType = Type;
		range->NumDescriptors = Count;
		range->BaseShaderRegister = Register;
		range->RegisterSpace = Space;
		range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}

	D3DENGINE_API const D3D12_ROOT_PARAMETER& operator() (void) const { return m_RootParam; }


protected:

	D3D12_ROOT_PARAMETER m_RootParam;
};
class RootSignature
{
public:
	D3DENGINE_API RootSignature(const std::shared_ptr<DX::DeviceResources> deviceResources);
	D3DENGINE_API ~RootSignature();

	D3DENGINE_API static void DestroyAll(void);

	D3DENGINE_API void Reset(UINT NumRootParams, UINT NumStaticSamplers = 0)
	{
		if (NumRootParams > 0)
			m_ParamArray.reset(new RootParameter[NumRootParams]);
		else
			m_ParamArray = nullptr;
		m_NumParameters = NumRootParams;

		if (NumStaticSamplers > 0)
			m_SamplerArray.reset(new D3D12_STATIC_SAMPLER_DESC[NumStaticSamplers]);
		else
			m_SamplerArray = nullptr;
		m_NumSamplers = NumStaticSamplers;
		m_NumInitializedStaticSamplers = 0;
	}

	D3DENGINE_API void InitStaticSampler(UINT Register, const D3D12_SAMPLER_DESC& NonStaticSamplerDesc,
		D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL);

	D3DENGINE_API void Finalize(const std::wstring& name, D3D12_ROOT_SIGNATURE_FLAGS Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);

	D3DENGINE_API ID3D12RootSignature* GetSignature() const { return m_Signature; }

	D3DENGINE_API RootParameter& operator[] (size_t EntryIndex)
	{
		return m_ParamArray.get()[EntryIndex];
	}

	D3DENGINE_API const RootParameter& operator[] (size_t EntryIndex) const
	{
		return m_ParamArray.get()[EntryIndex];
	}
protected:
	BOOL m_Finalized;
	UINT m_NumParameters;
	UINT m_NumSamplers;
	UINT m_NumInitializedStaticSamplers;
	std::unique_ptr<RootParameter[]> m_ParamArray;
	std::unique_ptr<D3D12_STATIC_SAMPLER_DESC[]> m_SamplerArray;
	ID3D12RootSignature* m_Signature;
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
};



class RootSignatureManager
{
public:
	D3DENGINE_API RootSignatureManager();
	D3DENGINE_API ~RootSignatureManager();
	D3DENGINE_API void AddSignature(const std::shared_ptr<DX::DeviceResources> deviceResources, UINT NumRootParams, UINT NumStaticSamplers = 0);
	D3DENGINE_API RootSignature*& operator[] (size_t EntryIndex)
	{
		return m_Signatures[EntryIndex];
	}
	D3DENGINE_API size_t GetSignatureCount() const { return m_Signatures.size(); };
private:
	std::vector<RootSignature*> m_Signatures = std::vector<RootSignature*>();
};