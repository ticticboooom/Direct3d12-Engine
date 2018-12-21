#include "stdafx.h"
#include "StaticMeshComponent.h"

/**
 * @brief Construct a new Static Mesh Component:: Static Mesh Component object
 * 	creates a non-animated mesh
 * 
 * @param filename 
 */
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
void StaticMeshComponent::Init()
{
	Mesh::Init();
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

void StaticMeshComponent::CreateDeviceDependentResources()
{
	Mesh::CreateDeviceDependentResources();
}
