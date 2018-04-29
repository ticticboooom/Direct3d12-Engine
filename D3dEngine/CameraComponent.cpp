#include "stdafx.h"
#include "CameraComponent.h"
#include "ComponentManager.h"
#include "PhysicsComponent.h"

/**
 * @brief Construct a new Camera Component:: Camera Component object
 * Controls the camera movement with keypresses and mouse movement also orbits around the node position
 */
CameraComponent::CameraComponent(std::shared_ptr<InputMovementComponent> movement) :
m_isUnderGround(false)
{
	m_movementComponent = movement;
}

/**
 * @brief Destroy the Camera Component:: Camera Component object
 * 
 */
CameraComponent::~CameraComponent()
{
}

int CameraComponent::InitRootSignatureParameters(int indexOffset)
{
	return 0;
}

void CameraComponent::Init()
{
}
/**
 * @brief calculates the matrices required to move the camera
 * 
 */
void CameraComponent::Update()
{
	auto target = m_movementComponent->GetPos();
	auto pitch = m_movementComponent->GetPitch();
	auto yaw = m_movementComponent->GetYaw();
	DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
	XMFLOAT3 fullNewPosFloat3;
	// Creates the multiplyer / offset for the camera orbitting the player.
	const auto camMultiplyer = 10;
	// Orbits the camera around the player (research sin waves and cos waves to understand more)
	XMStoreFloat3(&fullNewPosFloat3, XMVectorSet(sin(yaw) * camMultiplyer, sin(pitch) * camMultiplyer, cos(yaw) * camMultiplyer, 1) + target);


	// sets the \var newYPos if it is above terrain and the camera is not (\property c_cameraGroundOffset is the constant offset of how much the camera should be above ground)
	if (m_minY > fullNewPosFloat3.y - c_cameraGroundOffset) {
		fullNewPosFloat3.y = m_minY + c_cameraGroundOffset;
		m_movementComponent->SetRotationAbilities(false, true);
	}
	else {
		m_movementComponent->SetRotationAbilities(true, true);
	}
	m_transform->rotationQuat = XMQuaternionRotationRollPitchYaw(0, yaw, 0);
	// Puts the new pos (after orbitting and terrain collision) in to an XMVECTOR structure
	auto newPos = XMLoadFloat3(&fullNewPosFloat3);

	XMStoreFloat4x4(&m_view, XMMatrixTranspose(XMMatrixLookAtRH(newPos, target, up.v)));
	m_transform->position = newPos;
}

void CameraComponent::Render()
{
}

void CameraComponent::OnKeyDown(UINT key)
{
	
}

void CameraComponent::OnKeyUp(UINT key)
{
	
}
void CameraComponent::OnMouseMoved(float x, float y)
{

}

void CameraComponent::OnDeviceRemoved()
{
}

void CameraComponent::CreateWindowSizeDependentResources()
{
}

void CameraComponent::CreateDeviceDependentResoures()
{
}
