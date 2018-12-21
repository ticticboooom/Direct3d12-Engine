#include "stdafx.h"
#include "TerrainCollisionComponent.h"
#include "ComponentManager.h"
#include "TerrainComponent.h"
#include "TerrainCollisionHelper.h"
#include "PhysicsComponent.h"
#include "BoxCollider.h"
#include "CameraComponent.h"
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

void TerrainCollisionComponent::Init()
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
	auto cameraComp = fullOwner->GetComponent(typeid(CameraComponent).name());
	if (physicsComp != nullptr) {
		auto physicsComponent = (PhysicsComponent*)physicsComp.get();
		physicsComponent->minY = newYPos;
	}
	else {
		auto colliderComp = fullOwner->GetComponent(typeid(BoxCollider).name());
		if (colliderComp != nullptr) {
			auto collider = ((BoxCollider*)colliderComp.get())->GetCollider();
			if (XMVectorGetY(m_transform->position) - collider.Extents.y < newYPos) {
				m_transform->position = XMVectorSetY(m_transform->position, newYPos + collider.Extents.y);
			}
		}
		else {
			if (XMVectorGetY(m_transform->position) < newYPos) {
				m_transform->position = XMVectorSetY(m_transform->position, newYPos);
			}
		}
	}
	if (cameraComp != nullptr) {
		auto camera = (CameraComponent*)cameraComp.get();
		camera->m_minY = newYPos;
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

void TerrainCollisionComponent::CreateDeviceDependentResources()
{
}
