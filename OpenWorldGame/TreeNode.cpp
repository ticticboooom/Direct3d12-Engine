#include "TreeNode.h"
#include "SkeletalMeshComponent.h"
#include "PathManager.h"
#include "TerrainCollisionHelper.h"
#include "CylinderCollider.h"
TreeNode::TreeNode() : Node()
{
	auto pathManager = PathManager();
	auto mesh = std::make_shared<SkeletalMeshComponent>(pathManager.GetAssetPathStr() + std::string("Tree.coxl"));
	mesh->UseTexture(L"Tree.png");
	AddComponent(mesh);

	auto collider = std::make_shared<CylinderCollider>();
	collider->InitCollider(Structures::BoundingCylinder({ 0,0,0 }, 3, 5));
	AddComponent(collider);
}


TreeNode::~TreeNode()
{
}

int TreeNode::InitRootSignatureParameters(int indexOffset)
{
	return Node::InitRootSignatureParameters(indexOffset);
}

void TreeNode::Init()
{
	auto direction = XMVector3Normalize(XMVectorSet(rand(), 0, rand(), 0));
	auto pos = direction * ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 500.f);
	XMFLOAT3 pos3{};
	XMStoreFloat3(&pos3, pos);
	auto newY = 0.0f;
	auto terraionCollision = TerrainCollisionHelper().GetNewYPos(pos3, &newY);
	m_transform->position = XMVectorSetY(pos, newY);
	auto SF = 2.f;
	m_transform->scale = XMVectorSet(SF, SF, SF, SF);

	m_transform->rotationQuat = XMQuaternionRotationRollPitchYaw(0, ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (XM_PI * 2)), 0);
	Node::Init();
}

void TreeNode::Update()
{
	Node::Update();
}

void TreeNode::Render()
{
	Node::Render();
}

void TreeNode::OnKeyDown(UINT key)
{
	Node::OnKeyDown(key);
}

void TreeNode::OnKeyUp(UINT key)
{
	Node::OnKeyUp(key);
}

void TreeNode::OnMouseMoved(float x, float y)
{
	Node::OnMouseMoved(x, y);
}

void TreeNode::OnDeviceRemoved()
{
	Node::OnDeviceRemoved();
}

void TreeNode::CreateWindowSizeDependentResources()
{
	Node::CreateWindowSizeDependentResources();
}

void TreeNode::CreateDeviceDependentResources()
{
	Node::CreateDeviceDependentResources();
}
