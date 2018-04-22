#include "stdafx.h"
#include "TerrainCollisionComponent.h"
#include "ComponentManager.h"
#include "TerrainComponent.h"
#include "PhysicsComponent.h"


TerrainCollisionComponent::TerrainCollisionComponent()
{
	m_terrainCollisionHelper = std::make_unique<TerrainCollisionHelper>();
}


TerrainCollisionComponent::~TerrainCollisionComponent()
{
}

int TerrainCollisionComponent::InitRootSignatureParameters(int indexOffset)
{
	return indexOffset;
}

void TerrainCollisionComponent::Init(std::shared_ptr<CommandListManager>* commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT * descOffset, std::shared_ptr<PSOManager>* pso)
{
}

void TerrainCollisionComponent::Update()
{
	auto float3Pos = XMFLOAT3{};
	XMStoreFloat3(&float3Pos, m_transform->position);
	float newYPos = 0.f;
	m_terrainCollisionHelper->GetNewYPos(float3Pos, &newYPos);

	ComponentManager* fullOwner = ComponentManager::GetOwner(owner);
	auto physicsComp = fullOwner->GetComponent(typeid(PhysicsComponent).name());

	if (physicsComp) {
		auto physicsComponent = (PhysicsComponent*)physicsComp.get();
		physicsComponent->minY = newYPos;
	}
	else {
		if (XMVectorGetY(m_transform->position) < newYPos) {
			XMVectorSetY(m_transform->position, newYPos);
		}
	}
}

void TerrainCollisionComponent::Render()
{
}

void TerrainCollisionComponent::OnKeyDown(UINT key)
{
}

void TerrainCollisionComponent::OnKeyUp(UINT key)
{
}

void TerrainCollisionComponent::OnMouseMoved(float x, float y)
{
}

void TerrainCollisionComponent::OnDeviceRemoved()
{
}

void TerrainCollisionComponent::CreateWindowSizeDependentResources()
{
}

void TerrainCollisionComponent::CreateDeviceDependentResoures()
{
}
