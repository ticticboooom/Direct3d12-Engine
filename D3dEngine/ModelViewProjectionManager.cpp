#include "stdafx.h"
#include "ModelViewProjectionManager.h"


/**
 * @brief Construct a new Model View Projection Manager:: Model View Projection Manager object
 * @detail This will store the Model View and Perspective Matrices and will convert them to Constant buffer safe Data 
 * 
 */
ModelViewProjectionManager::ModelViewProjectionManager()
{
}

/**
 * @brief Destroy the Model View Projection Manager:: Model View Projection Manager object
 * 
 */
ModelViewProjectionManager::~ModelViewProjectionManager()
{
}

/**
 * @brief store a matrix in 1 of the 3 types
 * 
 * @param type integer of representing matrix (0 = model, 1 = view, 3 = perspective)
 * @param mat the matrix to store
 */
void ModelViewProjectionManager::SetMatrix(UINT type, XMMATRIX mat)
{
	if (type == 0) {
		m_model = mat;
	}
	else if (type == 1) {
		m_view = mat;
	}
	else if (type == 2) {
		m_projection = mat;
	}
}

/**
 * @brief store a matrix in 1 of the 3 types
 * 
 * @param type integer of representing matrix to get (0 = model, 1 = view, 3 = perspective)
 */
XMMATRIX ModelViewProjectionManager::GetMatrix(UINT type)
{
	if (type == 0) {
		return m_model;
	}
	else if (type == 1) {
		return m_view;
	}
	else if (type == 2) {
		return m_projection;
	}
}

/**
 * @brief turns the Matrices into Constant buffer usable data.
 * 
 * @return Structures::ModelViewProjectionConstantBuffer 
 */
Structures::ModelViewProjectionConstantBuffer ModelViewProjectionManager::GetCbvData()
{
	Structures::ModelViewProjectionConstantBuffer value;

	XMStoreFloat4x4(&value.model, m_model);
	XMStoreFloat4x4(&value.view, m_view);
	XMStoreFloat4x4(&value.projection, m_projection);

	return value;
}
