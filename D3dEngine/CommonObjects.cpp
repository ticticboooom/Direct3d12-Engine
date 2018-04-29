#include "stdafx.h"
#include "CommonObjects.h"

std::shared_ptr<DX::DeviceResources> CommonObjects::m_deviceResources = nullptr;
std::shared_ptr<RootSignatureManager> CommonObjects::m_rootSignatureManager = nullptr;
std::shared_ptr<DescriptorHeapManager> CommonObjects::m_descriptorHeapManager = nullptr;
UINT CommonObjects::m_descriptorHeapIndexOffset = 0u;
std::shared_ptr<CommandListManager> CommonObjects::m_commandListManager = nullptr;
std::shared_ptr<PSOManager> CommonObjects::m_psoManager = nullptr;

