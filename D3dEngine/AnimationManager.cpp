#include "stdafx.h"
#include "AnimationManager.h"
#include "ModelLoader.h"
AnimationManager::AnimationManager(std::shared_ptr<std::vector<std::vector<std::vector<XMMATRIX>>>> animations)
	: m_animations(animations)
{
	
}
AnimationManager::~AnimationManager() {

}
std::vector<XMFLOAT4X4> AnimationManager::GetPositioninAnim(const int index, int time) {
	auto transforms = std::vector<XMFLOAT4X4>();
	auto animation = (*m_animations)[index];
	for (auto& bone : animation) {
		auto newTransform = XMMatrixTranspose(bone[time]);
		auto float4x4 = XMFLOAT4X4{};
		XMStoreFloat4x4(&float4x4, newTransform);
		transforms.push_back(float4x4);
	}
	return transforms;
}