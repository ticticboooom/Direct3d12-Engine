#pragma once
#include "Node.h"
#include "SkeletalMeshComponent.h"
#include "PathManager.h"
#include "CameraComponent.h"
#include "TerrainComponent.h"
#include "TerrainCollisionComponent.h"
#include "BoxCollider.h"
#include "PhysicsComponent.h"
#include "CylinderCollider.h"

class PlayerNode : public Node
{
public:
	PlayerNode();
	~PlayerNode();
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
	UINT counter;
	UINT animation;
};

