#pragma once
#include "RootSignatureManager.h"
#include "DeviceResources.h"
#include "DescriptorHeapManager.h"
#include "PSOManager.h"

class D3DENGINE_API CommonObjects
{
public:
	static std::shared_ptr<DX::DeviceResources> m_deviceResources;
	static std::shared_ptr<RootSignatureManager> m_rootSignatureManager;
	static std::shared_ptr<DescriptorHeapManager> m_descriptorHeapManager;
	static UINT m_descriptorHeapIndexOffset;
	static std::shared_ptr<CommandListManager> m_commandListManager;
	static std::shared_ptr<PSOManager> m_psoManager;
	static void Reset() {
		m_rootSignatureManager.reset();
		m_descriptorHeapManager.reset();
		m_descriptorHeapIndexOffset = 0;
		m_commandListManager.reset();
		m_psoManager.reset();
	}
};

