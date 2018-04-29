#include "stdafx.h"
#include "ComponentManager.h"

/**
 * @brief Construct a new Component Manager:: Component Manager object
 * 	 calls all the required functions when they need to be called
 * @param passTransform 
 */
ComponentManager::ComponentManager(bool passTransform) : passTransform(passTransform)
{
	m_components = std::vector<std::shared_ptr<Component>>();
}
/**
 * @brief Destroy the Component Manager:: Component Manager object
 * 
 */
ComponentManager::~ComponentManager()
{
}

int ComponentManager::InitRootSignatureParameters(int indexOffset)
{
	for (auto& comp : m_components) {
		indexOffset = comp->InitRootSignatureParameters(indexOffset);
	}
	return indexOffset;
}

void ComponentManager::Init()
{
	for (auto& comp : m_components) {
		comp->Init();
	}
}

void ComponentManager::Update()
{
	for (auto& comp : m_components) {
		if (passTransform) {
			comp->m_transform = m_transform;
		}
		comp->Update();
	}
}

void ComponentManager::Render()
{
	for (auto& comp : m_components) {
		comp->Render();
	}
}

void ComponentManager::OnKeyDown(UINT key)
{
	for (auto& comp : m_components) {
		comp->OnKeyDown(key);
	}
}

void ComponentManager::OnKeyUp(UINT key)
{
	for (auto& comp : m_components) {
		comp->OnKeyUp(key);
	}
}

void ComponentManager::OnMouseMoved(float x, float y)
{
	for (auto& comp : m_components) {
		comp->OnMouseMoved(x, y);
	}
}

void ComponentManager::OnDeviceRemoved()
{
	for (auto& comp : m_components) {
		comp->OnDeviceRemoved();
	}
}

void ComponentManager::CreateWindowSizeDependentResources()
{
	for (auto& comp : m_components) {
		comp->CreateWindowSizeDependentResources();
	}
}

void ComponentManager::CreateDeviceDependentResoures()
{
	for (auto& comp : m_components) {
		comp->CreateDeviceDependentResoures();
	}
}
/**
 * @brief adds a component to the manager (component must inherit from Componment)
 * 
 * @param comp componment
 */
void ComponentManager::AddComponent(std::shared_ptr<Component> comp)
{
	comp->owner = this;
	m_components.push_back(comp);
}
/**
 * @brief gets gets the owner ComponentManager from  the IGameBase interface
 * 
 * @param ownerBase IGameBase interface instance
 * @return ComponentManager* the owner
 */
ComponentManager * ComponentManager::GetOwner(IGameBase * ownerBase)
{
	return dynamic_cast<ComponentManager*>(ownerBase);
}
/**
 * @brief gets a component that is stored in a manager
 * 
 * @param componentName the typeid() name of the component
 * @return std::shared_ptr<Component> the component found or nullptr if not found
 */
std::shared_ptr<Component> ComponentManager::GetComponent(std::string componentName)
{
	for (auto& comp : m_components) {
		if (typeid(*comp.get()).name() == componentName) {
			return comp;
		}
	}
	return nullptr;
}
