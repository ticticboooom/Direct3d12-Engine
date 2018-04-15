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

	D3DENGINE_API Structures::ModelViewProjectionConstantBuffer GetCbvData();
private:
	std::shared_ptr<DX::DeviceResources> m_deviceResources; /// The D3d Device manager class
	std::shared_ptr<ModelViewProjectionManager> m_mvpManager; /// The Model View Perspective matrices manager
	std::unique_ptr<TerrainCollisionHelper> m_terrainCollider; /// Terrain Collision detecter and manager
	float m_pitch; /// The pitch of the mouse movement
	float m_yaw; /// The yaw of the mouse movement
	DirectX::XMVECTORF32 m_direction; /// The direction the keys have been pressed to form 
	DirectX::XMVECTORF32 m_position; /// The position of the camera
	DirectX::XMMATRIX m_camRotationMatrix; /// The rotation matrix of the camera
	DirectX::XMVECTOR DefaultForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); /// The Default forward facing vector
	DirectX::XMVECTOR DefaultRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f); /// The Default Right facing vector
	DirectX::XMVECTOR m_camTarget; /// The target that the camera will look at
	D3D12_RECT m_scissorRect; /// Rect that anything outside of it will not be displayed
	bool isJumping; /// Is camera or player jumping
	const float c_cameraGroundOffset = 2; /// The constant offset of the camera from the ground
	bool m_isUnderGround; /// is the camera underground 

};

