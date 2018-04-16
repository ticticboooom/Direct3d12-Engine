#pragma once
#include "BufferManagerBase.h"

/**
 * @brief  Index beffers are for the indices which are what tell the order of the vertices should be usually so there doesn't need to be more vertices than needed
 * 
 */
class IndexBufferManager : public BufferManagerBase
{
public:
	D3DENGINE_API IndexBufferManager(std::shared_ptr<std::vector<unsigned int>> vertices, const std::shared_ptr<DX::DeviceResources> deviceResources,
				const std::shared_ptr<CommandListManager> commandListManager);
	D3DENGINE_API ~IndexBufferManager();
	D3DENGINE_API D3D12_INDEX_BUFFER_VIEW CreateIndexBufferView();
private:
	std::shared_ptr<std::vector<unsigned int>> m_indices; /// The index data
	size_t m_indicesSize; /// size of the index data
};
