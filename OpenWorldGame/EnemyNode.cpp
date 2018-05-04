#include "EnemyNode.h"

EnemyNode::EnemyNode() : Node(),
m_currentPointIndex(0)
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
	InitPoints();
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
	Move();
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

void EnemyNode::Move()
{
	auto diff = m_points[m_currentPointIndex] - XMVectorSetY(m_transform->position, 0);
	auto distance = XMVectorGetX(XMVector3Length(XMVectorSet(XMVectorGetX(diff), 0, XMVectorGetZ(diff), 0)));
	if (distance < 3) {
		auto newIndex = m_currentPointIndex + 1;
		if (newIndex >= m_points.size()) {
			newIndex = 0;
		}

		m_currentPointIndex = newIndex;
		m_intervalIndex = 0;
	}


	auto rotMat = XMMatrixTranspose(XMMatrixLookAtLH(m_transform->position, m_points[m_currentPointIndex], { 0,1, 0,0 }));
	auto scale = XMVECTOR{};
	auto rot = XMVECTOR{};
	auto pos = XMVECTOR{};
	XMMatrixDecompose(&scale, &rot, &pos, rotMat);
	auto zForward = XMVectorSet(0, 0, c_multiplyer, 0);
	auto movement = XMVector3Rotate(zForward, rot);
	auto newPos = m_transform->position + movement;
	m_transform->position = newPos;
}

void EnemyNode::InitPoints()
{
	for (auto i = 0; i < 10; i++) {
		auto direction = XMVector3Normalize(XMVectorSet(rand(), 0, rand(), 0));
		auto pos = direction * (((float)rand() / RAND_MAX) * 500);
		m_points.push_back(pos);
	}
}
