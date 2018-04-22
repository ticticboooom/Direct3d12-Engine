#include "TestRenderer.h"
#include "SkeletalMeshComponent.h"
#include "PathManager.h"
#include "CameraComponent.h"
#include "TerrainComponent.h"
#include "TerrainCollisionComponent.h"
#include "BoxCollider.h"
#include "PhysicsComponent.h"


TestRenderer::TestRenderer() : Renderer()
{
	m_playerNode = std::make_shared<Node>();
	m_nodeManager.AddComponent(m_playerNode);

	auto pathManager = PathManager();
	auto playerMesh = std::make_shared<SkeletalMeshComponent>(pathManager.GetAssetPathStr() + std::string("Character.coxl"), true);
	playerMesh->UseTexture(L"Character.png");
	m_playerNode->AddComponent(playerMesh);


	auto physicsComponent = std::make_shared<PhysicsComponent>();
	m_playerNode->AddComponent(physicsComponent);
	
	auto camera = std::make_shared<CameraComponent>();
	m_playerNode->AddComponent(camera);

	auto terrainCollisionComponent = std::make_shared<TerrainCollisionComponent>();
	m_playerNode->AddComponent(terrainCollisionComponent);

	auto boxCollider = std::make_shared<BoxCollider>();
	m_playerNode->AddComponent(boxCollider);
	boxCollider->InitCollider(BoundingBox{ XMFLOAT3{0,0,0}, XMFLOAT3{1,2.4f,1} });

	
	m_terrainNode = std::make_shared<Node>();
	m_nodeManager.AddComponent(m_terrainNode);
	auto terrain = std::make_shared<TerrainComponent>();
	m_terrainNode->AddComponent(terrain);
	terrain->UseTexture(L"terrain.png");
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
