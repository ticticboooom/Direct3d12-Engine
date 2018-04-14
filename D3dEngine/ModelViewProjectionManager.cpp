#include "stdafx.h"
#include "ModelViewProjectionManager.h"



ModelViewProjectionManager::ModelViewProjectionManager()
{
}


ModelViewProjectionManager::~ModelViewProjectionManager()
{
}

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

Structures::ModelViewProjectionConstantBuffer ModelViewProjectionManager::GetCbvData()
{
	Structures::ModelViewProjectionConstantBuffer value;

	XMStoreFloat4x4(&value.model, m_model);
	XMStoreFloat4x4(&value.view, m_view);
	XMStoreFloat4x4(&value.projection, m_projection);

	return value;
}
