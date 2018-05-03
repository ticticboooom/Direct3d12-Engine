#include "PlayerNode.h"



PlayerNode::PlayerNode() : Node(),
counter(0)
{
	//player
	auto pathManager = PathManager();
	auto playerMesh = std::make_shared<SkeletalMeshComponent>(pathManager.GetAssetPathStr() + std::string("Character.coxl"));
	playerMesh->UseTexture(L"Character.png");
	AddComponent(playerMesh);



	auto physicsComponent = std::make_shared<PhysicsComponent>();
	AddComponent(physicsComponent);

	auto boxCollider = std::make_shared<CylinderCollider>();
	AddComponent(boxCollider);
	auto playerCollider = Structures::BoundingCylinder({ 0,0,0 }, 1.2f, 3);
	boxCollider->InitCollider(playerCollider);

	auto movementComponent = std::make_shared<InputMovementComponent>();
	AddComponent(movementComponent);

	auto terrainCollisionComponent = std::make_shared<TerrainCollisionComponent>();
	AddComponent(terrainCollisionComponent);
}

PlayerNode::~PlayerNode()
{
}

int PlayerNode::InitRootSignatureParameters(int indexOffset)
{
	return Node::InitRootSignatureParameters(indexOffset);
}

void PlayerNode::Init()
{
	Node::Init();
}

void PlayerNode::Update()
{
	if (counter == 0) {
		counter = 100;
		auto meshComp = GetComponentManager()->GetComponent(typeid(SkeletalMeshComponent).name());
		auto skeletalMesh = (SkeletalMeshComponent*)meshComp.get();
		skeletalMesh->SetAnimInUse(animation);
		animation = !animation;
	}
	counter--;
	Node::Update();
}

void PlayerNode::Render()
{
	Node::Render();
}

void PlayerNode::OnKeyDown(UINT key)
{
	Node::OnKeyDown(key);
}

void PlayerNode::OnKeyUp(UINT key)
{
	Node::OnKeyUp(key);
}

void PlayerNode::OnMouseMoved(float x, float y)
{
	Node::OnMouseMoved(x, y);
}

void PlayerNode::OnDeviceRemoved()
{
	Node::OnDeviceRemoved();
}

void PlayerNode::CreateWindowSizeDependentResources()
{
	Node::CreateWindowSizeDependentResources();
}

void PlayerNode::CreateDeviceDependentResoures()
{
	Node::CreateDeviceDependentResoures();
}
