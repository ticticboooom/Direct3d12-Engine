#pragma once
#include "stdafx.h"
#include "IGameBase.h"
#include "RequiredComponentObjects.h"
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
	virtual void Init(std::shared_ptr<CommandListManager>* commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT * descOffset, std::shared_ptr<PSOManager>* pso) override = 0;
	virtual void Update() override = 0;
	virtual void Render() override = 0;
	virtual void OnKeyDown(UINT key) override = 0;
	virtual void OnKeyUp(UINT key) override = 0;
	virtual void OnMouseMoved(float x, float y) override = 0;
	virtual void OnDeviceRemoved() override = 0;
	virtual void CreateWindowSizeDependentResources() override = 0;
	virtual void CreateDeviceDependentResoures() override = 0;
	std::shared_ptr<Structures::Transform> m_transform;
	IGameBase* owner;
protected:
	std::shared_ptr<PSOManager> m_psoManager;
	std::shared_ptr<CommandListManager> m_commandListManager;
	std::shared_ptr<DescriptorHeapManager> m_cbvSrvHeapManager;
	UINT m_cbvDescriptorSize;
	UINT m_descHeapOffset;
	static XMFLOAT4X4 m_projection;
	static XMFLOAT4X4 m_view;

	std::vector<UINT> m_rootSignInds;
	std::vector<UINT> m_heapInds;
	bool hasSetRootSigInd;
};