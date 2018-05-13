#pragma once
#include "Component.h"
#include "VertexBufferManager.h"
#include "IndexBufferManager.h"
#include "TerrainGenerationHelper.h"
#include <map>
class D3DENGINE_API TerrainComponent : public Component
{
public:
	TerrainComponent();
	~TerrainComponent();

	virtual int InitRootSignatureParameters(int indexOffset) override;
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void OnKeyDown(UINT key) override;
	virtual void OnKeyUp(UINT key) override;
	virtual void OnMouseMoved(float x, float y) override;
	virtual void OnDeviceRemoved() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void CreateDeviceDependentResoures() override;
	void UseTexture(std::wstring filename);
	static XMVECTOR m_playerPos;
private:
	void CreateChunkFromCoords(float x, float z);
	std::unique_ptr<TerrainGenerationHelper> m_terrainGenerator;
	std::vector<D3D12_VERTEX_BUFFER_VIEW> m_terrainVertexBufferViews;
	std::vector<std::shared_ptr<VertexBufferManager>> m_terrainVertexBufferManagers;
	std::vector<D3D12_INDEX_BUFFER_VIEW> m_terrainIndexBufferViews;
	std::vector<std::shared_ptr<IndexBufferManager>> m_terrainIndexBufferManagers;
	std::vector<UINT> m_indexCounts;
	std::vector<XMFLOAT2> m_newPositions;
	static bool m_isRootSignatureInitialised;
	std::wstring m_texturePath;
	bool m_usingTexture;
	static UINT m_textureRootSigIndex;
	UINT m_textureDescHeapIndex;
	const int gridOffset = 3;
};

