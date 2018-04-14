#pragma once
#include "stdafx.h"
#include "CommandListManager.h"
#include "DescriptorHeapManager.h"
#include "PSOManager.h"
class IGameBase
{
public:
	virtual ~IGameBase() = default;
	virtual int InitRootSignatureParameters(int indexOffset) = 0;
	virtual void RunCommandLists() = 0;
	virtual std::shared_ptr<CommandListManager> Init(std::shared_ptr<CommandListManager> commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT* descOffset, std::shared_ptr<PSOManager>* pso) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void OnKeyDown(UINT key) = 0;
	virtual void OnKeyUp(UINT key) = 0;
	virtual void OnMouseMoved(float x, float y) = 0;
	virtual void OnDeviceRemoved() = 0;
	virtual void CreateWindowSizeDependentResources() = 0;
private:
	virtual void CreateDeviceDependentResoures() = 0;
};

