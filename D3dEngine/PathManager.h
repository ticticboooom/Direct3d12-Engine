#pragma once
#include "stdafx.h"
#include "DirectXHelper.h"
class PathManager
{
public:
	D3DENGINE_API PathManager();
	D3DENGINE_API ~PathManager();
	D3DENGINE_API  const wchar_t* GetAssetPath() { return AssetPath->c_str(); };
	D3DENGINE_API const char* GetAssetPathStr()  { return AssetPathStr->c_str(); };
	//D3DENGINE_API void AppendWString(const wchar_t* wcs0, const wchar_t* wcs1, wchar_t* result) const;
private:
	std::shared_ptr<std::wstring> AssetPath;
	std::shared_ptr<std::string> AssetPathStr;
};




