#pragma once
#include "Component.h"
class LifeComponent : public Component
{
public:
	LifeComponent();
	~LifeComponent();

	// Inherited via 

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

	void SetLife(float value) { m_life = value; };
	float GetLife() const { return m_life; };
	static std::vector<LifeComponent*> m_lives;
private:
	float m_life;

};

