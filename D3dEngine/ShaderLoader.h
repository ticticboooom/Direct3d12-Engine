#pragma once
#include "Structures.h"

class ShaderLoader
{
public:
	D3DENGINE_API static Structures::ShaderData GetShaderFromFile(const wchar_t*  fileName);
};
