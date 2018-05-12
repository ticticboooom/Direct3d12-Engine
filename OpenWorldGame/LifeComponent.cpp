#include "LifeComponent.h"
#include "ComponentManager.h"
#include "Node.h"
#include "Renderer.h"

std::vector<LifeComponent*> LifeComponent::m_lives;
LifeComponent::LifeComponent() : Component(),
 m_life(20)
{
	m_lives.push_back(this);
	m_lifeIndex = m_lives.size() - 1;
}

LifeComponent::~LifeComponent()
{
}

int LifeComponent::InitRootSignatureParameters(int indexOffset)
{
	return 0;
}

void LifeComponent::Init()
{
}

void LifeComponent::Update()
{
	if (m_life <= 0){
		auto node = dynamic_cast<Node*>(ComponentManager::GetOwner(owner)->owner);
		node->Destroy();
	}
}

void LifeComponent::Render()
{
}

void LifeComponent::OnKeyDown(UINT key)
{
}

void LifeComponent::OnKeyUp(UINT key)
{
}

void LifeComponent::OnMouseMoved(float x, float y)
{
}

void LifeComponent::OnDeviceRemoved()
{
}

void LifeComponent::CreateWindowSizeDependentResources()
{
}

void LifeComponent::CreateDeviceDependentResoures()
{
}
