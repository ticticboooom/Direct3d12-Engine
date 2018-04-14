#include "stdafx.h"
#include "IndexBufferManager.h"
#include "VertexBufferManager.h"

IndexBufferManager::IndexBufferManager(std::shared_ptr<std::vector<unsigned int>> indices,
	const std::shared_ptr<DX::DeviceResources> deviceResources,
	const std::shared_ptr<CommandListManager> commandListManager) :
	BufferManagerBase(indices->size() * sizeof(unsigned int),
		reinterpret_cast<BYTE*>(indices->data()),
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		deviceResources,
		commandListManager),
	m_indices(indices)
{
	m_indicesSize = m_indices->size();
}

IndexBufferManager::~IndexBufferManager()
{
	m_indices->clear();
}

D3D12_INDEX_BUFFER_VIEW IndexBufferManager::CreateIndexBufferView()
{
	D3D12_INDEX_BUFFER_VIEW vertexBufferView;
	vertexBufferView.BufferLocation = GetResource()->GetGPUVirtualAddress();
	vertexBufferView.Format = DXGI_FORMAT_R32_UINT;
	vertexBufferView.SizeInBytes = sizeof(unsigned int) * m_indicesSize;

	return vertexBufferView;
}
