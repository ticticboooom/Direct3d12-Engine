#pragma once
#include "Component.h"
#include "InputMovementComponent.h"
#include "SkeletalMeshComponent.h"
class AttackComponent : public Component
{
public:
	AttackComponent();
	~AttackComponent();

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
	void AttckOther();
	UINT counter;
	UINT animation;
	bool m_prevIdleState;
	std::shared_ptr<InputMovementComponent> m_movementComp;
	std::shared_ptr <SkeletalMeshComponent> m_meshComponent;
	bool m_isHitting;
	UINT m_hitStartFrame;
};

