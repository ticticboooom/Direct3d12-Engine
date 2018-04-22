#pragma once
#include "IGameBase.h"
#include "ComponentManager.h"
#include "Renderer.h"

class D3DENGINE_API RendererManager : IGameBase
{
public:
	RendererManager();
	~RendererManager();
	void CreateRenderers();
	// Inherited via IGameBase
	virtual int InitRootSignatureParameters(int indexOffset) override;
	virtual void Init(std::shared_ptr<CommandListManager>* commandListManager, std::shared_ptr<DescriptorHeapManager> descriptorHeapManager, UINT * descOffset, std::shared_ptr<PSOManager>* pso) override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void OnKeyDown(UINT key) override;
	virtual void OnKeyUp(UINT key) override;
	virtual void OnMouseMoved(float x, float y) override;
	virtual void OnDeviceRemoved() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void CreateDeviceDependentResoures() override;
protected:
	void AddRenderer(std::shared_ptr<Renderer> renderer);
private:
	std::shared_ptr<RootSignatureManager> CreateRootSignatures(const std::shared_ptr<DX::DeviceResources>& deviceResources) const;
	ComponentManager m_renderers; /// renderers to be run
	std::shared_ptr<CommandListManager> m_commandListManager;
};

