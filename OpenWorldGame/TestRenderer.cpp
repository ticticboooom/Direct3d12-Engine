#include "TestRenderer.h"
#include "SkeletalMeshComponent.h"
#include "PathManager.h"
#include "CameraComponent.h"
#include "TerrainComponent.h"


TestRenderer::TestRenderer() : Renderer()
{
	m_playerNode = std::make_shared<Node>();
	m_nodeManager.AddComponent(m_playerNode);

	auto pathManager = PathManager();
	auto playerMesh = std::make_shared<SkeletalMeshComponent>(pathManager.GetAssetPathStr() + std::string("Character.coxl"), true);
	playerMesh->UseTexture(L"Character.png");
	m_playerNode->AddComponent(playerMesh);

	auto camera = std::make_shared<CameraComponent>();
	m_playerNode->AddComponent(camera);

	m_terrainNode = std::make_shared<Node>();
	m_nodeManager.AddComponent(m_terrainNode);
	auto terrain = std::make_shared<TerrainComponent>();
	m_terrainNode->AddComponent(terrain);
}


TestRenderer::~TestRenderer()
{
}

int TestRenderer::InitRootSignatureParameters(int indexOffset)
{
	return Renderer::InitRootSignatureParameters(indexOffset);
}

void TestRenderer::Init(std::shared_ptr<CommandListManager>* commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT * descOffset, std::shared_ptr<PSOManager>* pso)
{
	Renderer::Init(commandListManager, descriptorHeapManager, descOffset, pso);
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
