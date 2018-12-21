#pragma once
#include "Component.h"
#include "ComponentManager.h"
class D3DENGINE_API Renderer : public Component
{
public:
	Renderer();
	~Renderer();

	// Inherited via Component
	virtual int InitRootSignatureParameters(int indexOffset) override;
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void OnKeyDown(UINT key) override;
	virtual void OnKeyUp(UINT key) override;
	virtual void OnMouseMoved(float x, float y) override;
	virtual void OnDeviceRemoved() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void CreateDeviceDependentResources() override;

protected:
	ComponentManager m_nodeManager;
};

