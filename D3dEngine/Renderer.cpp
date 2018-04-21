#include "stdafx.h"
#include "Renderer.h"


Renderer::Renderer() : Component()
{
	m_nodeManager = ComponentManager();
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

void Renderer::Init(std::shared_ptr<CommandListManager>* commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT * descOffset, std::shared_ptr<PSOManager>* pso)
{
	m_nodeManager.Init(commandListManager, descriptorHeapManager, descOffset, pso);
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

void Renderer::CreateDeviceDependentResoures()
{
	m_nodeManager.CreateDeviceDependentResoures();
}
