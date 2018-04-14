#pragma once
#include "stdafx.h"
#include "Structures.h"
class ModelViewProjectionManager
{
public:
	D3DENGINE_API ModelViewProjectionManager();
	D3DENGINE_API ~ModelViewProjectionManager();

	D3DENGINE_API void SetMatrix(UINT type, XMMATRIX mat);
	D3DENGINE_API XMMATRIX GetMatrix(UINT type);

	D3DENGINE_API Structures::ModelViewProjectionConstantBuffer GetCbvData();
	
private:
	XMMATRIX m_model;
	XMMATRIX m_view;
	XMMATRIX m_projection;
};

