#pragma once
#include "Renderer.h"
#include "Node.h"
class TestRenderer : public Renderer
{
public:
	TestRenderer();
	~TestRenderer();

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
private:
	std::shared_ptr<Node> m_playerNode;
	std::shared_ptr<Node> m_cameraNode;
	std::shared_ptr<Node> m_terrainNode;
};

