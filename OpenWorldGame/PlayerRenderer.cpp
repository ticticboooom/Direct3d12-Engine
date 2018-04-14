#include "PlayerRenderer.h"
#include "Structures.h"
#include "RendererRequiredStructures.h"
#include "DirectXHelper.h"
#include "ShaderLoader.h"
#include "VertexBufferManager.h"
#include "IndexBufferManager.h"
#include "TextureResourceManager.h"
#include "BinaryModelLoader.h"
#include "PathManager.h"
#include "PhysicsHelper.h"

PlayerRenderer::PlayerRenderer(Structures::RequiredRendererObjects objects) :
	m_deviceResources(objects.deviceResources),
	m_rootSignatureManager(objects.rootSignatureManager),
	m_cameraHelper(objects.cameraHelper),
	m_cbvDescriptorSize(0),
	m_indexCount(0),
	m_scissorRect({ 0,0,0,0 }),
	seed(0),
	m_playerPosition({10,0,10})
{

}

PlayerRenderer::~PlayerRenderer()
{
}

int PlayerRenderer::InitRootSignatureParameters(int indexOffset)
{
	(*(*m_rootSignatureManager)[0])[0 + indexOffset].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 0, 1, D3D12_SHADER_VISIBILITY_VERTEX);
	(*(*m_rootSignatureManager)[0])[1 + indexOffset].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	(*(*m_rootSignatureManager)[0])[2 + indexOffset].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1, D3D12_SHADER_VISIBILITY_VERTEX);
	(*(*m_rootSignatureManager)[0])[3 + indexOffset].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 2, 1, D3D12_SHADER_VISIBILITY_VERTEX);
	m_rootSigIndexOffset = indexOffset;
	return indexOffset;
}

void PlayerRenderer::RunCommandLists()
{

}

std::shared_ptr<CommandListManager> PlayerRenderer::Init(std::shared_ptr<CommandListManager> commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT* descOffset, std::shared_ptr<PSOManager>* pso)
{
	m_descHeapOffset = *descOffset;
	m_cbvSrvHeapManager = descriptorHeapManager;
	CreateDeviceDependentResoures();
	CreateWindowSizeDependentResources();
	*descOffset += 5;
	*pso = m_psoManager;
	return m_commandListManager;
}

void PlayerRenderer::Update()
{
	{
		auto newY = 0.0f;
		auto pos3 = XMFLOAT3{};
		XMStoreFloat3(&pos3, m_playerPosition);
		auto terrainCollSuccess = m_terrainCollisionHelper->GetNewYPos(pos3, &newY);

		collider.Center = pos3;
		m_physicsHelper->Update(newY, &m_playerPosition, m_cameraHelper->GetIsJumping(), collider);

		m_cameraHelper->Update(&m_playerPosition);
		auto camPos = m_cameraHelper->GetCameraPosition();
		auto cbvData = m_cameraHelper->GetCbvData();
		auto rot = m_cameraHelper->GetCameraRotYaw();
		rot -= XM_PI / 2;

		auto mat = XMMatrixTranspose(XMMatrixAffineTransformation(XMVectorSet(1, 1, 1, 0), XMVectorSet(0, 0, 0, 0), XMQuaternionRotationRollPitchYaw(0, rot, 0), (m_playerPosition)));
	
		XMStoreFloat4x4(&cbvData.model, mat);
		UINT8* destination = m_mvpConstantBufferManager->GetMappedData() + (m_deviceResources->GetCurrentFrameIndex() * m_mvpConstantBufferManager->GetAlignedSize());
		std::memcpy(destination, &cbvData, sizeof(cbvData));

		auto rotator = XMFLOAT4X4{};
		XMStoreFloat4x4(&rotator, XMMatrixTranspose(XMMatrixAffineTransformation(XMVectorSet(1, 1, 1, 0), { 0,0,0,1 }, XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), -1.59), XMVectorSet(0, 0, 0, 0))));
		std::memcpy(m_rotatorConstantBuffer->GetMappedData(), &rotator, sizeof(XMFLOAT4X4));

		auto frame = m_animationManager->GetPositioninAnim(0, m_frame);
		std::memcpy(m_animationConstantBufferManager->GetMappedData(), frame.data(), sizeof(XMFLOAT4X4) * frame.size());
		m_frame++;
		if (m_frame >= m_animationManager->GetFrameCount(0, 0))
		{
			m_frame = 0;
		}
	}
}

void PlayerRenderer::Render()
{
	m_deviceResources->WaitForGpu();
	ThrowIfFailed(m_deviceResources->GetCommandAllocator()->Reset());
	ThrowIfFailed(m_commandListManager->Reset(m_deviceResources->GetCommandAllocator(), m_psoManager->GetState()));

	m_commandListManager->SetGraphicsRootSignature((*m_rootSignatureManager)[0]->GetSignature());
	ID3D12DescriptorHeap* ppHeaps[] = { m_cbvSrvHeapManager->Get() };
	m_commandListManager->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	UINT rootSigIds[] = { 0, 1, 2, 3 };
	UINT heapIds[] = { m_deviceResources->GetCurrentFrameIndex() + m_descHeapOffset, 3 + m_descHeapOffset, 4 + m_descHeapOffset, 9 };
	m_cbvSrvHeapManager->Render(4, rootSigIds, heapIds, m_commandListManager);

	D3D12_VIEWPORT viewport = m_deviceResources->GetScreenViewport();
	m_commandListManager->SetViewports(1, &viewport);
	m_commandListManager->SetScissorRects(1, &m_scissorRect);

	m_commandListManager->CreateResourceBarrier(D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_commandListManager->ClearSetRenderTargets();

	m_commandListManager->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_commandListManager->SetVertexBuffers(0, 1, &m_playerVertexBufferView);
	m_commandListManager->SetIndexBuffer(&m_playerIndexBufferView);
	m_commandListManager->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);

	m_commandListManager->CreateResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
}

