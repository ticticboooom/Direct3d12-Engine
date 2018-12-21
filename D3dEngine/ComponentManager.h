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
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void OnKeyDown(UINT key) override;
	virtual void OnKeyUp(UINT key) override;
	virtual void OnMouseMoved(float x, float y) override;
	virtual void OnDeviceRemoved() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void CreateDeviceDependentResources() override;
	void AddComponent(std::shared_ptr<Component> comp);
	std::shared_ptr<Structures::Transform> m_transform;
	static ComponentManager* GetOwner(IGameBase* ownerBase);
	IGameBase* owner;
	std::shared_ptr<Component> GetComponent(std::string componentName);
	std::shared_ptr<Component> GetComponentByPointer(Component* compPtr);
	void RemoveComponentByPointer(Component* compPtr);
private:
	bool passTransform; // whether the manage rneeds to pass on the transform
	std::vector<std::shared_ptr<Component>> m_components; /// the components for the manager to call
	std::vector<UINT> m_componentsToRemove;
};
