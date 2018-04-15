#include "stdafx.h"
#include "PathManager.h"
/**
 * @brief Construct a new Path Manager:: Path Manager object
 *  sets the paths up with the correct value to use later
 */
PathManager::PathManager()
{
	WCHAR path[MAX_PATH];
	GetAssetsPath(path, MAX_PATH);
	AssetPath = std::make_shared<std::wstring>(path);
	AssetPathStr = std::make_shared<std::string>(AssetPath->begin(), AssetPath->end());
	auto lastSlash = wcsrchr(AssetPath->c_str(), L'\\');
	(*AssetPath)[(wcslen(AssetPath->c_str()) - wcslen(lastSlash) + 1)] = '\0';
}

/**
 * @brief Destroy the Path Manager:: Path Manager object
 * 
 */
PathManager::~PathManager()
{
}
