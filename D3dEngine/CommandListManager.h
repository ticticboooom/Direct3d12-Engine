#pragma once
#include "stdafx.h"
#include "DeviceResources.h"
#include <memory>

class CommandListManager
{
public:
	D3DENGINE_API CommandListManager(std::shared_ptr<DX::DeviceResources> deviceResources, ID3D12PipelineState * pipelineState, D3D12_COMMAND_LIST_TYPE type);
	D3DENGINE_API ~CommandListManager();
	D3DENGINE_API ID3D12GraphicsCommandList* Get();
	D3DENGINE_API void UpdateSubresource(ID3D12Resource* resource, ID3D12Resource* resourceUpload, D3D12_SUBRESOURCE_DATA* desc);
	D3DENGINE_API void CreateResourceBarrier(int numBarriers, CD3DX12_RESOURCE_BARRIER* resource);
	D3DENGINE_API void CloseAndExcecute();
	D3DENGINE_API void SetVertexBuffers(UINT StartSlot, UINT Count, const D3D12_VERTEX_BUFFER_VIEW VBViews[]);
	D3DENGINE_API void SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW* VBView);
	D3DENGINE_API void SetPrimitiveTopology(enum D3D_PRIMITIVE_TOPOLOGY topology);
	D3DENGINE_API void DrawInstanced(UINT VertexCountPerInstance, UINT InstanceCount,
		UINT StartVertexLocation, UINT StartInstanceLocation);
	D3DENGINE_API void DrawIndexedInstanced(UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation,
		UINT StartIndexLocation, UINT StartInstanceLocation);
	D3DENGINE_API void SetScissorRects(int numRects, D3D12_RECT* rect);
	D3DENGINE_API void SetViewports(int numViewports, D3D12_VIEWPORT* viewport);
	D3DENGINE_API void SetGraphicsRootDescriptorTable(int rootParameterIndex, const CD3DX12_GPU_DESCRIPTOR_HANDLE& handle);
	D3DENGINE_API void SetDescriptorHeaps(UINT numDescriptors, ID3D12DescriptorHeap*const* ppHeaps);
	D3DENGINE_API void SetGraphicsRootSignature(Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature);
	D3DENGINE_API HRESULT Reset(ID3D12CommandAllocator* commandAllocator, Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState);

	D3DENGINE_API void ClearSetRenderTargets();
	D3DENGINE_API void CreateResourceBarrier(D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);
private:
	void SetRenderTargets(UINT NumRTVs, const D3D12_CPU_DESCRIPTOR_HANDLE RTVs[], D3D12_CPU_DESCRIPTOR_HANDLE DSV);
	void ClearRenderTargetView(const D3D12_CPU_DESCRIPTOR_HANDLE& render_target_view, DirectX::XMVECTORF32 clearColor);
	void ClearDepthStencilView(const D3D12_CPU_DESCRIPTOR_HANDLE& depth_stencil_view, enum D3D12_CLEAR_FLAGS d3_d12_clear_flags, int depth, int stencil, int rects, D3D12_RECT* pRects);

	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
};
