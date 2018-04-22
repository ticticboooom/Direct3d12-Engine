#pragma once
#include "Component.h"
#include "VertexBufferManager.h"
#include "IndexBufferManager.h"
#include "TerrainGenerationHelper.h"
class D3DENGINE_API TerrainComponent : public Component
{
public:
	TerrainComponent();
	~TerrainComponent();

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
	void UseTexture(std::wstring filename);
private:
	std::unique_ptr<TerrainGenerationHelper> m_terrainGenerator;
	D3D12_VERTEX_BUFFER_VIEW m_terrainVertexBufferView;
	std::unique_ptr<VertexBufferManager> m_terrainVertexBufferManager;
	D3D12_INDEX_BUFFER_VIEW m_terrainIndexBufferView;
	std::unique_ptr<IndexBufferManager> m_terrainIndexBufferManager;
	UINT m_indexCount;
	static bool m_isRootSignatureInitialised;
	std::wstring m_texturePath;
	bool m_usingTexture;
	static UINT m_textureRootSigIndex;
	UINT m_textureDescHeapIndex;
};

