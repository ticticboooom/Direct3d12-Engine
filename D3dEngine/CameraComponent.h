#pragma once
#include "Component.h"
class D3DENGINE_API CameraComponent : public Component
{
public:
	CameraComponent();
	~CameraComponent();

	virtual int InitRootSignatureParameters(int indexOffset) override;
	virtual void Init(std::shared_ptr<CommandListManager>* commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT * descOffset, std::shared_ptr<PSOManager>* pso) override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void OnKeyDown(UINT key) override;
	virtual void OnKeyUp(UINT key) override;
	virtual void OnMouseMoved(float x, float y) override;
	virtual void OnDeviceRemoved() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void CreateDeviceDependentResoures() override;
private:
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

