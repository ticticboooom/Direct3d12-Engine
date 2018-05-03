#include "PlayerCameraNode.h"
#include "SkeletalMeshComponent.h"
#include "PathManager.h"
#include "CameraComponent.h"
#include "TerrainComponent.h"
#include "TerrainCollisionComponent.h"
#include "BoxCollider.h"
#include "PhysicsComponent.h"
#include "CylinderCollider.h"

PlayerCameraNode::PlayerCameraNode(std::shared_ptr<InputMovementComponent> movementComponent) : Node()
{
	auto camera = std::make_shared<CameraComponent>(movementComponent);
	AddComponent(camera);
	auto terrainCollisionComponentForCam = std::make_shared<TerrainCollisionComponent>();
	AddComponent(terrainCollisionComponentForCam);
}


PlayerCameraNode::~PlayerCameraNode()
{
}

int PlayerCameraNode::InitRootSignatureParameters(int indexOffset)
{
	return Node::InitRootSignatureParameters(indexOffset);
}

void PlayerCameraNode::Init()
{
	Node::Init();
}

void PlayerCameraNode::Update()
{
	Node::Update();
}

void PlayerCameraNode::Render()
{
	Node::Render();
}

void PlayerCameraNode::OnKeyDown(UINT key)
{
	Node::OnKeyDown(key);
}

void PlayerCameraNode::OnKeyUp(UINT key)
{
	Node::OnKeyUp(key);
}

void PlayerCameraNode::OnMouseMoved(float x, float y)
{
	Node::OnMouseMoved(x, y);
}

void PlayerCameraNode::OnDeviceRemoved()
{
	Node::OnDeviceRemoved();
}

void PlayerCameraNode::CreateWindowSizeDependentResources()
{
	Node::CreateWindowSizeDependentResources();
}

void PlayerCameraNode::CreateDeviceDependentResoures()
{
	Node::CreateDeviceDependentResoures();
}
