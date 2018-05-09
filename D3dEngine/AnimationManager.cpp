#include "stdafx.h"
#include "AnimationManager.h"
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

std::vector<XMFLOAT4X4> AnimationManager::BlendAnimationsAtPositions(const int index0, const int index1, int time0, int time1, const float t)
{
	auto newT = t > 1 ? 1 : t;
	auto pos0 = GetPositioninAnim(index0, time0);
	auto pos1 = GetPositioninAnim(index1, time1);

	auto finalPos = std::vector<XMFLOAT4X4>();
	for (auto i = 0; i < pos0.size(); i++) {
		auto float4x4 = XMFLOAT4X4{};
		auto mat = XMMatrixTranspose(InterpMatrix(XMMatrixTranspose(XMLoadFloat4x4(&pos0[i])), XMMatrixTranspose(XMLoadFloat4x4(&pos1[i])), newT));
		XMStoreFloat4x4(&float4x4, mat);
		finalPos.push_back(float4x4);
	}
	
	return finalPos;
}

XMMATRIX AnimationManager::InterpMatrix(XMMATRIX m0, XMMATRIX m1, float t)
{
	XMVECTOR pos0;
	XMVECTOR rot0;
	XMVECTOR scale0;
	XMMatrixDecompose(&scale0, &rot0, &pos0, m0);

	XMVECTOR pos1;
	XMVECTOR rot1;
	XMVECTOR scale1;
	XMMatrixDecompose(&scale1, &rot1, &pos1, m1);

	scale0 = XMVectorRound(scale0);
	scale1 = XMVectorRound(scale1);

	auto newPos = XMVectorLerp(pos0, pos1, t);
	auto newRot = XMVectorLerp(rot0, rot1, t);
	auto newScale = XMVectorLerp(scale0, scale1, t);

	auto outMat = XMMatrixScalingFromVector(newScale) * XMMatrixRotationQuaternion(newRot) * XMMatrixTranslationFromVector(newPos);
	return outMat;
}