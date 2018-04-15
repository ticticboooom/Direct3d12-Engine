#pragma once
#include "BufferManagerBase.h"


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
