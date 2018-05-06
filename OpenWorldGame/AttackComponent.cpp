#include "AttackComponent.h"
#include "ComponentManager.h"
#include "LifeComponent.h"

AttackComponent::AttackComponent() : Component(),
counter(0), animation(0), m_prevIdleState(true)
{
}


AttackComponent::~AttackComponent()
{
}

int AttackComponent::InitRootSignatureParameters(int indexOffset)
{
	return 0;
}

void AttackComponent::Init()
{
	ComponentManager* fullOwner = ComponentManager::GetOwner(owner);
	m_movementComp = std::dynamic_pointer_cast<InputMovementComponent>(fullOwner->GetComponent(typeid(InputMovementComponent).name()));

	m_meshComponent = std::dynamic_pointer_cast<SkeletalMeshComponent>(fullOwner->GetComponent(typeid(SkeletalMeshComponent).name()));
}

void AttackComponent::Update()
{
	counter++;
	auto idleState = m_movementComp->GetIdleState();
	if (m_prevIdleState != idleState && !m_isHitting) {
		if (idleState == true) {
			m_meshComponent->InterpFromTo(4, 2, 0.09f, 2);
		}
		else {
			m_meshComponent->InterpFromTo(2, 4, 0.09f, 2);
		}
		m_prevIdleState = idleState;
	}

	auto anim = ((idleState == true) ? 2 : 4);
	auto frameCount = m_meshComponent->GetAnimFrameCount(1);
	if (counter >= frameCount && m_isHitting) {
		if (counter == frameCount) {
			m_meshComponent->InterpFromTo(1, anim, 0.25f, -1);
		}
		else if (counter == frameCount + 4) {
			m_isHitting = false;
			m_movementComp->SetCanMove(true);
		}
	}
	if ((GetKeyState(VK_LBUTTON) & 0x100) != 0 && !m_isHitting) {
		m_meshComponent->InterpFromTo(anim, 1, 0.25f, -1);
		m_isHitting = true;
		counter = 0;
		m_movementComp->SetCanMove(false);
		AttckOther();
	}
}

void AttackComponent::Render()
{
}

void AttackComponent::OnKeyDown(UINT key)
{
}

void AttackComponent::OnKeyUp(UINT key)
{
}

void AttackComponent::OnMouseMoved(float x, float y)
{
}

void AttackComponent::OnDeviceRemoved()
{
}

void AttackComponent::CreateWindowSizeDependentResources()
{
}

void AttackComponent::CreateDeviceDependentResoures()
{
}

void AttackComponent::AttckOther()
{
	LifeComponent* closestLife = nullptr;
	XMVECTOR closestPos{};
	for (auto& life : LifeComponent::m_lives) {
		if (XMVectorGetX(XMVector3Length(m_transform->position - life->m_transform->position)) < 30) {
			closestPos = life->m_transform->position;
			closestLife = life;
		}
	}
	if (closestLife != nullptr) {
		closestLife->SetLife(closestLife->GetLife() - 5);
	}
}
