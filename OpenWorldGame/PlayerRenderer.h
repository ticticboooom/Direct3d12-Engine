#pragma once
#include "stdafx.h"
#include "IGameBase.h"
#include "RootSignatureManager.h"
#include "Structures.h"
#include "PSOManager.h"
#include "CommandListManager.h"
#include "IndexBufferManager.h"
#include "VertexBufferManager.h"
#include "DescriptorHeapManager.h"
#include "ConstantBufferManager.h"
#include "AnimationManager.h"
#include "RendererRequiredStructures.h"
#include "CameraHelper.h"
#include "TerrainCollisionHelper.h"
#include "PhysicsHelper.h"
#include "DeviceResources.h"

class PlayerRenderer : public IGameBase
{
public:
	PlayerRenderer(Structures::RequiredRendererObjects objects);
	~PlayerRenderer() override;
	int InitRootSignatureParameters(int indexOffset) override;
	void RunCommandLists() override;
	std::shared_ptr<CommandListManager> Init(std::shared_ptr<CommandListManager> commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT* descOffset, std::shared_ptr<PSOManager>* pso) override;
	void Update() override;
	void Render() override;
	void OnKeyDown(UINT key) override;
	void OnKeyUp(UINT key) override;
	void OnMouseMoved(float x, float y) override;
	void OnDeviceRemoved() override;
	void CreateWindowSizeDependentResources() override;

private:
	void CreateDeviceDependentResoures() override;
	std::shared_ptr<CameraHelper> m_cameraHelper;
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	std::shared_ptr<RootSignatureManager> m_rootSignatureManager;
	std::shared_ptr<ModelViewProjectionManager> m_mvpManager;
	UINT m_rootSigIndexOffset;
	UINT m_descHeapOffset;
	std::shared_ptr<PSOManager> m_psoManager;
	std::shared_ptr<CommandListManager> m_commandListManager;

	D3D12_VERTEX_BUFFER_VIEW m_playerVertexBufferView;
	std::unique_ptr<VertexBufferManager> m_playerVertexBufferManager;
	D3D12_INDEX_BUFFER_VIEW m_playerIndexBufferView;
	std::unique_ptr<IndexBufferManager> m_playerIndexBufferManager;

	std::unique_ptr<AnimationManager> m_animationManager;
	std::shared_ptr<DescriptorHeapManager> m_cbvSrvHeapManager;
	std::unique_ptr<ConstantBufferManager<Structures::ModelViewProjectionConstantBuffer>> m_mvpConstantBufferManager;
	std::unique_ptr<ConstantBufferManager<XMFLOAT4X4>> m_rotatorConstantBuffer;
	std::unique_ptr<ConstantBufferManager<XMFLOAT4X4>> m_animationConstantBufferManager;
	std::shared_ptr<TerrainCollisionHelper> m_terrainCollisionHelper;
	std::shared_ptr<PhysicsHelper> m_physicsHelper;
	BoundingBox collider;
	UINT m_cbvDescriptorSize;
	UINT m_indexCount;
	D3D12_RECT m_scissorRect;
	DirectX::XMVECTOR m_playerPosition;
	DirectX::XMVECTORF32 m_playerRotation;
	UINT m_frame = 0;
	int seed;
	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;
};
