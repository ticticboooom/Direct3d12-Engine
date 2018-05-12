#pragma once
#include <memory>
#include "DeviceResources.h"
#include "RendererManager.h"
class D3DENGINE_API App
{
public:
	App();
	~App();
	int Run(HINSTANCE hInstance, int nCmdShow, std::shared_ptr<RendererManager> rendererManager);
protected:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	std::shared_ptr<DX::DeviceResources> GetDeviceResources();
	void ToggleFullscreen();
	static RECT m_windowRect;
	static const LONG c_windowStyle = WS_OVERLAPPEDWINDOW;
private:
	bool m_fullscreenMode;
	bool m_cursorVisible;
	HWND m_hwnd;
	bool isInitialised;
	std::shared_ptr<RendererManager> m_rendererManager;
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
};

