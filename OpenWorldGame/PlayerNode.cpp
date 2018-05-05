#include "PlayerNode.h"



PlayerNode::PlayerNode() : Node(),
counter(0), animation(0), m_prevIdleState(true)
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

	m_movementComp = std::make_shared<InputMovementComponent>();
	AddComponent(m_movementComp);

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
	auto idleState = m_movementComp->GetIdleState();
	if (m_prevIdleState != idleState) {
		auto meshComp = GetComponentManager()->GetComponent(typeid(SkeletalMeshComponent).name());
		auto skeletalMesh = (SkeletalMeshComponent*)meshComp.get();
		if (idleState == true) {
			skeletalMesh->InterpFromTo(1, 0, 0.09f, 2);
		}
		else {
			skeletalMesh->InterpFromTo(0, 1, 0.09f, 2);
		}
		m_prevIdleState = idleState;
	}
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
