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

	auto cylinderCollider = std::make_shared<CylinderCollider>();
	AddComponent(cylinderCollider);
	auto playerCylinderCollider = Structures::BoundingCylinder({ 0,0,0 }, 3.f, 3);
	cylinderCollider->InitCollider(playerCylinderCollider);

	auto boxCollider = std::make_shared<BoxCollider>();
	AddComponent(boxCollider);
	auto playerCollider = BoundingBox({ 0,0,0 }, { 0,5.f,0 });
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
	counter++;
	auto idleState = m_movementComp->GetIdleState();
	auto meshComp = GetComponentManager()->GetComponent(typeid(SkeletalMeshComponent).name());
	auto skeletalMesh = std::dynamic_pointer_cast<SkeletalMeshComponent>(meshComp);
	if (m_prevIdleState != idleState && !m_isHitting) {
		if (idleState == true) {
			skeletalMesh->InterpFromTo(4, 2, 0.09f, 2);
		}
		else {
			skeletalMesh->InterpFromTo(2, 4, 0.09f, 2);
		}
		m_prevIdleState = idleState;
	}
	auto anim = ((idleState == true) ? 2 : 4);
	auto frameCount = skeletalMesh->GetAnimFrameCount(1);
	if (counter >= frameCount && m_isHitting) {
		if (counter == frameCount) {
			skeletalMesh->InterpFromTo(1, anim, 0.25f, -1);
		}
		else if (counter == frameCount + 4) {
			m_isHitting = false;
			auto movementComp = GetComponentManager()->GetComponent(typeid(InputMovementComponent).name());
			auto inputMovement = std::dynamic_pointer_cast<InputMovementComponent>(movementComp);
			inputMovement->SetCanMove(true);
		}
	}
	if ((GetKeyState(VK_LBUTTON) & 0x100) != 0 && !m_isHitting) {
		skeletalMesh->InterpFromTo(anim, 1, 0.25f, -1);
		m_isHitting = true;
		counter = 0;
		auto movementComp = GetComponentManager()->GetComponent(typeid(InputMovementComponent).name());
		auto inputMovement = std::dynamic_pointer_cast<InputMovementComponent>(movementComp);
		inputMovement->SetCanMove(false);
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
