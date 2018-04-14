#pragma once
#include <memory>
#include "DeviceResources.h"
#include "PlayerRenderer.h"
#include "TerrainRenderer.h"
#include "RootSignatureManager.h"
#include "ModelViewProjectionManager.h"
#include "RendererRequiredStructures.h"


class GameMain
{
public:
	GameMain();
	~GameMain();
	void CreateRenderers(const std::shared_ptr<DX::DeviceResources>& deviceResources);
	void Update() const;
	void Render() const;
	void OnKeyDown(UINT key) const;
	void OnKeyUp(UINT key) const;
	void OnMouseMoved(float x, float y) const;

	void OnWindowSizeChanged() const;
	void OnSuspending() const;
	void OnResuming() const;
	void OnDeviceRemoved() const;

private:
	std::shared_ptr<RootSignatureManager> CreateRootSignatures(const std::shared_ptr<DX::DeviceResources>& deviceResources) const;
	void RunCommandLists();
	std::shared_ptr<ModelViewProjectionManager> CreateCameraHelper() const;
	std::unique_ptr<PlayerRenderer> m_playerRenderer;
	std::unique_ptr<TerrainRenderer> m_terrainRenderer;
};

