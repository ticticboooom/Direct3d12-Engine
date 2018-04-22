#include "stdafx.h"
#include "PhysicsComponent.h"
#include "ComponentManager.h"
#include "BoxCollider.h"

PhysicsComponent::PhysicsComponent() :
	minY(0),
	isRising(false)
{
}


PhysicsComponent::~PhysicsComponent()
{
}

int PhysicsComponent::InitRootSignatureParameters(int indexOffset)
{
	return 0;
}

void PhysicsComponent::Init(std::shared_ptr<CommandListManager>* commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT * descOffset, std::shared_ptr<PSOManager>* pso)
{
}

void PhysicsComponent::Update()
{
	ComponentManager* fullOwner = ComponentManager::GetOwner(owner);
	auto colliderComp = fullOwner->GetComponent(typeid(BoxCollider).name());
	if (colliderComp) {
		auto collider = ((BoxCollider*)colliderComp.get())->GetCollider();

		if (XMVectorGetY(m_transform->position) - collider.Extents.y >= minY && !isRising) {
			if (previouslyRising) {
				previouslyRising = false;
				yPos = XMVectorGetY(m_transform->position);
			}
			// fall down
			yPosFallDiff *= yPosFallMultiplyer;
			yPos -= yPosFallDiff;
			m_transform->position = XMVectorSetY(m_transform->position, yPos);
		}
		// is going up
		if (isRising) {
			yPos = XMVectorGetY(m_transform->position);
			yPosFallDiff = yPosFallStart;
			previouslyRising = true;
		}

		// is under minY
		if (XMVectorGetY(m_transform->position) - collider.Extents.y <= minY) {
			yPos = minY;
			//put on top of minY
			yPosFallDiff = yPosFallStart;
			m_transform->position = XMVectorSetY(m_transform->position, minY + collider.Extents.y);
		}
	}
	else {
		if (XMVectorGetY(m_transform->position) >= minY && !isRising) {
			if (previouslyRising) {
				previouslyRising = false;
				yPos = XMVectorGetY(m_transform->position);
			}
			// fall down
			yPosFallDiff *= yPosFallMultiplyer;
			yPos -= yPosFallDiff;
			m_transform->position = XMVectorSetY(m_transform->position, yPos);
		}
		// is going up
		if (isRising) {
			yPos = XMVectorGetY(m_transform->position);
			yPosFallDiff = yPosFallStart;
			previouslyRising = true;
		}

		// is under minY
		if (XMVectorGetY(m_transform->position) <= minY) {
			yPos = minY;
			//put on top of minY
			yPosFallDiff = yPosFallStart;
			m_transform->position = XMVectorSetY(m_transform->position, minY);
		}
	}
}

void PhysicsComponent::Render()
{
}

void PhysicsComponent::OnKeyDown(UINT key)
{
}

void PhysicsComponent::OnKeyUp(UINT key)
{
}

void PhysicsComponent::OnMouseMoved(float x, float y)
{
}

void PhysicsComponent::OnDeviceRemoved()
{
}

void PhysicsComponent::CreateWindowSizeDependentResources()
{
}

void PhysicsComponent::CreateDeviceDependentResoures()
{
}
