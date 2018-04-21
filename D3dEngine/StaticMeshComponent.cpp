#include "stdafx.h"
#include "StaticMeshComponent.h"

StaticMeshComponent::StaticMeshComponent(std::string filename) : Mesh(filename, false)
{
}


StaticMeshComponent::~StaticMeshComponent()
{
}

int StaticMeshComponent::InitRootSignatureParameters(int indexOffset)
{
	return Mesh::InitRootSignatureParameters(indexOffset);
}
void StaticMeshComponent::Init(std::shared_ptr<CommandListManager>* commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT * descOffset, std::shared_ptr<PSOManager>* pso)
{
	Mesh::Init(commandListManager, descriptorHeapManager, descOffset, pso);
}

void StaticMeshComponent::Update()
{
	Mesh::Update();
}

void StaticMeshComponent::Render()
{
	Mesh::Render();
}

void StaticMeshComponent::OnKeyDown(UINT key)
{
	Mesh::OnKeyDown(key);
}

void StaticMeshComponent::OnKeyUp(UINT key)
{
	Mesh::OnKeyUp(key);
}

void StaticMeshComponent::OnMouseMoved(float x, float y)
{
	Mesh::OnMouseMoved(x, y);
}

void StaticMeshComponent::OnDeviceRemoved()
{
	Mesh::OnDeviceRemoved();
}

void StaticMeshComponent::CreateWindowSizeDependentResources()
{
	Mesh::CreateWindowSizeDependentResources();
}

void StaticMeshComponent::CreateDeviceDependentResoures()
{
	Mesh::CreateDeviceDependentResoures();
}
