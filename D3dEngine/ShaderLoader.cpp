#include "stdafx.h"
#include "ShaderLoader.h"
#include <string>
#include "DirectXHelper.h"

Structures::ShaderData ShaderLoader::GetShaderFromFile(const wchar_t* fileName)
{
	Structures::ShaderData data;
	ReadDataFromFile(fileName, &data.shader, &data.size);
	return data;
}
