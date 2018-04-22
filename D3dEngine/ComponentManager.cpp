#include "stdafx.h"
#include "ComponentManager.h"


ComponentManager::ComponentManager(bool passTransform) : passTransform(passTransform)
{
	m_components = std::vector<std::shared_ptr<Component>>();
}

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

void ComponentManager::Init(std::shared_ptr<CommandListManager>* commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT * descOffset, std::shared_ptr<PSOManager>* pso)
{
	for (auto& comp : m_components) {
		comp->Init(commandListManager, descriptorHeapManager, descOffset, pso);
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

void ComponentManager::AddComponent(std::shared_ptr<Component> comp)
{
	m_components.push_back(comp);

}
