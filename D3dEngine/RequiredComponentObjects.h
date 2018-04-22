#pragma once
#include "stdafx.h"
#include <vector>
#include "RootSignatureManager.h"
#include "ModelViewProjectionManager.h"
namespace Structures {
	struct RequiredComponentObjects
	{
		RequiredComponentObjects(const std::shared_ptr<DX::DeviceResources>& device_resources,
			const std::shared_ptr<RootSignatureManager>& root_signature_manager)
			: deviceResources(device_resources),
			rootSignatureManager(root_signature_manager)
		{
		}

		const std::shared_ptr<DX::DeviceResources>& deviceResources;
		std::shared_ptr<RootSignatureManager> rootSignatureManager;
	};
}