#include "stdafx.h"
#include "GameMain.h"
#include "PlayerRenderer.h"
#include "Structures.h"


GameMain::GameMain()
{
}


GameMain::~GameMain()
{
}

void GameMain::CreateRenderers(const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	auto cameraHelper = std::make_shared<CameraHelper>(deviceResources);
	auto signatureManager = CreateRootSignatures(deviceResources);
	Structures::RequiredRendererObjects objects(deviceResources, signatureManager, cameraHelper);

	m_playerRenderer = std::make_unique<PlayerRenderer>(objects);
	m_terrainRenderer = std::make_unique<TerrainRenderer>(objects);
	//creating Root Signatures For shared usage
	auto indexOffset = 0;
	indexOffset = m_playerRenderer->InitRootSignatureParameters(indexOffset);
	indexOffset = m_terrainRenderer->InitRootSignatureParameters(indexOffset);

	for (int i = 0; i < signatureManager->GetSignatureCount(); i++)
	{
		(*signatureManager.get())[i]->Finalize(L"Root Signature " + std::to_wstring(i),
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS);
	}
	auto descOffset = 0u;
	auto descHeapManager = std::make_shared<DescriptorHeapManager>(DX::c_frameCount * 2 + 5, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, deviceResources);
	std::shared_ptr<PSOManager> pso;

	auto cmdManager = m_playerRenderer->Init(nullptr, descHeapManager, &descOffset, &pso);
	cmdManager = m_terrainRenderer->Init(cmdManager, descHeapManager, &descOffset, &pso);
	cmdManager->CloseAndExcecute();
	deviceResources->WaitForGpu();
}

void GameMain::Update() const
{
	m_playerRenderer->Update();
	m_terrainRenderer->Update();
}

void GameMain::Render() const
{

	m_playerRenderer->Render();
	m_terrainRenderer->Render();
}

void GameMain::OnKeyDown(const UINT key) const
{
	m_playerRenderer->OnKeyDown(key);
	m_terrainRenderer->OnKeyDown(key);
}

void GameMain::OnKeyUp(const UINT key) const
{
	m_playerRenderer->OnKeyUp(key);
	m_terrainRenderer->OnKeyUp(key);
}

void GameMain::OnMouseMoved(const float x, const float y) const
{
	m_playerRenderer->OnMouseMoved(x, y);
	m_terrainRenderer->OnMouseMoved(x, y);
}

void GameMain::OnWindowSizeChanged() const
{
	m_playerRenderer->CreateWindowSizeDependentResources();
	m_terrainRenderer->CreateWindowSizeDependentResources();
}

void GameMain::OnSuspending() const
{
}

void GameMain::OnResuming() const
{
}

void GameMain::OnDeviceRemoved() const
{
	m_playerRenderer->OnDeviceRemoved();
	m_terrainRenderer->OnDeviceRemoved();
}

std::shared_ptr<RootSignatureManager> GameMain::CreateRootSignatures(
	const std::shared_ptr<DX::DeviceResources>& deviceResources) const
{
	std::shared_ptr<RootSignatureManager> signatureManager = std::make_shared<RootSignatureManager>();
	signatureManager->AddSignature(deviceResources, 4, 1);
	D3D12_SAMPLER_DESC sampler;
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	(*signatureManager)[0]->InitStaticSampler(0, sampler, D3D12_SHADER_VISIBILITY_PIXEL);
	return signatureManager;
}

void GameMain::RunCommandLists()
{
	m_playerRenderer->RunCommandLists();
	m_terrainRenderer->RunCommandLists();
}

