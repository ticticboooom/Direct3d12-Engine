#pragma once
#include "stdafx.h"
#include <vector>
#include "RootSignatureManager.h"
#include "ModelViewProjectionManager.h"
#include "CameraHelper.h"
namespace Structures {
	struct RequiredRendererObjects
	{
		RequiredRendererObjects(const std::shared_ptr<DX::DeviceResources>& device_resources,
			const std::shared_ptr<RootSignatureManager>& root_signature_manager,
			std::shared_ptr<CameraHelper> camera_helper)
			: deviceResources(device_resources),
			rootSignatureManager(root_signature_manager),
			cameraHelper(camera_helper)
		{
		}

		const std::shared_ptr<DX::DeviceResources>& deviceResources;
		std::shared_ptr<RootSignatureManager> rootSignatureManager;
		std::shared_ptr<CameraHelper> cameraHelper;
	};
}