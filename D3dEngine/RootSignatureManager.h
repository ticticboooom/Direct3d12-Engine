#pragma once
#include "stdafx.h"
#include "DeviceResources.h"
#include <memory>
#include <vector>
/**
 * @brief a signature for what resources will be rendered
 *
 */
class D3DENGINE_API RootParameter
{
	friend class RootSignature;
public:


	/**
	 * @brief Construct a new Root Parameter object
	 *
	 */
	RootParameter()
	{
		m_RootParam = std::make_unique<D3D12_ROOT_PARAMETER>();
		m_RootParam->ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
	}

	/**
	 * @brief Destroy the Root Parameter object
	 *
	 */
	~RootParameter()
	{
		Clear();
	}

	/**
	 * @brief clear the root parameter
	 *
	 */
	void Clear()
	{
		if (m_RootParam->ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
			delete[] m_RootParam->DescriptorTable.pDescriptorRanges;

		m_RootParam->ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
	}

	/**
	 * @brief init root param as a constant
	 *
	 * @param Register parameter register
	 * @param NumDwords number of dwords
	 * @param Visibility shader visiblity flags
	 */
	void InitAsConstants(UINT Register, UINT NumDwords, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		m_RootParam->ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		m_RootParam->ShaderVisibility = Visibility;
		m_RootParam->Constants.Num32BitValues = NumDwords;
		m_RootParam->Constants.ShaderRegister = Register;
		m_RootParam->Constants.RegisterSpace = 0;
	}

	/**
	 * @brief init as constant buffer
	 * @param Register parameter register
	 * @param Visibility shader visiblity flags
	 */
	void InitAsConstantBuffer(UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		m_RootParam->ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		m_RootParam->ShaderVisibility = Visibility;
		m_RootParam->Descriptor.ShaderRegister = Register;
		m_RootParam->Descriptor.RegisterSpace = 0;
	}

	/**
	 * @brief init as shader reosurce view
	 * @param Register parameter register
	 * @param Visibility shader visiblity flags
	 */
	void InitAsBufferSRV(UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		m_RootParam->ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
		m_RootParam->ShaderVisibility = Visibility;
		m_RootParam->Descriptor.ShaderRegister = Register;
		m_RootParam->Descriptor.RegisterSpace = 0;
	}
	/**
	 * @brief init as Unordered Acess View
	 * @param Register parameter register
	 * @param Visibility shader visiblity flags
	 */
	void InitAsBufferUAV(UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		m_RootParam->ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
		m_RootParam->ShaderVisibility = Visibility;
		m_RootParam->Descriptor.ShaderRegister = Register;
		m_RootParam->Descriptor.RegisterSpace = 0;
	}
	/**
	 * @brief
	 *
	 * @param Type the type of descriptor ran ge
	 * @param Register parameter register
	 * @param Count number in range
	 * @param Visibility shader visibility
	 */
	void InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE Type, UINT Register, UINT Count, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsDescriptorTable(1, Visibility);
		SetTableRange(0, Type, Register, Count);
	}

	/**
	 * @brief init as descriptor table
	 *
	 * @param RangeCount the count of the range
	 * @param Visibility shader visibility
	 */
	void InitAsDescriptorTable(UINT RangeCount, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		m_RootParam->ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		m_RootParam->ShaderVisibility = Visibility;
		m_RootParam->DescriptorTable.NumDescriptorRanges = RangeCount;
		m_RootParam->DescriptorTable.pDescriptorRanges = new D3D12_DESCRIPTOR_RANGE[RangeCount];
	}

	/**
	 * @brief set the range of the table
	 *
	 * @param RangeIndex index of range
	 * @param Type type of range
	 * @param Register param register
	 * @param Count of range
	 * @param Space the shader space
	 */
	void SetTableRange(UINT RangeIndex, D3D12_DESCRIPTOR_RANGE_TYPE Type, UINT Register, UINT Count, UINT Space = 0)
	{
		D3D12_DESCRIPTOR_RANGE* range = const_cast<D3D12_DESCRIPTOR_RANGE*>(m_RootParam->DescriptorTable.pDescriptorRanges + RangeIndex);
		range->RangeType = Type;
		range->NumDescriptors = Count;
		range->BaseShaderRegister = Register;
		range->RegisterSpace = Space;
		range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}

	const D3D12_ROOT_PARAMETER& operator() (void) const { return *m_RootParam; } /// get a root parameter


protected:

	std::unique_ptr<D3D12_ROOT_PARAMETER> m_RootParam; /// the root parameter
};
class D3DENGINE_API RootSignature
{
public:
	RootSignature(const std::shared_ptr<DX::DeviceResources> deviceResources);
	~RootSignature();

	static void DestroyAll(void);

	/**
	 * @brief reset teh root signature
	 *
	 * @param NumRootParams number of params
	 * @param NumStaticSamplers number of static samplers
	 */
	void Reset(UINT NumRootParams, UINT NumStaticSamplers = 0)
	{
		m_ParamArray = std::make_unique<std::vector<std::shared_ptr<RootParameter>>>(NumRootParams);
		m_SamplerArray = std::make_unique<std::vector<D3D12_STATIC_SAMPLER_DESC>>(NumStaticSamplers);
		m_NumSamplers = NumStaticSamplers;
		m_NumInitializedStaticSamplers = 0;
	}

	void InitStaticSampler(UINT Register, const D3D12_SAMPLER_DESC& NonStaticSamplerDesc,
		D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL);
	void AddNewParameter(std::shared_ptr<RootParameter> param);
	void Finalize(const std::wstring& name, D3D12_ROOT_SIGNATURE_FLAGS Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);

	ID3D12RootSignature* GetSignature() const { return m_Signature; } // get the actual root signature

   /**
	* @brief get the parameter at EntryIndex
	*
	* @param EntryIndex index of param
	*/
	std::shared_ptr<RootParameter>& operator[] (size_t EntryIndex)
	{
		return (*m_ParamArray)[EntryIndex];
	}

	/**
	 * @brief get the parameter at EntryIndex
	 *
	 * @param EntryIndex index of param
	 */
	const std::shared_ptr<RootParameter>& operator[] (size_t EntryIndex) const
	{
		return (*m_ParamArray)[EntryIndex];
	}
protected:
	BOOL m_Finalized; /// is finalised
	UINT m_NumParameters; /// number of params
	UINT m_NumSamplers; /// number of samplers
	UINT m_NumInitializedStaticSamplers; /// number of initialised static samplers
	std::unique_ptr<std::vector<std::shared_ptr<RootParameter>>> m_ParamArray; /// root params
	std::unique_ptr<std::vector<D3D12_STATIC_SAMPLER_DESC>> m_SamplerArray; /// static samplers
	ID3D12RootSignature* m_Signature; /// rot signature
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
};



class D3DENGINE_API RootSignatureManager
{
public:
	RootSignatureManager();
	~RootSignatureManager();
	void AddSignature(const std::shared_ptr<DX::DeviceResources> deviceResources, UINT NumRootParams, UINT NumStaticSamplers = 0);
	/**
	 * @brief get root signature at EntryIndex
	 *
	 * @param EntryIndex index of signature
	 */
	RootSignature*& operator[] (size_t EntryIndex)
	{
		return m_Signatures[EntryIndex];
	}
	/**
	 * @brief get count of signatures
	 */
	size_t GetSignatureCount() const { return m_Signatures.size(); };
private:
	std::vector<RootSignature*> m_Signatures = std::vector<RootSignature*>(); /// vector of root signatures
};