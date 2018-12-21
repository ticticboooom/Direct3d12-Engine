#pragma once
#include "stdafx.h"
#include "CommandListManager.h"
#include "DescriptorHeapManager.h"
#include "PSOManager.h"
class D3DENGINE_API IGameBase
{
public:
	virtual ~IGameBase() = default;
	virtual int InitRootSignatureParameters(int indexOffset) = 0;
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void OnKeyDown(UINT key) = 0;
	virtual void OnKeyUp(UINT key) = 0;
	virtual void OnMouseMoved(float x, float y) = 0;
	virtual void OnDeviceRemoved() = 0;
	virtual void CreateWindowSizeDependentResources() = 0;
private:
	virtual void CreateDeviceDependentResources() = 0;
};

