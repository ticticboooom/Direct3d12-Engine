#pragma once
#include "Component.h"
#include "InputMovementComponent.h"
#include "SkeletalMeshComponent.h"
#include "AttackComponent.h"

class PlayerAnimationControllerComponent : public Component
{
public:
	PlayerAnimationControllerComponent();
	~PlayerAnimationControllerComponent();

	// Inherited via Component
	virtual int InitRootSignatureParameters(int indexOffset) override;
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void OnKeyDown(UINT key) override;
	virtual void OnKeyUp(UINT key) override;
	virtual void OnMouseMoved(float x, float y) override;
	virtual void OnDeviceRemoved() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void CreateDeviceDependentResoures() override;
private:
	bool m_prevIdleState;
	bool m_prevRunningState;
	bool m_prevHittingState;
	std::shared_ptr<InputMovementComponent> m_movementComp;
	std::shared_ptr <SkeletalMeshComponent> m_meshComponent;
	std::shared_ptr <AttackComponent> m_AttackComponent;
};

