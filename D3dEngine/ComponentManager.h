#pragma once
#include "IGameBase.h"
#include "Component.h"
class D3DENGINE_API ComponentManager : public IGameBase
{
public:
	ComponentManager(bool passTransform);
	~ComponentManager();
	
	// Inherited via IGameBase
	virtual int InitRootSignatureParameters(int indexOffset) override;
	virtual void Init(std::shared_ptr<CommandListManager>* commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT * descOffset, std::shared_ptr<PSOManager>* pso) override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void OnKeyDown(UINT key) override;
	virtual void OnKeyUp(UINT key) override;
	virtual void OnMouseMoved(float x, float y) override;
	virtual void OnDeviceRemoved() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void CreateDeviceDependentResoures() override;
	void AddComponent(std::shared_ptr<Component> comp);

	std::shared_ptr<Structures::Transform> m_transform;
	static ComponentManager* GetOwner(IGameBase* ownerBase);

	std::shared_ptr<Component> GetComponent(std::string componentName);
private:
	bool passTransform;
	std::vector<std::shared_ptr<Component>> m_components;
};
