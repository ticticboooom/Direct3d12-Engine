#pragma once
#include "stdafx.h"
namespace DX {
	static const UINT c_frameCount = 3;		// Use triple buffering.
	/**
	 * @brief the resources required for the D3D device
	 *
	 */
	class D3DENGINE_API DeviceResources
	{
	public:
		DeviceResources(DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT);
		void SetWindow(HWND window);
		void SetLogicalSize(DirectX::XMFLOAT2 logicalSize);
		void SetDpi(float dpi);
		void ValidateDevice();
		void Present();
		void WaitForGpu();
		// The size of the render target, in pixels.
		DirectX::XMFLOAT2	GetOutputSize() const { return m_outputSize; }
		// The size of the render target, in dips.
		DirectX::XMFLOAT2	GetLogicalSize() const { return m_logicalSize; }

		float						GetDpi() const { return m_effectiveDpi; }
		bool						IsDeviceRemoved() const { return m_deviceRemoved; }
		// D3D Accessors.
		ID3D12Device*				GetD3DDevice() const { return m_d3dDevice.Get(); }
		IDXGISwapChain3*			GetSwapChain() const { return m_swapChain.Get(); }
		ID3D12Resource*				GetRenderTarget() const { return m_renderTargets[m_currentFrame].Get(); }
		ID3D12Resource*				GetDepthStencil() const { return m_depthStencil.Get(); }
		ID3D12CommandQueue*			GetCommandQueue() const { return m_commandQueue.Get(); }
		ID3D12CommandAllocator*		GetCommandAllocator() const { return m_commandAllocators[m_currentFrame].Get(); }
		DXGI_FORMAT					GetBackBufferFormat() const { return m_backBufferFormat; }
		DXGI_FORMAT					GetDepthBufferFormat() const { return m_depthBufferFormat; }
		D3D12_VIEWPORT				GetScreenViewport() const { return m_screenViewport; }
		UINT						GetCurrentFrameIndex() const { return m_currentFrame; }

		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() const
		{
			return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_currentFrame, m_rtvDescriptorSize);
		}
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const
		{
			return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
		}

	private:
		void CreateDeviceIndependentResources();
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();
		void UpdateRenderTargetSize();
		void MoveToNextFrame();
		void GetHardwareAdapter(IDXGIAdapter1** ppAdapter);

		UINT											m_currentFrame;

		// Direct3D objects.
		Microsoft::WRL::ComPtr<ID3D12Device>			m_d3dDevice;
		Microsoft::WRL::ComPtr<IDXGIFactory4>			m_dxgiFactory;
		Microsoft::WRL::ComPtr<IDXGISwapChain3>			m_swapChain;
		Microsoft::WRL::ComPtr<ID3D12Resource>			m_renderTargets[c_frameCount];
		Microsoft::WRL::ComPtr<ID3D12Resource>			m_depthStencil;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_rtvHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	m_dsvHeap;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>		m_commandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>	m_commandAllocators[c_frameCount];
		DXGI_FORMAT										m_backBufferFormat;
		DXGI_FORMAT										m_depthBufferFormat;
		D3D12_VIEWPORT									m_screenViewport;
		UINT											m_rtvDescriptorSize;
		bool											m_deviceRemoved;

		// CPU/GPU Synchronization.
		Microsoft::WRL::ComPtr<ID3D12Fence>				m_fence;
		UINT64											m_fenceValues[c_frameCount];
		HANDLE											m_fenceEvent;

		// Cached reference to the Window.
		HWND											m_window;

		// Cached device properties.
		DirectX::XMFLOAT2								m_d3dRenderTargetSize;
		DirectX::XMFLOAT2								m_outputSize;
		DirectX::XMFLOAT2								m_logicalSize;
		float											m_dpi;

		// This is the DPI that will be reported back to the app. It takes into account whether the app supports high resolution screens or not.
		float											m_effectiveDpi;
	};
}