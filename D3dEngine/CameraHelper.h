#pragma once
 
#include "stdafx.h"
#include "Structures.h"
#include "ModelViewProjectionManager.h"
#include "TerrainCollisionHelper.h"
class CameraHelper
{
public:
	D3DENGINE_API CameraHelper(std::shared_ptr<DX::DeviceResources> deviceResources);
	D3DENGINE_API ~CameraHelper();
	D3DENGINE_API void OnKeyDown(UINT key);
	D3DENGINE_API void OnKeyUp(UINT key);
	D3DENGINE_API void Update(XMVECTOR* pos);
	D3DENGINE_API void OnMouseMoved(float x, float y);
	D3DENGINE_API bool GetIsJumping() const { return isJumping; };
	D3DENGINE_API void OnWindowSizeChanged();
	D3DENGINE_API D3D12_RECT GetScissorRect() const { return m_scissorRect; };
	D3DENGINE_API XMVECTOR GetPosition() const { return m_position; };
	D3DENGINE_API XMVECTOR GetCameraPosition() const { return m_camTarget; };
	D3DENGINE_API float GetCameraRotYaw() const { return m_yaw; };
	D3DENGINE_API void SetPlayerPos(XMVECTOR pos) { m_playerPos = pos; };
	D3DENGINE_API Structures::ModelViewProjectionConstantBuffer GetCbvData();
private:
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	std::shared_ptr<ModelViewProjectionManager> m_mvpManager;
	std::unique_ptr<TerrainCollisionHelper> m_terrainCollider;
	float m_pitch;
	float m_yaw;
	DirectX::XMVECTORF32 m_direction;
	DirectX::XMVECTORF32 m_position;
	DirectX::XMMATRIX m_camRotationMatrix;
	DirectX::XMVECTOR DefaultForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR DefaultRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR m_camTarget;
	XMVECTOR m_playerPos;
	D3D12_RECT m_scissorRect;
	bool isJumping;
	float camLerpF;
	const float c_cameraGroundOffset = 2;
	bool m_isUnderGround;

};

