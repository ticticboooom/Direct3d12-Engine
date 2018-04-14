#include "stdafx.h"
#include "VertexBufferManager.h"
#include "Structures.h"

VertexBufferManager::VertexBufferManager(std::shared_ptr<std::vector<Structures::VertexTexCoordNormal>> vertices, const std::shared_ptr<DX::DeviceResources> deviceResources,
	const std::shared_ptr<CommandListManager> commandListManager) :
	BufferManagerBase(vertices->size() * sizeof Structures::VertexTexCoordNormal,
		reinterpret_cast<BYTE*>(vertices->data()),
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		deviceResources,
		commandListManager),
	m_vertices(vertices)
{
	m_verticesSize = m_vertices->size();
}

VertexBufferManager::~VertexBufferManager()
{
	m_vertices->clear();
}

D3D12_VERTEX_BUFFER_VIEW VertexBufferManager::CreateVertexBufferView() const
{
	auto buffer = GetResource();
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	vertexBufferView.BufferLocation = buffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(Structures::VertexTexCoordNormal);
	vertexBufferView.SizeInBytes = sizeof(Structures::VertexTexCoordNormal) * m_verticesSize;

	return vertexBufferView;
}
