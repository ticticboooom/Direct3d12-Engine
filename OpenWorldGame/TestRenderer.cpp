#include "TestRenderer.h"
#include "SkeletalMeshComponent.h"
#include "PathManager.h"
#include "CameraComponent.h"
#include "TerrainComponent.h"
#include "TerrainCollisionComponent.h"
#include "BoxCollider.h"
#include "PhysicsComponent.h"


TestRenderer::TestRenderer() : Renderer(),
counter(0)
{
	m_playerNode = std::make_shared<Node>();
	m_nodeManager.AddComponent(m_playerNode);

	//player
	auto pathManager = PathManager();
	auto playerMesh = std::make_shared<SkeletalMeshComponent>(pathManager.GetAssetPathStr() + std::string("Character.coxl"));
	playerMesh->UseTexture(L"Character.png");
	m_playerNode->AddComponent(playerMesh);



	auto physicsComponent = std::make_shared<PhysicsComponent>();
	m_playerNode->AddComponent(physicsComponent);

	auto movementComponent = std::make_shared<InputMovementComponent>();
	m_playerNode->AddComponent(movementComponent);

	auto terrainCollisionComponent = std::make_shared<TerrainCollisionComponent>();
	m_playerNode->AddComponent(terrainCollisionComponent);

	auto boxCollider = std::make_shared<BoxCollider>();
	m_playerNode->AddComponent(boxCollider);
	boxCollider->InitCollider(BoundingBox{ XMFLOAT3{0,0,0}, XMFLOAT3{1,2.4f,1} });



	//camera
	m_cameraNode = std::make_shared<Node>();
	m_nodeManager.AddComponent(m_cameraNode);
	auto camera = std::make_shared<CameraComponent>(movementComponent);
	m_cameraNode->AddComponent(camera);
	auto terrainCollisionComponentForCam = std::make_shared<TerrainCollisionComponent>();
	m_cameraNode->AddComponent(terrainCollisionComponentForCam);

	//terrain
	m_terrainNode = std::make_shared<Node>();
	m_nodeManager.AddComponent(m_terrainNode);
	auto terrain = std::make_shared<TerrainComponent>();
	m_terrainNode->AddComponent(terrain);
	terrain->UseTexture(L"terrain.png");

	m_otherNode = std::make_shared<Node>();
	m_nodeManager.AddComponent(m_otherNode);
	auto skeletalMeshComponent = std::make_shared<SkeletalMeshComponent>(pathManager.GetAssetPathStr() + std::string("Character.coxl"));
	skeletalMeshComponent->UseTexture(L"Character.png");
	m_otherNode->AddComponent(skeletalMeshComponent);


	auto physicsComponent1 = std::make_shared<PhysicsComponent>();
	m_otherNode->AddComponent(physicsComponent1);


	auto terrainCollisionComponent1 = std::make_shared<TerrainCollisionComponent>();
	m_otherNode->AddComponent(terrainCollisionComponent1);

	auto boxCollider1 = std::make_shared<BoxCollider>();
	m_otherNode->AddComponent(boxCollider1);
	boxCollider1->InitCollider(BoundingBox{ XMFLOAT3{ 0,0,0 }, XMFLOAT3{ 1,2.4f,1 } });
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
	if (counter == 0) {
		counter = 100;
		auto meshComp = m_playerNode->GetComponentManager()->GetComponent(typeid(SkeletalMeshComponent).name());
		auto skeletalMesh = (SkeletalMeshComponent*)meshComp.get();
		skeletalMesh->SetAnimInUse(animation);
		animation = !animation;
	}
	counter--;
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
