#include "stdafx.h"
#include "BoxCollider.h"


BoxCollider::BoxCollider()
{
}


BoxCollider::~BoxCollider()
{
}

int BoxCollider::InitRootSignatureParameters(int indexOffset)
{
	return 0;
}

void BoxCollider::Init(std::shared_ptr<CommandListManager>* commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT * descOffset, std::shared_ptr<PSOManager>* pso)
{
}

void BoxCollider::Update()
{
}

void BoxCollider::Render()
{
}

void BoxCollider::OnKeyDown(UINT key)
{
}

void BoxCollider::OnKeyUp(UINT key)
{
}

void BoxCollider::OnMouseMoved(float x, float y)
{
}

void BoxCollider::OnDeviceRemoved()
{
}

void BoxCollider::CreateWindowSizeDependentResources()
{
}

void BoxCollider::CreateDeviceDependentResoures()
{
}

void BoxCollider::InitCollider(BoundingBox collider)
{
	m_collider = collider;
}
