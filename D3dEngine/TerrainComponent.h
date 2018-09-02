#pragma once
#include "Component.h"
#include "VertexBufferManager.h"
#include "IndexBufferManager.h"
#include "TerrainGenerationHelper.h"
#include "Thread.h"
#include <concurrent_vector.h>
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
	void CreateTerrainPoints();
	void CreateChunkFromData(std::shared_ptr<Structures::VerticesIndicesFromBin> data);
	void CreateChunks();
	std::unique_ptr<TerrainGenerationHelper> m_terrainGenerator;
	std::vector<D3D12_VERTEX_BUFFER_VIEW> m_terrainVertexBufferViews;
	std::vector<std::shared_ptr<VertexBufferManager>> m_terrainVertexBufferManagers;
	std::vector<D3D12_INDEX_BUFFER_VIEW> m_terrainIndexBufferViews;
	std::vector<std::shared_ptr<IndexBufferManager>> m_terrainIndexBufferManagers;
	std::vector<UINT> m_indexCounts;
	std::vector<XMFLOAT2> m_newPositions;
	std::vector<std::unique_ptr<Thread>> m_loadingThreads;
	concurrency::concurrent_vector<std::shared_ptr<Structures::VerticesIndicesFromBin>> m_generated;
	static bool m_isRootSignatureInitialised;
	std::wstring m_texturePath;
	bool m_usingTexture;
	static UINT m_textureRootSigIndex;
	UINT m_textureDescHeapIndex;
	const int c_gridOffset = 3;
	const float c_raySize = 20;
	UINT m_runningTerrainThreads;
	bool m_isGeneratedVectorBeingWrittenTo;
	bool m_isGeneratedVectorBeingRead;
	bool m_isPrevThreadComplete;
};

