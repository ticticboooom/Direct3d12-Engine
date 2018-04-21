#include "stdafx.h"
#include "App.h"
#include "CommonObjects.h"

RECT App::m_windowRect = RECT{ 0,0,0,0 };
App* m_app;
App::App() :
	m_fullscreenMode(false),
	m_cursorVisible(false)
{
	m_app = this;
}



App::~App()
{
}

int App::Run(HINSTANCE hInstance, int nCmdShow, std::shared_ptr<RendererManager> rendererManager)
{
	int argc;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	LocalFree(argv);

	// Initialize the window class.
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = L"DXSampleClass";
	RegisterClassEx(&windowClass);

	m_windowRect = { 0, 0, static_cast<LONG>(1500), static_cast<LONG>(1000) };
	AdjustWindowRect(&m_windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window and store a handle to it.
	m_hwnd = CreateWindow(
		windowClass.lpszClassName,
		L"Game",
		c_windowStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_windowRect.right - m_windowRect.left,
		m_windowRect.bottom - m_windowRect.top,
		nullptr,		// We have no parent window.
		nullptr,		// We aren't using menus.
		hInstance,
		NULL);

	m_rendererManager = rendererManager;
	isInitialised = true;
	// Initialize the sample. OnInit is defined in each child-implementation of DXSample.
	GetDeviceResources();

	ShowWindow(m_hwnd, nCmdShow);
	ShowCursor(m_cursorVisible);
	// Main sample loop.
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		RECT rect;
		RECT cRect;
		GetWindowRect(m_hwnd, &rect);
		ClipCursor(&rect);
		GetClientRect(m_hwnd, &cRect);
		DirectX::XMFLOAT2 deaultCursorPos = { static_cast<float>(cRect.right / 2) , static_cast<float>(cRect.bottom / 2) };
		POINT pt;
		pt.x = deaultCursorPos.x;
		pt.y = deaultCursorPos.y;
		ClientToScreen(m_hwnd, &pt);
		// Process any messages in the queue.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_KEYDOWN)
			{
				UINT8 key = static_cast<UINT8>(msg.wParam);
				if (key == VK_ESCAPE)
				{
					GetDeviceResources()->WaitForGpu();
					PostMessage(m_hwnd, WM_CLOSE, 0, 0);
				}
				else if (key == VK_F11) {
					ToggleFullscreen();
				}
				else if (key == VK_F1) {
					m_cursorVisible = !m_cursorVisible;
					ShowCursor(m_cursorVisible);
				}
				m_rendererManager->OnKeyDown(key);
			}
			else if (msg.message == WM_KEYUP)
			{
				m_rendererManager->OnKeyUp(static_cast<UINT8>(msg.wParam));
			}
			else if (msg.message == WM_MOUSEMOVE)
			{
				const int xLparam = GET_X_LPARAM(msg.lParam), yLparam = GET_Y_LPARAM(msg.lParam);
				const int x = deaultCursorPos.x - xLparam, y = deaultCursorPos.y - yLparam;
				m_rendererManager->OnMouseMoved(x, y);
				SetCursorPos(pt.x, pt.y);
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			m_rendererManager->Update();
			m_rendererManager->Render();
			GetDeviceResources()->Present();

		}
	}
	ClipCursor(nullptr);
	m_rendererManager->OnDeviceRemoved();

	// Return this part of the WM_QUIT message to Windows.
	return static_cast<char>(msg.wParam);
}
LRESULT CALLBACK App::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		// Save the DXSample* passed in to CreateWindow.
		LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
	}
	return 0;
	case WM_SIZE:
	{
		if (m_app->isInitialised) {
			RECT rect;
			GetClientRect(hWnd, &rect);
			m_app->GetDeviceResources()->SetLogicalSize(
				DirectX::XMFLOAT2(
					float(rect.right),
					float(rect.bottom)));
			m_app->m_rendererManager->CreateWindowSizeDependentResources();
		}
	}
	return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	// Handle any messages the switch statement didn't.
	return DefWindowProc(hWnd, message, wParam, lParam);
}


std::shared_ptr<DX::DeviceResources> App::GetDeviceResources()
{
	if (m_deviceResources != nullptr && m_deviceResources->IsDeviceRemoved())
	{
		// All references to the existing D3D device must be released before a new device
		// can be created.

		m_deviceResources = nullptr;
		m_rendererManager->OnDeviceRemoved();
	}

	if (m_deviceResources == nullptr)
	{
		m_deviceResources = std::make_shared<DX::DeviceResources>();
		m_deviceResources->SetWindow(m_hwnd);
		CommonObjects::m_deviceResources = m_deviceResources;
		m_rendererManager->CreateRenderers();
	}
	return m_deviceResources;
}

void App::ToggleFullscreen()
{
	if (m_fullscreenMode)
	{
		// Restore the window's attributes and size.
		SetWindowLong(m_hwnd, GWL_STYLE, c_windowStyle);

		SetWindowPos(
			m_hwnd,
			HWND_NOTOPMOST,
			m_windowRect.left,
			m_windowRect.top,
			m_windowRect.right - m_windowRect.left,
			m_windowRect.bottom - m_windowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);

		ShowWindow(m_hwnd, SW_NORMAL);
	}
	else
	{
		// Save the old window rect so we can restore it when exiting fullscreen mode.
		GetWindowRect(m_hwnd, &m_windowRect);

		// Make the window borderless so that the client area can fill the screen.
		SetWindowLong(m_hwnd, GWL_STYLE, c_windowStyle & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME));

		// Get the settings of the primary display. We want the app to go into
		// fullscreen mode on the display that supports Independent Flip.
		DEVMODE devMode = {};
		devMode.dmSize = sizeof(DEVMODE);
		EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);

		SetWindowPos(
			m_hwnd,
			HWND_TOPMOST,
			devMode.dmPosition.x,
			devMode.dmPosition.y,
			devMode.dmPosition.x + devMode.dmPelsWidth,
			devMode.dmPosition.y + devMode.dmPelsHeight,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);

		ShowWindow(m_hwnd, SW_MAXIMIZE);
	}

	m_fullscreenMode = !m_fullscreenMode;

}
