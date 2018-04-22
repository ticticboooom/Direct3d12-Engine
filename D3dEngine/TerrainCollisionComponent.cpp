#include "stdafx.h"
#include "TerrainCollisionComponent.h"
#include "ComponentManager.h"
#include "TerrainComponent.h"
#include "TerrainCollisionHelper.h"
#include "PhysicsComponent.h"

/**
 * @brief Construct a new Terrain Collision Component:: Terrain Collision Component object
 * 	controls collisions with terrain
 * 
 */
TerrainCollisionComponent::TerrainCollisionComponent()
{
	m_terrainCollisionHelper = std::make_unique<TerrainCollisionHelper>();
}

/**
 * @brief Destroy the Terrain Collision Component:: Terrain Collision Component object
 * 
 */

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

/**
 * @brief run s the collision checks of the nodes transform and uses physics if the node has the component
 * 
 */
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
