#include "EnemyNode.h"

EnemyNode::EnemyNode() : Node()
{
	auto pathManager = PathManager();
	auto skeletalMeshComponent = std::make_shared<SkeletalMeshComponent>(pathManager.GetAssetPathStr() + std::string("Character.coxl"));
	skeletalMeshComponent->UseTexture(L"Character.png");
	AddComponent(skeletalMeshComponent);


	auto physicsComponent1 = std::make_shared<PhysicsComponent>();
	AddComponent(physicsComponent1);


	auto terrainCollisionComponent1 = std::make_shared<TerrainCollisionComponent>();
	AddComponent(terrainCollisionComponent1);

	auto playerCollider = Structures::BoundingCylinder({ 0,0,0 }, 1.2f, 3);
	auto boxCollider1 = std::make_shared<CylinderCollider>();

	AddComponent(boxCollider1);
	boxCollider1->InitCollider(playerCollider);
}


EnemyNode::~EnemyNode()
{
}

int EnemyNode::InitRootSignatureParameters(int indexOffset)
{
	return Node::InitRootSignatureParameters(indexOffset);
}

void EnemyNode::Init()
{
	Node::Init();
}

void EnemyNode::Update()
{
	Node::Update();
}

void EnemyNode::Render()
{
	Node::Render();
}

void EnemyNode::OnKeyDown(UINT key)
{
	Node::OnKeyDown(key);
}

void EnemyNode::OnKeyUp(UINT key)
{
	Node::OnKeyUp(key);
}

void EnemyNode::OnMouseMoved(float x, float y)
{
	Node::OnMouseMoved(x, y);
}

void EnemyNode::OnDeviceRemoved()
{
	Node::OnDeviceRemoved();
}

void EnemyNode::CreateWindowSizeDependentResources()
{
	Node::CreateWindowSizeDependentResources();
}

void EnemyNode::CreateDeviceDependentResoures()
{
	Node::CreateDeviceDependentResoures();
}
