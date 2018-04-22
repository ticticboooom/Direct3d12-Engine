#pragma once
#include "RootSignatureManager.h"
#include "DeviceResources.h"
class D3DENGINE_API CommonObjects
{
public:
	static std::shared_ptr<DX::DeviceResources> m_deviceResources;
	static std::shared_ptr<RootSignatureManager> m_rootSignatureManager;
};

