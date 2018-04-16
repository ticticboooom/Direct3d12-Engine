#pragma once
#include "stdafx.h"
#include "Structures.h"
/**
 * @brief This will store the Model View and Perspective Matrices and will convert them to Constant buffer safe Data 
 * 
 */
class ModelViewProjectionManager
{
public:
	D3DENGINE_API ModelViewProjectionManager();
	D3DENGINE_API ~ModelViewProjectionManager();

	D3DENGINE_API void SetMatrix(UINT type, XMMATRIX mat);
	D3DENGINE_API XMMATRIX GetMatrix(UINT type);

	D3DENGINE_API Structures::ModelViewProjectionConstantBuffer GetCbvData();
	
private:
	XMMATRIX m_model; /// model matrix
	XMMATRIX m_view; /// view matrix
	XMMATRIX m_projection; /// projection / perspective matrix
};

