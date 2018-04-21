#include "stdafx.h"
#include "RendererManager.h"
#include "UtilityRenderer.h"
#include "CommonObjects.h"

RendererManager::RendererManager()
{
	m_renderers = ComponentManager();
	AddRenderer(std::make_shared<UtilityRenderer>());
}


RendererManager::~RendererManager()
{
}

void RendererManager::CreateRenderers()
{
	auto indexOffset = 0u;
	InitRootSignatureParameters(indexOffset);
	auto heapOffset = 0u;
	auto descHeapManager = std::make_shared<DescriptorHeapManager>(DX::c_frameCount * 2 + 5, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, CommonObjects::m_deviceResources);
	std::shared_ptr<PSOManager> pso;
	std::shared_ptr<CommandListManager> cmdManager;
	Init(&cmdManager, descHeapManager, &heapOffset, &pso);
}

int RendererManager::InitRootSignatureParameters(int indexOffset)
{
	auto signatureManager = CreateRootSignatures(CommonObjects::m_deviceResources);

	CommonObjects::m_rootSignatureManager = signatureManager;

	indexOffset = m_renderers.InitRootSignatureParameters(indexOffset);

	for (int i = 0; i < CommonObjects::m_rootSignatureManager->GetSignatureCount(); i++)
	{
		(*CommonObjects::m_rootSignatureManager.get())[i]->Finalize(L"Root Signature " + std::to_wstring(i),
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS);
	}
	return indexOffset;
}

void RendererManager::Init(std::shared_ptr<CommandListManager>* commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT * descOffset, std::shared_ptr<PSOManager>* pso)
{
	m_renderers.Init(commandListManager, descriptorHeapManager, descOffset, pso);
	m_commandListManager = *commandListManager;
	(*commandListManager)->CloseAndExcecute();
	CommonObjects::m_deviceResources->WaitForGpu();
}

void RendererManager::Update()
{
	m_renderers.Update();
}

void RendererManager::Render()
{
	m_renderers.Render();
	m_commandListManager->CloseAndExcecute();
	CommonObjects::m_deviceResources->WaitForGpu();
}

void RendererManager::OnKeyDown(UINT key)
{
	m_renderers.OnKeyDown(key);
}

void RendererManager::OnKeyUp(UINT key)
{
	m_renderers.OnKeyUp(key);
}

void RendererManager::OnMouseMoved(float x, float y)
{
	m_renderers.OnMouseMoved(x, y);
}

void RendererManager::OnDeviceRemoved()
{
	m_renderers.OnDeviceRemoved();
}

void RendererManager::CreateWindowSizeDependentResources()
{
	m_renderers.CreateWindowSizeDependentResources();
}

void RendererManager::CreateDeviceDependentResoures()
{
	m_renderers.CreateDeviceDependentResoures();
}

void RendererManager::AddRenderer(std::shared_ptr<Renderer> renderer)
{
	m_renderers.AddComponent(renderer);
}

std::shared_ptr<RootSignatureManager> RendererManager::CreateRootSignatures(
	const std::shared_ptr<DX::DeviceResources>& deviceResources) const
{
	std::shared_ptr<RootSignatureManager> signatureManager = std::make_shared<RootSignatureManager>();
	signatureManager->AddSignature(deviceResources, 0, 1);
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