#include "stdafx.h"
#include "IndexBufferManager.h"
#include "VertexBufferManager.h"
/**
 * @brief Construct a new Index Buffer Manager:: Index Buffer Manager object
 * 	 Index beffers are for the indices which are what tell the order of the vertices should be usually so there doesn't need to be more vertices than needed
 * @param indices the vector of indices to be added to the buffer
 * @param deviceResources 
 * @param commandListManager 
 */
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
/**
 * @brief Destroy the Index Buffer Manager:: Index Buffer Manager object
 * 
 */
IndexBufferManager::~IndexBufferManager()
{
	m_indices->clear();
}
/**
 * @brief creates the index bufer view which can be used by the command list to add it to rendering
 * 
 * @return D3D12_INDEX_BUFFER_VIEW the view
 */
D3D12_INDEX_BUFFER_VIEW IndexBufferManager::CreateIndexBufferView()
{
	//describe and create index buffer view
	D3D12_INDEX_BUFFER_VIEW vertexBufferView;
	vertexBufferView.BufferLocation = GetResource()->GetGPUVirtualAddress();
	vertexBufferView.Format = DXGI_FORMAT_R32_UINT;
	vertexBufferView.SizeInBytes = sizeof(unsigned int) * m_indicesSize;

	return vertexBufferView;
}
