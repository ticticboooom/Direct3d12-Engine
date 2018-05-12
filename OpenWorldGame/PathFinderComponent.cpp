#include "PathFinderComponent.h"
#include "ComponentManager.h"

Structures::Transform PathFinderComponent::s_playerTransform;

PathFinderComponent::PathFinderComponent() : Component(),
m_currentPointIndex(0)
{
}


PathFinderComponent::~PathFinderComponent()
{
}

int PathFinderComponent::InitRootSignatureParameters(int indexOffset)
{
	return indexOffset;
}

void PathFinderComponent::Init()
{
	InitPoints();
	auto startPointIndex = (rand() / RAND_MAX) * (m_points.size());
	m_transform->position = m_points[startPointIndex];
	auto fullOwner = ComponentManager::GetOwner(owner);
	m_enemyAttackComponent = std::dynamic_pointer_cast<EnemyAttackComponent>(fullOwner->GetComponent(typeid(EnemyAttackComponent).name()));
}

void PathFinderComponent::Update()
{
	Move();
}

void PathFinderComponent::Render()
{
}

void PathFinderComponent::OnKeyDown(UINT key)
{
}

void PathFinderComponent::OnKeyUp(UINT key)
{

}

void PathFinderComponent::OnMouseMoved(float x, float y)
{
}

void PathFinderComponent::OnDeviceRemoved()
{
}

void PathFinderComponent::CreateWindowSizeDependentResources()
{
}

void PathFinderComponent::CreateDeviceDependentResoures()
{
}

void PathFinderComponent::Move()
{
	auto positionToGoTo = XMVECTOR{};
	auto distanceToPlayer = XMVectorGetX(XMVector3Length(s_playerTransform.position - m_transform->position));
	auto distanceToApproach = c_distanceFromPlayer;

	if (m_isPlayerClocked) {
		distanceToApproach = c_distanceFromPlayerWhenClocked;
	}

	if (distanceToPlayer < distanceToApproach) {
		positionToGoTo = s_playerTransform.position;
		m_isPlayerClocked = true;
	}
	else {
		if (m_isPlayerClocked) {
			m_isPlayerClocked = false;
		}
		positionToGoTo = m_points[m_currentPointIndex];
	}

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


	auto rotMat = XMMatrixTranspose(XMMatrixLookAtLH(m_transform->position, positionToGoTo, { 0,1, 0,0 }));
	auto scale = XMVECTOR{};
	auto rot = XMVECTOR{};
	auto pos = XMVECTOR{};
	XMMatrixDecompose(&scale, &rot, &pos, rotMat);
	auto zForward = XMVectorSet(0, 0, c_multiplyer, 0);
	auto movement = XMVector3Rotate(zForward, rot);
	auto newPos = m_transform->position + movement;
	m_transform->position = newPos;

	{
		auto newRot = XMQuaternionMultiply(rot, XMQuaternionRotationRollPitchYaw(0, XM_PI / 2, 0));
		float angle;
		XMVECTOR axis;
		XMQuaternionToAxisAngle(&axis, &angle, newRot);

		auto newAxis = axis * XMVectorSet(0, 1, 0, 0);

		m_transform->rotationQuat = XMQuaternionRotationAxis(newAxis, angle);
	}
}

void PathFinderComponent::InitPoints()
{
	for (auto i = 0; i < 10; i++) {
		auto direction = XMVector3Normalize(XMVectorSet(rand(), 0, rand(), 0));
		auto pos = direction * ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 500.f);
		m_points.push_back(pos);
	}
}