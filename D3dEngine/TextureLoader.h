#pragma once

class TextureLoader
{
public:
	D3DENGINE_API TextureLoader();
	D3DENGINE_API ~TextureLoader();	
	D3DENGINE_API int LoadImageDataFromFile(BYTE** imageData, D3D12_RESOURCE_DESC& resourceDescription, LPCWSTR filename,
	                          int& bytesPerRow);
private:
	DXGI_FORMAT GetDXGIFormatFromWICFormat(WICPixelFormatGUID& wicFormatGUID);
	WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID);
	int GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat);
};
