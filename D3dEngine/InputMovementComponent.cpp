#include "stdafx.h"
#include "InputMovementComponent.h"
#include "ComponentManager.h"
#include "PhysicsComponent.h"


InputMovementComponent::InputMovementComponent() :
	m_pitch(0),
	m_yaw(0),
	m_direction({}),
	m_canRotatePitch(true),
	m_canRotateYaw(true),
	isIdle(true),
	m_canMove(true)
{
}


InputMovementComponent::~InputMovementComponent()
{
}

int InputMovementComponent::InitRootSignatureParameters(int indexOffset)
{
	return 0;
}

void InputMovementComponent::Init()
{
}

void InputMovementComponent::Update()
{
	if (m_canMove) {

		if (XMVector3Equal(m_direction, XMVectorSet(0, 0, 0, 0))) {
			isIdle = true;
		}
		else {
			isIdle = false;
		}
		ComponentManager* fullOwner = ComponentManager::GetOwner(owner);
		auto physicsComp = fullOwner->GetComponent(typeid(PhysicsComponent).name());
		if (physicsComp) {
			auto collider = ((PhysicsComponent*)physicsComp.get())->isRising = isJumping;
		}
		// The vector that is facing up
		DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

		// Gets the yaw rotation matrix from \property m_yaw
		const auto RotateYTempMatrix = XMMatrixRotationY(m_yaw);

		// Rotates the \property DefaultRight vector with the yaw rotation matrix \var RotateYTempMatrix
		const auto camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
		// Rotates the \property DefaultForward vector with the yaw rotation matrix \var RotateYTempMatrix
		const auto camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);
		// Rotates the \var up vector with the yaw rotation matrix \var RotateYTempMatrix
		up.v = XMVector3TransformCoord(up, RotateYTempMatrix);

		// The float value to multiply the \property m_direction vector coords X and Z
		const auto multiplyerXZ = -0.3f;
		// The float value to multiply the \property m_direction vector coord Y
		const auto multiplyerY = 0.3f;

		// Multiplies the \property m_direction vector with the multiplyers \var multiplyerXZ and \var multiplyerY
		const auto x = XMVectorGetX(m_direction) * multiplyerXZ;
		const auto z = XMVectorGetZ(m_direction) * multiplyerXZ;
		const auto y = XMVectorGetY(m_direction) * multiplyerY;

		// Creates XMVECTOR from \var x and multiplies it by \var camRight to get the x movement of the camera
		const auto xAxisMovement = XMVectorMultiply(XMVectorSet(x, x, x, x), camRight);
		// Creates XMVECTOR from \var z and multiplies it by \var camForward to get the z movement of the camera
		const auto zAxisMovement = XMVectorMultiply(XMVectorSet(z, z, z, z), camForward);
		// Creates XMVECTOR from \var y and multiplies it by the \var up vector to get the y movement of the camera
		const auto yAxisMovement = XMVectorMultiply(XMVectorSet(y, y, y, y), up);

		// Add the movement vectors (\var xAxisMovement \var zAxisMovement \var yAxisMovement) to the position (\var pos) of the camera
		m_transform->position = XMVectorAdd(m_transform->position, xAxisMovement);
		m_transform->position = XMVectorAdd(m_transform->position, zAxisMovement);
		m_transform->position = XMVectorAdd(m_transform->position, yAxisMovement);
		m_position = m_transform->position;
		m_transform->rotationQuat = XMQuaternionRotationRollPitchYaw(0, m_yaw - XM_PI / 2, 0);
	}
}

void InputMovementComponent::Render()
{
}

void InputMovementComponent::OnKeyDown(UINT key)
{
	if (key == 0x57)
	{
		m_direction.v = XMVectorSetZ(m_direction, 1);
	}
	// Set \property m_direction to left on "A" pressed
	else if (key == 0x41)
	{
		m_direction.v = XMVectorSetX(m_direction, 1);
	}
	// Set \property m_direction to backward on "S" pressed
	else if (key == 0x53)
	{
		m_direction.v = XMVectorSetZ(m_direction, -1);
	}
	// Set \property m_direction to right on "D" pressed
	else if (key == 0x44)
	{
		m_direction.v = XMVectorSetX(m_direction, -1);
	}
	// Set \property m_direction to up on "SpaceBar" pressed
	else if (key == VK_SPACE)
	{
		isJumping = true;
		m_direction.v = XMVectorSetY(m_direction, 1);
	}
	// Set \property m_direction to down on "Shift" pressed
	else if (key == VK_SHIFT)
	{
		m_direction.v = XMVectorSetY(m_direction, -1);
	}
}

void InputMovementComponent::OnKeyUp(UINT key)
{
	// Set \property m_direction to forward on "W" pressed
	if (key == 0x57)
	{
		m_direction.v = XMVectorSetZ(m_direction, 0);
	}
	// Set \property m_direction to not left on "A" pressed
	else if (key == 0x41)
	{
		m_direction.v = XMVectorSetX(m_direction, 0);
	}
	// Set \property m_direction to not backward on "S" pressed
	else if (key == 0x53)
	{
		m_direction.v = XMVectorSetZ(m_direction, 0);
	}
	// Set \property m_direction to not right on "D" pressed
	else if (key == 0x44)
	{
		m_direction.v = XMVectorSetX(m_direction, 0);
	}
	// Set \property m_direction to not up on "SpaceBar" pressed
	else if (key == VK_SPACE)
	{
		isJumping = false;
		m_direction.v = XMVectorSetY(m_direction, 0);
	}
	// Set \property m_direction to not down on "Shift" pressed
	else if (key == VK_SHIFT)
	{
		m_direction.v = XMVectorSetY(m_direction, 0);
	}
}

void InputMovementComponent::OnMouseMoved(float x, float y)
{
	// Multiplyer of the relative \param x  \param y;
	const auto multiplyer = 0.003f;

	if ((y < 0 && !m_canRotatePitch) || m_canRotatePitch) {
		m_pitch -= y * multiplyer;
	}

	if (m_canRotateYaw) {
		m_yaw -= -x * multiplyer;
	}

	// Stops the rotations from going past half a circle
	m_pitch = (float)__max(-DirectX::XM_PI / 2.0f - 0.00003f, m_pitch);
	m_pitch = (float)__min(+DirectX::XM_PI / 2.0f - 0.00003f, m_pitch);
}

void InputMovementComponent::OnDeviceRemoved()
{
}

void InputMovementComponent::CreateWindowSizeDependentResources()
{
}

void InputMovementComponent::CreateDeviceDependentResoures()
{
}
