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

	auto playerMovementComp = (m_playerNode->GetComponentManager()->GetComponent(typeid(InputMovementComponent).name()));
	auto playerMovementComponent = std::dynamic_pointer_cast<InputMovementComponent>(playerMovementComp);

	m_cameraNode = std::make_shared<PlayerCameraNode>(playerMovementComponent);
	m_nodeManager.AddComponent(m_cameraNode);
	
	m_terrainNode = std::make_shared<TerrainNode>();
	m_nodeManager.AddComponent(m_terrainNode);
	
	for (auto i = 0; i < 10; i++) {
		auto enemy = std::make_shared<EnemyNode>();
		m_otherNode.push_back(enemy);
		m_nodeManager.AddComponent(enemy);
	}
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
