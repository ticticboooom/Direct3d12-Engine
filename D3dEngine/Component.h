#pragma once
#include "stdafx.h"
#include "IGameBase.h"
#include "Structures.h"
#include "ModelViewProjectionManager.h"
/**
 * @brief the base class for anything that runs the required functions for the game engine
 * 
 */
class D3DENGINE_API Component : public IGameBase
{
public:

	Component();
	~Component();

	// Inherited via IGameBase
	virtual int InitRootSignatureParameters(int indexOffset) override = 0;
	virtual void Init() override = 0;
	virtual void Update() override = 0;
	virtual void Render() override = 0;
	virtual void OnKeyDown(UINT key) override = 0;
	virtual void OnKeyUp(UINT key) override = 0;
	virtual void OnMouseMoved(float x, float y) override = 0;
	virtual void OnDeviceRemoved() override = 0;
	virtual void CreateWindowSizeDependentResources() override = 0;
	virtual void CreateDeviceDependentResources() override = 0;
	std::shared_ptr<Structures::Transform> m_transform;
	IGameBase* owner;
	static UINT m_descriptorCount;
protected:
	UINT m_cbvDescriptorSize;
	static XMFLOAT4X4 m_projection;
	static XMFLOAT4X4 m_view;

	std::vector<UINT> m_rootSignInds;
	std::vector<UINT> m_heapInds;
	bool hasSetRootSigInd;
};