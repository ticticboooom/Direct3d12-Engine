#include "stdafx.h"
#include "UtilityRenderer.h"
#include "ShaderLoader.h"
#include "PathManager.h"
#include "CommonObjects.h"

bool UtilityRenderer::m_isRootSignatureInitialised = false;
UINT UtilityRenderer::m_rotRootSigIndex = 0;
UINT UtilityRenderer::m_rotHeapIndex = 0;
UtilityRenderer::UtilityRenderer() : Renderer()
{
}

UtilityRenderer::~UtilityRenderer()
{
}

int UtilityRenderer::InitRootSignatureParameters(int indexOffset)
{
	return indexOffset;
}

void UtilityRenderer::Init(std::shared_ptr<CommandListManager>* commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT * descOffset, std::shared_ptr<PSOManager>* pso)
{
	m_cbvSrvHeapManager = descriptorHeapManager;
	auto pathManager = PathManager();
	const auto assetPath = std::wstring(pathManager.GetAssetPath());
	auto VSName = L"PlayerVertexShader.cso";
	auto PSName = L"PlayerPixelShader.cso";
	auto VSPath = assetPath + VSName;
	auto PSPath = assetPath + PSName;
	const auto vertexShader = ShaderLoader::GetShaderFromFile(VSPath.c_str());
	const auto pixelShader = ShaderLoader::GetShaderFromFile(PSPath.c_str());

	static const D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0, 12,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "NORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 20,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	1,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 32,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	2,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 48,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	3,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 64,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	4,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 80,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	5,	DXGI_FORMAT_R32_SINT,			0, 96,	D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
	m_psoManager = std::make_unique<PSOManager>(CommonObjects::m_deviceResources);
	m_psoManager->SetInputLayout({ inputLayout, _countof(inputLayout) });
	m_psoManager->SetSignature((*CommonObjects::m_rootSignatureManager)[0]->GetSignature());
	m_psoManager->SetVS(CD3DX12_SHADER_BYTECODE(vertexShader.shader, vertexShader.size));
	m_psoManager->SetPS(CD3DX12_SHADER_BYTECODE(pixelShader.shader, pixelShader.size));
	m_psoManager->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_psoManager->SetNumRenderTargets(1);
	m_psoManager->SetRTVFormats(CommonObjects::m_deviceResources->GetBackBufferFormat(), 0);
	m_psoManager->SetDSVFormat(CommonObjects::m_deviceResources->GetDepthBufferFormat());
	m_psoManager->Finalise();

	m_commandListManager = std::make_shared<CommandListManager>(CommonObjects::m_deviceResources, m_psoManager->GetState(), D3D12_COMMAND_LIST_TYPE_DIRECT);
	*commandListManager = m_commandListManager;
}

void UtilityRenderer::Update()
{
}

void UtilityRenderer::Render()
{
	CommonObjects::m_deviceResources->WaitForGpu();
	ThrowIfFailed(CommonObjects::m_deviceResources->GetCommandAllocator()->Reset());
	ThrowIfFailed(m_commandListManager->Reset(CommonObjects::m_deviceResources->GetCommandAllocator(), m_psoManager->GetState()));

	m_commandListManager->SetGraphicsRootSignature((*CommonObjects::m_rootSignatureManager)[0]->GetSignature());
	ID3D12DescriptorHeap* ppHeaps[] = { m_cbvSrvHeapManager->Get() };
	m_commandListManager->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	D3D12_VIEWPORT viewport = CommonObjects::m_deviceResources->GetScreenViewport();

	m_commandListManager->SetViewports(1, &viewport);
	m_commandListManager->SetScissorRects(1, &m_scissorRect);

	m_commandListManager->CreateResourceBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	m_commandListManager->ClearSetRenderTargets();

	m_commandListManager->CreateResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
}

void UtilityRenderer::OnKeyDown(UINT key)
{
}

void UtilityRenderer::OnKeyUp(UINT key)
{
}

void UtilityRenderer::OnMouseMoved(float x, float y)
{
}

void UtilityRenderer::OnDeviceRemoved()
{
}

void UtilityRenderer::CreateWindowSizeDependentResources()
{
	// Gets thye inner window (output size) form \class DeviceResources
	const auto outputSize = CommonObjects::m_deviceResources->GetOutputSize();
	// Gets aspect ratio from output size
	const float aspectRatio = outputSize.x / outputSize.y;
	// Creates the Field of View for the perspective
	float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;

	// Gets the viewport structure from \class Device Resources
	const auto viewport = CommonObjects::m_deviceResources->GetScreenViewport();
	// Sets \property m_scissorRect with the viewport width and height
	m_scissorRect = { 0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height) };
	// Checks the aspect ratio to see if the FOV angle needs to be changed
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}
	// Sets the perspective matrix using the \var fovAngleY \var aspectRatio and the constant vlaues which represent the Z Cut off (near then far)
	const auto perspectiveMatrix = DirectX::XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		10000.0f
	);

	// Screen orientation
	static const DirectX::XMFLOAT4X4 Rotation0(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	// Turns into an XMMATRIX
	const auto orientationMatrix = XMLoadFloat4x4(&Rotation0);

	// Sets the perspective matrix in the \class ModelViewProjectionManager
	XMStoreFloat4x4(&m_projection, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));
}

void UtilityRenderer::CreateDeviceDependentResoures()
{
}
