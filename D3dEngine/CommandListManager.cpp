#include "stdafx.h"
#include "CommandListManager.h"
#include "DirectXHelper.h"
#include <memory>

CommandListManager::CommandListManager(std::shared_ptr<DX::DeviceResources> deviceResources,
	ID3D12PipelineState* pipelineState, D3D12_COMMAND_LIST_TYPE type) :
	m_deviceResources(deviceResources)
{
	ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateCommandList(0, type, m_deviceResources->GetCommandAllocator(), pipelineState, IID_PPV_ARGS(&m_commandList)));
	NAME_D3D12_OBJECT(m_commandList);
}


CommandListManager::~CommandListManager()
{
}

ID3D12GraphicsCommandList* CommandListManager::Get()
{
	return m_commandList.Get();
}

void CommandListManager::UpdateSubresource(ID3D12Resource* resource, ID3D12Resource* resourceUpload, D3D12_SUBRESOURCE_DATA* desc)
{
	UpdateSubresources(m_commandList.Get(), resource, resourceUpload, 0, 0, 1, desc);
}


void CommandListManager::CreateResourceBarrier(int numBarriers, CD3DX12_RESOURCE_BARRIER* resource)
{
	m_commandList->ResourceBarrier(numBarriers, resource);
}

void CommandListManager::CloseAndExcecute()
{
	ThrowIfFailed(m_commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_deviceResources->GetCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}

void CommandListManager::SetVertexBuffers(UINT StartSlot, UINT Count, const D3D12_VERTEX_BUFFER_VIEW* VBViews)
{
	m_commandList->IASetVertexBuffers(StartSlot, Count, VBViews);
}

void CommandListManager::SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW* IBView)
{
	m_commandList->IASetIndexBuffer(IBView);
}

void CommandListManager::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
	m_commandList->IASetPrimitiveTopology(topology);
}

void CommandListManager::DrawInstanced(UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation,
	UINT StartInstanceLocation)
{
	m_commandList->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

void CommandListManager::DrawIndexedInstanced(UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartIndexLocation,
	UINT StartInstanceLocation)
{
	m_commandList->DrawIndexedInstanced(VertexCountPerInstance, InstanceCount, StartIndexLocation, StartVertexLocation, StartInstanceLocation);
}


void CommandListManager::SetRenderTargets(UINT NumRTVs, const D3D12_CPU_DESCRIPTOR_HANDLE RTVs[], D3D12_CPU_DESCRIPTOR_HANDLE DSV)
{
	m_commandList->OMSetRenderTargets(NumRTVs, RTVs, FALSE, &DSV);
}

void CommandListManager::ClearRenderTargetView(const D3D12_CPU_DESCRIPTOR_HANDLE& target, DirectX::XMVECTORF32 clearColor)
{
	m_commandList->ClearRenderTargetView(target, clearColor, 0, nullptr);
}

void CommandListManager::ClearDepthStencilView(const D3D12_CPU_DESCRIPTOR_HANDLE& depthStencil,
	enum D3D12_CLEAR_FLAGS clearFlags, int depth, int stencil, int rects, D3D12_RECT* pRects)
{
	m_commandList->ClearDepthStencilView(depthStencil, clearFlags, depth, stencil, rects, pRects);
}

void CommandListManager::SetScissorRects(int numRects, D3D12_RECT* rect)
{
	m_commandList->RSSetScissorRects(numRects, rect);
}

void CommandListManager::SetViewports(int numViewports, D3D12_VIEWPORT* viewport)
{
	m_commandList->RSSetViewports(numViewports, viewport);
}

void CommandListManager::SetGraphicsRootDescriptorTable(int rootParameterIndex,
	const CD3DX12_GPU_DESCRIPTOR_HANDLE& handle)
{
	m_commandList->SetGraphicsRootDescriptorTable(rootParameterIndex, handle);
}

void CommandListManager::SetDescriptorHeaps(UINT numDescriptors, ID3D12DescriptorHeap* const* ppHeaps)
{
	m_commandList->SetDescriptorHeaps(numDescriptors, ppHeaps);
}

void CommandListManager::SetGraphicsRootSignature(Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature)
{
	m_commandList->SetGraphicsRootSignature(rootSignature.Get());
}

HRESULT CommandListManager::Reset(ID3D12CommandAllocator* commandAllocator,
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState)
{
	return m_commandList->Reset(commandAllocator, pipelineState.Get());
}

void CommandListManager::ClearSetRenderTargets()
{
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView = m_deviceResources->GetRenderTargetView();
	const D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = m_deviceResources->GetDepthStencilView();
	this->ClearRenderTargetView(renderTargetView, DirectX::Colors::CornflowerBlue);	
	this->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	this->SetRenderTargets(1, &renderTargetView, depthStencilView);
}

void CommandListManager::CreateResourceBarrier(D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter)
{
	CD3DX12_RESOURCE_BARRIER renderTargetResourceBarrier =
		CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetRenderTarget(), stateBefore, stateAfter);
	this->CreateResourceBarrier(1, &renderTargetResourceBarrier);
}
