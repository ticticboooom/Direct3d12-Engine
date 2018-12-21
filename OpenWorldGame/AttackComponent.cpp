#include "AttackComponent.h"
#include "ComponentManager.h"
#include "LifeComponent.h"

AttackComponent::AttackComponent() : Component(),
counter(0), animation(0), m_isHitting(false)
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
	m_lifeComponent = std::dynamic_pointer_cast<LifeComponent>(fullOwner->GetComponent(typeid(LifeComponent).name()));
	m_meshComponent = std::dynamic_pointer_cast<SkeletalMeshComponent>(fullOwner->GetComponent(typeid(SkeletalMeshComponent).name()));
}

void AttackComponent::Update()
{
	counter++;
	auto idleState = m_movementComp->GetIdleState();
	auto runningState = m_movementComp->GetRunningState();
	auto moveAnim = (runningState) ? 2 : 4;
	auto idleAnim = (idleState) ? 1 : moveAnim;
	auto frameCount = m_meshComponent->GetAnimFrameCount(1);
	if (counter >= frameCount && m_isHitting) {
		if (counter == frameCount) {
			m_meshComponent->InterpFromTo(0, idleAnim, 0.1f, -1);
		}
		else if (counter == frameCount + 10) {
			m_isHitting = false;
			m_movementComp->SetCanMove(true);
		}
	}

	if ((GetKeyState(VK_LBUTTON) & 0x100) != 0 && !m_isHitting) {
		m_meshComponent->InterpFromTo(idleAnim, 0, 0.25f, -1);
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

void AttackComponent::CreateDeviceDependentResources()
{
}

void AttackComponent::AttckOther()
{
	LifeComponent* closestLife = nullptr;
	XMVECTOR closestPos{};

	BoundingOrientedBox hitArea = BoundingOrientedBox({ 0,0,0 }, { 0, 0, c_attackDistance }, { 0,0,0,0 });
	XMStoreFloat3(&hitArea.Center, m_transform->position - XMVector3Rotate(XMVectorSet(0, 0, c_attackDistance, 0), m_transform->rotationQuat));
	XMStoreFloat4(&hitArea.Orientation, m_transform->rotationQuat);
	
	BoundingBox them = BoundingBox({ 0,0,0 }, { 5,5,5 });
	for (auto& life : LifeComponent::m_lives) {
		if (m_lifeComponent->GetLifeIndex() == life->GetLifeIndex()) {
			continue;
		}
		XMStoreFloat3(&them.Center, life->m_transform->position);

		if (hitArea.Intersects(them)) {
			closestPos = life->m_transform->position;
			closestLife = life;
		}
	}
	if (closestLife != nullptr) {
		closestLife->SetLife(closestLife->GetLife() - c_attckDamage);
	}
}
