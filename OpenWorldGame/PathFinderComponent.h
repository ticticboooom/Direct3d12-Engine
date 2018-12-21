#pragma once
#include "Component.h"
#include "EnemyAttackComponent.h"
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
	virtual void CreateDeviceDependentResources() override;
	static void SetPlayerTransform(Structures::Transform playerTransform) { s_playerTransform = playerTransform; };
private:
	void Move();
	void InitPoints();
	std::vector<XMVECTOR> m_points;
	std::shared_ptr<EnemyAttackComponent> m_enemyAttackComponent;
	UINT m_currentPointIndex;
	UINT m_intervalIndex;
	const float c_multiplyer = 0.2;
	const float c_distanceFromPlayer = 25.f;
	const float c_distanceFromPlayerWhenClocked = 55.f;
	bool m_isPlayerClocked;
	static Structures::Transform s_playerTransform;
};

