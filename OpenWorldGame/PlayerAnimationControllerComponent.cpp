#include "PlayerAnimationControllerComponent.h"
#include "ComponentManager.h"
#include "LifeComponent.h"


PlayerAnimationControllerComponent::PlayerAnimationControllerComponent() : Component()
{
	m_prevIdleState = true;
	m_prevRunningState = false;
}


PlayerAnimationControllerComponent::~PlayerAnimationControllerComponent()
{
}

int PlayerAnimationControllerComponent::InitRootSignatureParameters(int indexOffset)
{
	return 0;
}

void PlayerAnimationControllerComponent::Init()
{
	ComponentManager* fullOwner = ComponentManager::GetOwner(owner);
	m_movementComp = std::dynamic_pointer_cast<InputMovementComponent>(fullOwner->GetComponent(typeid(InputMovementComponent).name()));
	m_meshComponent = std::dynamic_pointer_cast<SkeletalMeshComponent>(fullOwner->GetComponent(typeid(SkeletalMeshComponent).name()));
	m_AttackComponent = std::dynamic_pointer_cast<AttackComponent>(fullOwner->GetComponent(typeid(AttackComponent).name()));
}

void PlayerAnimationControllerComponent::Update()
{
	auto idleState = m_movementComp->GetIdleState();
	auto runningState = m_movementComp->GetRunningState();
	auto moveAnim = (runningState) ? 2 : 4;
	auto idleAnim = (idleState) ? 1 : moveAnim;
	if (m_prevIdleState != idleState && !m_AttackComponent->GetHittingState()) {
		if (idleState == true) {
			m_meshComponent->InterpFromTo(moveAnim, 1, 0.09f, 2);
		}
		else {
			m_meshComponent->InterpFromTo(1, moveAnim, 0.09f, 2);
		}
		m_prevIdleState = idleState;
	}

	if (m_prevRunningState != runningState && !idleState) {
		idleAnim = idleState ? 1 : 4;
		if (runningState) {
			m_meshComponent->InterpFromTo(idleAnim, 2, 0.09f, 2);
		}
		else {
			m_meshComponent->InterpFromTo(2, idleAnim, 0.09f, 2);
		}
		m_prevRunningState = runningState;
	}
}

void PlayerAnimationControllerComponent::Render()
{
}

void PlayerAnimationControllerComponent::OnKeyDown(UINT key)
{
}

void PlayerAnimationControllerComponent::OnKeyUp(UINT key)
{
}

void PlayerAnimationControllerComponent::OnMouseMoved(float x, float y)
{
}

void PlayerAnimationControllerComponent::OnDeviceRemoved()
{
}

void PlayerAnimationControllerComponent::CreateWindowSizeDependentResources()
{
}

void PlayerAnimationControllerComponent::CreateDeviceDependentResoures()
{
}
