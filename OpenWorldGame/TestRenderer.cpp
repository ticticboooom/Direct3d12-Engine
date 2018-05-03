#include "TestRenderer.h"

#include "PlayerNode.h"
#include "PlayerCameraNode.h"
#include "TerrainNode.h"
#include "EnemyNode.h"
#include "InputMovementComponent.h"

TestRenderer::TestRenderer() : Renderer()
{
	m_playerNode = std::make_shared<PlayerNode>();
	m_nodeManager.AddComponent(m_playerNode);

	auto playerMovementComp = (InputMovementComponent*)(m_playerNode->GetComponentManager()->GetComponent(typeid(InputMovementComponent).name()).get());
	auto playerMovementComponent = std::shared_ptr<InputMovementComponent>(playerMovementComp);

	m_cameraNode = std::make_shared<PlayerCameraNode>(playerMovementComponent);
	m_nodeManager.AddComponent(m_playerNode);
	
	m_terrainNode = std::make_shared<TerrainNode>();
	m_nodeManager.AddComponent(m_terrainNode);
	
	m_otherNode = std::make_shared<EnemyNode>();
	m_nodeManager.AddComponent(m_otherNode);
	
}


TestRenderer::~TestRenderer()
{
}

int TestRenderer::InitRootSignatureParameters(int indexOffset)
{
	return Renderer::InitRootSignatureParameters(indexOffset);
}

void TestRenderer::Init()
{
	Renderer::Init();
}

void TestRenderer::Update()
{

	Renderer::Update();
}

void TestRenderer::Render()
{
	Renderer::Render();
}

void TestRenderer::OnKeyDown(UINT key)
{
	Renderer::OnKeyDown(key);
}

void TestRenderer::OnKeyUp(UINT key)
{
	Renderer::OnKeyUp(key);
}

void TestRenderer::OnMouseMoved(float x, float y)
{
	Renderer::OnMouseMoved(x, y);
}

void TestRenderer::OnDeviceRemoved()
{
	Renderer::OnDeviceRemoved();
}

void TestRenderer::CreateWindowSizeDependentResources()
{
	Renderer::CreateWindowSizeDependentResources();
}

void TestRenderer::CreateDeviceDependentResoures()
{
	Renderer::CreateDeviceDependentResoures();
}
