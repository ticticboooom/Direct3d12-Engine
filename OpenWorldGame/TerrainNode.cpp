#include "TerrainNode.h"

TerrainNode::TerrainNode() : Node()
{
	auto terrain = std::make_shared<TerrainComponent>();
	AddComponent(terrain);
	terrain->UseTexture(L"terrain.png");

}


TerrainNode::~TerrainNode()
{
}

int TerrainNode::InitRootSignatureParameters(int indexOffset)
{
	return Node::InitRootSignatureParameters(indexOffset);
}

void TerrainNode::Init()
{
	Node::Init();
}

void TerrainNode::Update()
{
	Node::Update();
}

void TerrainNode::Render()
{
	Node::Render();
}

void TerrainNode::OnKeyDown(UINT key)
{
	Node::OnKeyDown(key);
}

void TerrainNode::OnKeyUp(UINT key)
{
	Node::OnKeyUp(key);
}

void TerrainNode::OnMouseMoved(float x, float y)
{
	Node::OnMouseMoved(x, y);
}

void TerrainNode::OnDeviceRemoved()
{
	Node::OnDeviceRemoved();
}

void TerrainNode::CreateWindowSizeDependentResources()
{
	Node::CreateWindowSizeDependentResources();
}

void TerrainNode::CreateDeviceDependentResoures()
{
	Node::CreateDeviceDependentResoures();
}
