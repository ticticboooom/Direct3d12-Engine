#pragma once
#include "Component.h"
class D3DENGINE_API CylinderCollider : public Component
{
public:
	CylinderCollider();
	~CylinderCollider();


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

	virtual void CreateDeviceDependentResources() override;
	void InitCollider(Structures::BoundingCylinder collider);
	Structures::BoundingCylinder GetCollider() const { return m_collider; };
private:
	void Collide();
	XMVECTOR GetPushBack(Structures::BoundingCylinder coll);
	Structures::BoundingCylinder m_collider; /// The collider
	static std::vector<Structures::BoundingCylinder> m_allColliders;
	UINT m_colliderIndex;
};

