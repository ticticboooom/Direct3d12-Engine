#pragma once
#include "Component.h"
#include "ComponentManager.h"
#include "ConstantBufferManager.h"
class D3DENGINE_API Node : public Component
{
public:
	Node();
	~Node();

	virtual int InitRootSignatureParameters(int indexOffset) override;
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void OnKeyDown(UINT key) override;
	virtual void OnKeyUp(UINT key) override;
	virtual void OnMouseMoved(float x, float y) override;
	virtual void OnDeviceRemoved() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void CreateDeviceDependentResoures() override;
	void AddComponent(std::shared_ptr<Component> comp);
protected:
	ComponentManager m_compManager; /// The manager of the copmponents used by the node
	std::shared_ptr<ModelViewProjectionManager> m_mvpManager; 
	std::unique_ptr<ConstantBufferManager<Structures::ModelViewProjectionConstantBuffer>> m_mvpConstantBufferManager;
	static UINT m_mvpRootSigIndex; /// rot signature index
	UINT m_mvpDescHeapIndex; /// heap index
private:
	static bool m_isRootSignatureInitialised; 
};

