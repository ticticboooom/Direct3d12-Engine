#include "stdafx.h"
#include "PathManager.h"

PathManager::PathManager()
{
	WCHAR path[MAX_PATH];
	GetAssetsPath(path, MAX_PATH);
	AssetPath = std::make_shared<std::wstring>(path);
	AssetPathStr = std::make_shared<std::string>(AssetPath->begin(), AssetPath->end());
	auto lastSlash = wcsrchr(AssetPath->c_str(), L'\\');
	(*AssetPath)[(wcslen(AssetPath->c_str()) - wcslen(lastSlash) + 1)] = '\0';
	//size_t i;
	//AssetPath = std::make_shared<wchar_t*>(reinterpret_cast<wchar_t*>(malloc(MAX_PATH * sizeof(wchar_t))));
	//mbstowcs_s(&i, *AssetPath, static_cast<size_t>(MAX_PATH), reinterpret_cast<char*>(path), static_cast<size_t>(MAX_PATH));
	//auto lastSlash = wcsrchr(*AssetPath, L'\\');
	//(*AssetPath)[(wcslen(*AssetPath) - wcslen(lastSlash) + 1)] = '\0';
	//size_t i1;
	//AssetPathStr = std::make_shared<char*>(reinterpret_cast<char*>(malloc(MAX_PATH * sizeof(char))));
	//wcstombs_s(&i1, *AssetPathStr, static_cast<size_t>(MAX_PATH), *AssetPath, static_cast<size_t>(MAX_PATH));
}

PathManager::~PathManager()
{
}

//void PathManager::AppendWString(const wchar_t* wcs0, const wchar_t* wcs1,  wchar_t* result) const
//{
//	auto wstr0 = std::wstring(wcs0);
//	auto wstr1 = std::wstring(wcs1);
//
//	const std::wstring pathWStr = std::wstring(wstr0 + wstr1);
//	const wchar_t* path = pathWStr.data();
//	result = path;
//}
