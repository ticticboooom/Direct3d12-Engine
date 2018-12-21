#include "stdafx.h"
#include "Renderer.h"

/**
 * @brief Construct a new Renderer:: Renderer object
 * 	stores nodes and runs them when reequired
 */
Renderer::Renderer() : Component(), m_nodeManager(false)
{
}


Renderer::~Renderer()
{
	Component::~Component();
}

int Renderer::InitRootSignatureParameters(int indexOffset)
{
	indexOffset = m_nodeManager.InitRootSignatureParameters(indexOffset);
	return indexOffset;
}

void Renderer::Init()
{
	m_nodeManager.Init();
}

void Renderer::Update()
{
	m_nodeManager.Update();
}

void Renderer::Render()
{
	m_nodeManager.Render();
}

void Renderer::OnKeyDown(UINT key)
{
	m_nodeManager.OnKeyDown(key);
}

void Renderer::OnKeyUp(UINT key)
{
	m_nodeManager.OnKeyUp(key);

}

void Renderer::OnMouseMoved(float x, float y)
{
	m_nodeManager.OnMouseMoved(x,y);
}

void Renderer::OnDeviceRemoved()
{
	m_nodeManager.OnDeviceRemoved();
}

void Renderer::CreateWindowSizeDependentResources()
{

	m_nodeManager.CreateWindowSizeDependentResources();
}

void Renderer::CreateDeviceDependentResources()
{
	m_nodeManager.CreateDeviceDependentResources();
}
