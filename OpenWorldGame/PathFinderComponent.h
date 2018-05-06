#pragma once
#include "Component.h"

class PathFinderComponent : public Component
{
public:
	PathFinderComponent();
	~PathFinderComponent();
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
	void Move();
	void InitPoints();
	std::vector<XMVECTOR> m_points;
	UINT m_currentPointIndex;
	UINT m_intervalIndex;
	const float c_multiplyer = 0.2;
};

