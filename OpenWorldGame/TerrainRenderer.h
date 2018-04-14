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
#include "ModelViewProjectionManager.h"
#include "RendererRequiredStructures.h"
#include "CameraHelper.h"
#include "TerrainGenerationHelper.h";
#include "TerrainCollisionHelper.h"

class TerrainRenderer : public IGameBase
{
public:
	TerrainRenderer(Structures::RequiredRendererObjects objects);
	~TerrainRenderer() override;
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
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	std::shared_ptr<RootSignatureManager> m_rootSignatureManager;
	std::shared_ptr<CameraHelper> m_cameraHelper;
	std::shared_ptr<ModelViewProjectionManager> m_mvpManager;
	UINT m_rootSigIndexOffset;
	UINT m_descHeapOffset;
	std::shared_ptr<PSOManager> m_psoManager;
	std::shared_ptr<CommandListManager> m_commandListManager;

	D3D12_VERTEX_BUFFER_VIEW m_terrainVertexBufferView;
	std::unique_ptr<VertexBufferManager> m_terrainVertexBufferManager;
	D3D12_INDEX_BUFFER_VIEW m_terrainIndexBufferView;
	std::unique_ptr<IndexBufferManager> m_terrainIndexBufferManager;
	std::unique_ptr<ConstantBufferManager<XMFLOAT4X4>> m_rotatorConstantBuffer;

	std::shared_ptr<DescriptorHeapManager> m_cbvSrvHeapManager;
	std::unique_ptr<ConstantBufferManager<Structures::ModelViewProjectionConstantBuffer>> m_mvpConstantBufferManager;
	std::unique_ptr<TerrainGenerationHelper> m_terrainGenerator;
	std::unique_ptr<ConstantBufferManager<XMFLOAT4X4>> m_axisRotatorConstantBuffer;
	UINT m_cbvDescriptorSize;
	UINT m_indexCount;
	Structures::ModelViewProjectionConstantBuffer m_constantBufferData;

};
