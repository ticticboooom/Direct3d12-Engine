#include "stdafx.h"
#include "CameraHelper.h"



CameraHelper::CameraHelper(std::shared_ptr<DX::DeviceResources> deviceResources)
	: m_deviceResources(deviceResources),
	m_pitch(0),
	m_yaw(0),
	m_camTarget({}),
	m_position({ 0,0,0,0 }),
	m_direction({}),
	isJumping(false),
	m_isUnderGround(false)
{
	m_terrainCollider = std::make_unique<TerrainCollisionHelper>();
	m_mvpManager = std::make_shared<ModelViewProjectionManager>();
}


CameraHelper::~CameraHelper()
{
}

void CameraHelper::OnKeyDown(UINT key)
{
	if (key == 0x57)
	{
		m_direction.v = XMVectorSetZ(m_direction, 1);
	}
	else if (key == 0x41)
	{
		m_direction.v = XMVectorSetX(m_direction, 1);
	}
	else if (key == 0x53)
	{
		m_direction.v = XMVectorSetZ(m_direction, -1);
	}
	else if (key == 0x44)
	{
		m_direction.v = XMVectorSetX(m_direction, -1);
	}
	else if (key == VK_SPACE)
	{
		isJumping = true;
		m_direction.v = XMVectorSetY(m_direction, 1);
	}
	else if (key == VK_SHIFT)
	{
		m_direction.v = XMVectorSetY(m_direction, -1);
	}
}

void CameraHelper::OnKeyUp(UINT key)
{
	if (key == 0x57)
	{
		m_direction.v = XMVectorSetZ(m_direction, 0);
	}
	else if (key == 0x41)
	{
		m_direction.v = XMVectorSetX(m_direction, 0);
	}
	else if (key == 0x53)
	{
		m_direction.v = XMVectorSetZ(m_direction, 0);
	}
	else if (key == 0x44)
	{
		m_direction.v = XMVectorSetX(m_direction, 0);
	}
	else if (key == VK_SPACE)
	{
		isJumping = false;
		m_direction.v = XMVectorSetY(m_direction, 0);
	}
	else if (key == VK_SHIFT)
	{
		m_direction.v = XMVectorSetY(m_direction, 0);
	}
}

void CameraHelper::Update(XMVECTOR* pos)
{
	DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	m_camRotationMatrix = XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, 0);
	m_camTarget = XMVector3TransformCoord(DefaultForward, m_camRotationMatrix);
	m_camTarget = XMVector3Normalize(m_camTarget);
	const auto RotateYTempMatrix = XMMatrixRotationY(m_yaw);

	const auto camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	const auto camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);
	up.v = XMVector3TransformCoord(up, RotateYTempMatrix);

	const auto multiplyerXZ = -0.3f;
	const auto multiplyerY = 0.3f;

	const auto x = XMVectorGetX(m_direction) * multiplyerXZ;
	const auto z = XMVectorGetZ(m_direction) * multiplyerXZ;
	const auto y = XMVectorGetY(m_direction) * multiplyerY;

	const auto xAxisMovement = XMVectorMultiply(XMVectorSet(x, x, x, x), camRight);
	const auto zAxisMovement = XMVectorMultiply(XMVectorSet(z, z, z, z), camForward);
	const auto yAxisMovement = XMVectorMultiply(XMVectorSet(y, y, y, y), up);

	(*pos) = XMVectorAdd(*pos, xAxisMovement);
	(*pos) = XMVectorAdd(*pos, zAxisMovement);
	(*pos) = XMVectorAdd(*pos, yAxisMovement);

	XMFLOAT3 fullNewPosFloat3;
	const auto camMultiplyer = 10;
	XMStoreFloat3(&fullNewPosFloat3, XMVectorSet(sin(m_yaw) * camMultiplyer, sin(m_pitch) * camMultiplyer, cos(m_yaw) * camMultiplyer, 1) + (*pos));
	float newYPos;

	m_terrainCollider->GetNewYPos(fullNewPosFloat3, &newYPos);

	if (newYPos > fullNewPosFloat3.y - c_cameraGroundOffset) {
		fullNewPosFloat3.y = newYPos + c_cameraGroundOffset;
		m_isUnderGround = true;
	}
	else {
		m_isUnderGround = false;
	}

	auto newPos = XMLoadFloat3(&fullNewPosFloat3);
	m_mvpManager->SetMatrix(1, XMMatrixTranspose(XMMatrixLookAtRH(newPos, *pos, up.v)));
	m_mvpManager->SetMatrix(0, XMMatrixIdentity());
}

void CameraHelper::OnMouseMoved(float x, float y)
{
	const auto multiplyer = 0.003f;
	if ((y < 0 && m_isUnderGround) || !m_isUnderGround) {
		m_pitch -= y * multiplyer;
	}

	m_yaw -= -x * multiplyer;
	m_pitch = (float)__max(-DirectX::XM_PI / 2.0f - 0.00003f, m_pitch);
	m_pitch = (float)__min(+DirectX::XM_PI / 2.0f - 0.00003f, m_pitch);
}

void CameraHelper::OnWindowSizeChanged()
{
	const auto outputSize = m_deviceResources->GetOutputSize();
	const float aspectRatio = outputSize.x / outputSize.y;
	float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;

	const auto viewport = m_deviceResources->GetScreenViewport();
	m_scissorRect = { 0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height) };
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}
	const auto perspectiveMatrix = DirectX::XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		10000.0f
	);

	static const DirectX::XMFLOAT4X4 Rotation0(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	const auto orientationMatrix = XMLoadFloat4x4(&Rotation0);

	m_mvpManager->SetMatrix(2, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));
}

Structures::ModelViewProjectionConstantBuffer CameraHelper::GetCbvData()
{
	return m_mvpManager->GetCbvData();
}
