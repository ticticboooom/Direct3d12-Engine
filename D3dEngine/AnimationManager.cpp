#include "stdafx.h"
#include "AnimationManager.h"
#include "ModelLoader.h"
/**
 * @brief Construct a new Animation Manager:: Animation Manager object
 * 
 * @param animations the multi-dimentional vector which represents a models animation on each bone
 */
AnimationManager::AnimationManager(std::shared_ptr<std::vector<std::vector<std::vector<XMMATRIX>>>> animations)
	: m_animations(animations)
{
	
}
/**
 * @brief Destroy the Animation Manager:: Animation Manager object
 * 
 */
AnimationManager::~AnimationManager() {

}
/**
 * @brief Gets the final transform of animation per bone for a given animation and time of that animation
 * 
 * @param index the bone index
 * @param time the time in animation
 * @return std::vector<XMFLOAT4X4> 
 */
std::vector<XMFLOAT4X4> AnimationManager::GetPositioninAnim(const int index, int time) {
	auto transforms = std::vector<XMFLOAT4X4>();
	// gets the bones animation
	auto animation = (*m_animations)[index];
	for (auto& bone : animation) {
		// gets the final transform and puts into float4x4 to be used in cbv
		auto newTransform = XMMatrixTranspose(bone[time]);
		auto float4x4 = XMFLOAT4X4{};
		XMStoreFloat4x4(&float4x4, newTransform);
		transforms.push_back(float4x4);
	}
	return transforms;
}