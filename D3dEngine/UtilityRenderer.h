#pragma once
#include "Renderer.h"
#include "ConstantBufferManager.h"
class D3DENGINE_API UtilityRenderer : public Renderer
{
public:
	UtilityRenderer();
	~UtilityRenderer();

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
	static UINT m_rotRootSigIndex;
	static UINT m_rotHeapIndex;
protected:
	D3D12_RECT m_scissorRect;
	static bool m_isRootSignatureInitialised;
};

