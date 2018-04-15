#include "stdafx.h"
#include "CommandListManager.h"
#include "DirectXHelper.h"
#include <memory>

/**
 * @brief Construct a new Command List Manager:: Command List Manager object
 *  creates a command list and gives it a debug name
 * @param deviceResources 
 * @param pipelineState PSO (Pipeline state object)
 * @param type the type of command list that will be created
 */
CommandListManager::CommandListManager(std::shared_ptr<DX::DeviceResources> deviceResources,
	ID3D12PipelineState* pipelineState, D3D12_COMMAND_LIST_TYPE type) :
	m_deviceResources(deviceResources)
{
	ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateCommandList(0, type, m_deviceResources->GetCommandAllocator(), pipelineState, IID_PPV_ARGS(&m_commandList)));
	NAME_D3D12_OBJECT(m_commandList);
}

/**
 * @brief Destroy the Command List Manager:: Command List Manager object
 * 
 */
CommandListManager::~CommandListManager()
{
}

/**
 * @brief gets the raw pointer of the command list
 * 
 * @return ID3D12GraphicsCommandList* the raw pointer of the command list
 */
ID3D12GraphicsCommandList* CommandListManager::Get()
{
	return m_commandList.Get();
}

/**
 * @brief updates a subresource such as (vertices in vertex buffer or indices in an index buffer)
 * 
 * @param resource the Direct3d 12 default heap resource that will be updated
 * @param resourceUpload the Direct3d 12 upload heap resource that will be updated
 * @param desc the resource descriptor
 */
void CommandListManager::UpdateSubresource(ID3D12Resource* resource, ID3D12Resource* resourceUpload, D3D12_SUBRESOURCE_DATA* desc)
{
	UpdateSubresources(m_commandList.Get(), resource, resourceUpload, 0, 0, 1, desc);
}

/**
 * @brief Creates a resource barrier
 *  Which notifies a driver that it needs to synchronise multiple accesses to a resource
 * 
 * @param numBarriers count / number of barriers to be made
 * @param resource The resourcebarrier to be made
 */
void CommandListManager::CreateResourceBarrier(int numBarriers, CD3DX12_RESOURCE_BARRIER* resource)
{
	m_commandList->ResourceBarrier(numBarriers, resource);
}

/**
 * @brief closes and executes the command list
 *  This ensures that the next command list can be added to and the commands stored inside will be able to be rendered
 * 
 */
void CommandListManager::CloseAndExcecute()
{
	ThrowIfFailed(m_commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_deviceResources->GetCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}

/**
 * @brief renderers a vertex buffer
 *  this renders vertices in a 3D space
 * @param StartSlot The start of the vertex buffer
 * @param Count The number of vertex buffers
 * @param VBViews The vewrtex Buffer views to be rendered
 */
void CommandListManager::SetVertexBuffers(UINT StartSlot, UINT Count, const D3D12_VERTEX_BUFFER_VIEW* VBViews)
{
	m_commandList->IASetVertexBuffers(StartSlot, Count, VBViews);
}

/**
 * @brief Use an index buffer for the vertices
 *  index buffers are an index of which vertices to show in an order so that you don't need to repeat vertices
 * @param IBView the index buffer view that the vertices will use.
 */
void CommandListManager::SetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW* IBView)
{
	m_commandList->IASetIndexBuffer(IBView);
}

/**
 * @brief Set the type of shape to use (Primitive Topology)
 * 
 * @param topology the type of shape
 */
void CommandListManager::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
	m_commandList->IASetPrimitiveTopology(topology);
}

/**
 * @brief draw vertices that are not with an index buffer
 * 
 * @param VertexCountPerInstance  the number of vertices per instance
 * @param InstanceCount the number of instances
 * @param StartVertexLocation the start location in the vertex buffer
 * @param StartInstanceLocation the start point in the number of instances
 */
