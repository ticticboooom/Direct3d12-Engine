#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif
#include <windowsx.h>
#include <windows.h>
#include <memory>
#include <d3d12.h>
#include <wincodec.h>
#include <dxgi1_4.h>
#include <dxgi.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include "d3dx12.h"
#include <vector>

#include <string>
#include <wrl.h>
#include <shellapi.h>
#if defined(_DEBUG)
#include <dxgidebug.h>

#endif
#pragma comment (lib, "d3dcompiler")
#pragma comment (lib, "d3d12")
#pragma comment (lib, "dxgi")
using namespace DirectX;
