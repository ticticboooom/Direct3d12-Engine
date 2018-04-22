#pragma once
#include "Component.h"
#include <DirectXCollision.h>
class D3DENGINE_API	 PhysicsComponent : public Component
{
public:
	PhysicsComponent();
	~PhysicsComponent();

	// Inherited via Component
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
	float minY;
	bool isRising;
private:
	bool previouslyRising = false; /// was the vector previsouly rising
	float yPos; /// the current y position
	float yPosFallDiff = yPosFallStart; /// fall distance each tick
	const float yPosFallMultiplyer = 1.05f; ///fall speed multiplyer 
	const float yPosFallStart = 0.3f; /// fall speed start
};

