#include "stdafx.h"
#include "CylinderCollider.h"

std::vector<Structures::BoundingCylinder> CylinderCollider::m_allColliders;
CylinderCollider::CylinderCollider() :
	m_colliderIndex(0)
{
}


CylinderCollider::~CylinderCollider()
{
}

int CylinderCollider::InitRootSignatureParameters(int indexOffset)
{
	return 0;
}

void CylinderCollider::Init()
{
}

void CylinderCollider::Update()
{
	XMStoreFloat3(&m_collider.Center, m_transform->position);

	m_allColliders[m_colliderIndex] = m_collider;
	Collide();

	XMStoreFloat3(&m_collider.Center, m_transform->position);
	m_allColliders[m_colliderIndex] = m_collider;
}

void CylinderCollider::Render()
{
}

void CylinderCollider::OnKeyDown(UINT key)
{
}

void CylinderCollider::OnKeyUp(UINT key)
{
}

void CylinderCollider::OnMouseMoved(float x, float y)
{
}

void CylinderCollider::OnDeviceRemoved()
{
}

void CylinderCollider::CreateWindowSizeDependentResources()
{
}

void CylinderCollider::CreateDeviceDependentResoures()
{
}

void CylinderCollider::InitCollider(Structures::BoundingCylinder collider)
{
	m_collider = collider;
	m_allColliders.push_back(collider);
	m_colliderIndex = m_allColliders.size() - 1;
}

void CylinderCollider::Collide()
{
	for (auto i = 0; i < m_allColliders.size(); i++) {
		if (i == m_colliderIndex) {
			continue;
		}
		if (m_allColliders[i].Intersects(m_collider)) {
			m_transform->position += GetPushBack(m_allColliders[i]);
		}
	}
}

XMVECTOR CylinderCollider::GetPushBack(Structures::BoundingCylinder coll)
{
	auto diff = XMLoadFloat3(&m_collider.Center) - XMLoadFloat3(&coll.Center);
	auto direction = XMVector3Normalize(diff);
	auto length = XMVectorGetX(XMVector3Length(XMVectorSetY(diff, 0)));
	auto push = XMVectorSet(0, 0, 0, 0);
	if (XMVector3Equal(direction, XMVectorSet(0, XMVectorGetY(direction), 0, 0))) {
		push = XMVectorSet(1, 0, 0, 0) * m_collider.radius;
	}
	else {
		push = direction * XMVectorSet(1, 0, 1, 1) * (m_collider.radius + coll.radius - length);
	}

	return push;
}
