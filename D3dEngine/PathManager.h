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
private:
	std::shared_ptr<std::wstring> AssetPath; /// the wstring asset path
	std::shared_ptr<std::string> AssetPathStr; /// the string asset path
};




