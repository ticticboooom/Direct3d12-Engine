#pragma once
#include "Component.h"
#include "Structures.h"
#include "TextureResourceManager.h"
#include "VertexBufferManager.h"
#include "IndexBufferManager.h"
#include "ConstantBufferManager.h"
class D3DENGINE_API Mesh : public Component
{
public:
	Mesh(std::string filename, bool isAnimated);
	~Mesh();
	
	// Inherited via Component
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
	
	Structures::Transform transform;
protected:
	std::shared_ptr<Structures::VerticesIndicesFromBin> m_meshData;
	std::shared_ptr<TextureResourceManager> m_textureManager;
	UINT m_textureRootSigIndex;
	UINT m_textureDescHeapIndex;
	std::wstring m_texturePath;
	bool m_usingTexture;
	bool m_meshLoaded;


	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	std::unique_ptr<VertexBufferManager> m_vertexBufferManager;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
	std::unique_ptr<IndexBufferManager> m_indexBufferManager;
	UINT m_indexCount;
private:
	static bool m_isRootSignatureInitialised;

};