void CommandListManager::DrawInstanced(UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation,
	UINT StartInstanceLocation)
{
	m_commandList->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

/**
 * @brief draw a vertex buffer with an index buffer
 * 
 * @param VertexCountPerInstance  the number of vertices per instance
 * @param InstanceCount the number of instances
 * @param StartVertexLocation the start location in the vertex buffer
 * @param StartIndexLocation the start point in the index buffer
 * @param StartInstanceLocation the start point in the number of instances
 */
void CommandListManager::DrawIndexedInstanced(UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartIndexLocation,
	UINT StartInstanceLocation)
{
	m_commandList->DrawIndexedInstanced(VertexCountPerInstance, InstanceCount, StartIndexLocation, StartVertexLocation, StartInstanceLocation);
}

/**
 * @brief set a render target to rendeer things to
 * 
 * @param NumRTVs number of render targets
 * @param RTVs the render targets to set
 * @param DSV the depth stencil view to be used
 */
void CommandListManager::SetRenderTargets(UINT NumRTVs, const D3D12_CPU_DESCRIPTOR_HANDLE RTVs[], D3D12_CPU_DESCRIPTOR_HANDLE DSV)
{
	m_commandList->OMSetRenderTargets(NumRTVs, RTVs, FALSE, &DSV);
}

/**
 * @brief Clear the render target then set to a block colour
 * 
 * @param target the render target to clear
 * @param clearColor the color to set it to
 */
void CommandListManager::ClearRenderTargetView(const D3D12_CPU_DESCRIPTOR_HANDLE& target, DirectX::XMVECTORF32 clearColor)
{
	m_commandList->ClearRenderTargetView(target, clearColor, 0, nullptr);
}

/**
 * @brief clear the depth stencil view
 * 
 * @param depthStencil the stencil to clear
 * @param D3D12_CLEAR_FLAGS flags to clear it with
 * @param depth the depth to set it to
 * @param stencil the stencil to set
 * @param rects the number of rects to be used
 * @param pRects the rects to be used
 */
void CommandListManager::ClearDepthStencilView(const D3D12_CPU_DESCRIPTOR_HANDLE& depthStencil,
	enum D3D12_CLEAR_FLAGS clearFlags, int depth, int stencil, int rects, D3D12_RECT* pRects)
{
	m_commandList->ClearDepthStencilView(depthStencil, clearFlags, depth, stencil, rects, pRects);
}

/**
 * @brief Set the scissor rect to be used
 *  anything outside of the scissor rect will not be displayed and will be cut off.
 * @param numRects the number of rects to use
 * @param rect the rects to use
 */
void CommandListManager::SetScissorRects(int numRects, D3D12_RECT* rect)
{
	m_commandList->RSSetScissorRects(numRects, rect);
}

/**
 * @brief Set the view port to use
 * 
 * @param numViewports the number of viewports to be used
 * @param viewport the viewports  to use
 */
void CommandListManager::SetViewports(int numViewports, D3D12_VIEWPORT* viewport)
{
	m_commandList->RSSetViewports(numViewports, viewport);
}

/**
 * @brief Set the descriptor table to be used
 * 
 * @param rootParameterIndex the index of the root paramter used for the descriptor table
 * @param handle the descriptor table handle
 */
void CommandListManager::SetGraphicsRootDescriptorTable(int rootParameterIndex,
	const CD3DX12_GPU_DESCRIPTOR_HANDLE& handle)
{
	m_commandList->SetGraphicsRootDescriptorTable(rootParameterIndex, handle);
}

/**
 * @brief set the descriptor heaps that will be used
 * 
 * @param numDescriptors the number of descriptors to be used
 * @param ppHeaps the array of pointers to descriptor heaps
 */
void CommandListManager::SetDescriptorHeaps(UINT numDescriptors, ID3D12DescriptorHeap* const* ppHeaps)
{
	m_commandList->SetDescriptorHeaps(numDescriptors, ppHeaps);
}

/**
 * @brief Set the root signatuire that will be used in rendering
 * 
 * @param rootSignature the root signature pointer to be used
 */
void CommandListManager::SetGraphicsRootSignature(Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature)
{
	m_commandList->SetGraphicsRootSignature(rootSignature.Get());
}

/**
 * @brief Reset the command list
 * 
 * @param commandAllocator the command allocator to reset the command list with
 * @param pipelineState the PSO (pipeline state object) to init it with
 * @return HRESULT the result of resetting the command list
 */
HRESULT CommandListManager::Reset(ID3D12CommandAllocator* commandAllocator,
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState)
{
	return m_commandList->Reset(commandAllocator, pipelineState.Get());
}

/**
 * @brief clear the render targets that are currently set
 * 
 */
void CommandListManager::ClearSetRenderTargets()
{
	// get the rtv
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView = m_deviceResources->GetRenderTargetView();
	// get the dsv
	const D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = m_deviceResources->GetDepthStencilView();
	// reset with corflower blue colour and then set the render target again
	this->ClearRenderTargetView(renderTargetView, DirectX::Colors::CornflowerBlue);	
	this->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	this->SetRenderTargets(1, &renderTargetView, depthStencilView);
}

/**
 * @brief Wrapper for creating resource barrier
 * 
 * @param stateBefore previouse sate to change from
 * @param stateAfter state to be changed to
 */
void CommandListManager::CreateResourceBarrier(D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter)
{
	CD3DX12_RESOURCE_BARRIER renderTargetResourceBarrier =
		CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetRenderTarget(), stateBefore, stateAfter);
	this->CreateResourceBarrier(1, &renderTargetResourceBarrier);
}