void PlayerRenderer::OnKeyDown(UINT key)
{
	m_cameraHelper->OnKeyDown(key);
}

void PlayerRenderer::OnKeyUp(UINT key)
{
	m_cameraHelper->OnKeyUp(key);
}

void PlayerRenderer::OnMouseMoved(float x, float y)
{
	m_cameraHelper->OnMouseMoved(x, y);
}

void PlayerRenderer::OnDeviceRemoved()
{
}

void PlayerRenderer::CreateDeviceDependentResoures()
{
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
	m_psoManager = std::make_unique<PSOManager>(m_deviceResources);
	m_psoManager->SetInputLayout({ inputLayout, _countof(inputLayout) });
	m_psoManager->SetSignature((*m_rootSignatureManager)[0]->GetSignature());
	m_psoManager->SetVS(CD3DX12_SHADER_BYTECODE(vertexShader.shader, vertexShader.size));
	m_psoManager->SetPS(CD3DX12_SHADER_BYTECODE(pixelShader.shader, pixelShader.size));
	m_psoManager->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	m_psoManager->SetNumRenderTargets(1);
	m_psoManager->SetRTVFormats(m_deviceResources->GetBackBufferFormat(), 0);
	m_psoManager->SetDSVFormat(m_deviceResources->GetDepthBufferFormat());
	m_psoManager->Finalise();

	auto playerModelLoader = std::make_unique<BinaryModelLoader>((std::string(pathManager.GetAssetPathStr()) + std::string("Character.coxl")).c_str(), true);
	auto playerPtr = playerModelLoader->GetFinalData();
	auto player = *playerPtr;
	m_indexCount = player.indices->size();
	m_animationManager = std::make_unique<AnimationManager>(player.animations);

	m_commandListManager = std::make_shared<CommandListManager>(m_deviceResources, m_psoManager->GetState(), D3D12_COMMAND_LIST_TYPE_DIRECT);
	auto playerInds = std::make_shared<std::vector<unsigned int>>(player.indices->begin(), player.indices->end());
	m_playerVertexBufferManager = std::make_unique<VertexBufferManager>(player.vertices, m_deviceResources, m_commandListManager);
	m_playerIndexBufferManager = std::make_unique<IndexBufferManager>(playerInds, m_deviceResources, m_commandListManager);

	player.indices->clear();
	player.vertices->clear();


	m_cbvDescriptorSize = m_cbvSrvHeapManager->GetDescriptorSize();
	m_animationConstantBufferManager = std::make_unique<ConstantBufferManager<XMFLOAT4X4>>(1, m_animationConstantBufferManager->GetAlignedSize() * m_animationManager->GetPositioninAnim(0, 0).size(), m_deviceResources, m_commandListManager);
	m_animationConstantBufferManager->CreateBufferDesc(m_animationConstantBufferManager->GetAlignedSize() * m_animationManager->GetPositioninAnim(0, 0).size(), 4 + m_descHeapOffset, m_cbvSrvHeapManager, m_cbvDescriptorSize);

	m_mvpConstantBufferManager = std::make_unique<ConstantBufferManager<Structures::ModelViewProjectionConstantBuffer>>(DX::c_frameCount, m_mvpConstantBufferManager->GetAlignedSize(), m_deviceResources, m_commandListManager);
	for (int n = 0; n < DX::c_frameCount; n++)
	{
		m_mvpConstantBufferManager->CreateBufferDesc(m_mvpConstantBufferManager->GetAlignedSize(), n + m_descHeapOffset, m_cbvSrvHeapManager, m_cbvDescriptorSize);
	}

	m_rotatorConstantBuffer = std::make_unique<ConstantBufferManager<XMFLOAT4X4>>(1, m_rotatorConstantBuffer->GetAlignedSize(), m_deviceResources, m_commandListManager);
	m_rotatorConstantBuffer->CreateBufferDesc(m_rotatorConstantBuffer->GetAlignedSize(), 9, m_cbvSrvHeapManager, m_cbvDescriptorSize);

	const auto textureManager = new TextureResourceManager(std::wstring(pathManager.GetAssetPath() + std::wstring(L"Character.png")).c_str(), m_deviceResources, m_commandListManager);
	textureManager->CreateSRVFromTextureResource(m_cbvSrvHeapManager->Get(), m_cbvDescriptorSize, 3 + m_descHeapOffset);


	m_playerVertexBufferView = m_playerVertexBufferManager->CreateVertexBufferView();
	m_playerIndexBufferView = m_playerIndexBufferManager->CreateIndexBufferView();
	playerInds->clear();
	std::vector<unsigned int>().swap(*playerInds);

	player.vertices->clear();
	std::vector<Structures::VertexTexCoordNormal>().swap(*player.vertices);

	player.vertices->clear();
	std::vector<unsigned long>().swap(*player.indices);

	m_terrainCollisionHelper = std::make_shared<TerrainCollisionHelper>();
	m_physicsHelper = std::make_shared<PhysicsHelper>();

	collider = BoundingBox{};
	collider.Center = { 0,0,0 };
	collider.Extents = { 1,2.5,1 };
}

void PlayerRenderer::CreateWindowSizeDependentResources()
{
	m_cameraHelper->OnWindowSizeChanged();
	m_scissorRect = m_cameraHelper->GetScissorRect();
}
