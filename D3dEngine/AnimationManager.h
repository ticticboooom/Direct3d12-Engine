#pragma once

#include "stdafx.h"
#include <vector>
#include "Structures.h"
/**
 * @brief Manages animations
 *		Gets the animation frames for each bone
 * 		Usually passed in from a model loaded with BinaryModelLoader
 * 
 */
class AnimationManager
{
public:
	D3DENGINE_API AnimationManager(std::shared_ptr<std::vector<std::vector<std::vector<XMMATRIX>>>> animations);
	D3DENGINE_API ~AnimationManager();
	D3DENGINE_API std::vector<XMFLOAT4X4> GetPositioninAnim(const int index, int time);
	D3DENGINE_API std::vector<XMFLOAT4X4> BlendAnimationsAtPositions(const int index0, const int index1, int time0, int time1, const float t);
	D3DENGINE_API UINT GetFrameCount(int animIndex, int index) { return (*m_animations)[animIndex][index].size(); }
	D3DENGINE_API UINT GetAnimCount() { return (*m_animations).size(); }
private:
	std::shared_ptr<std::vector<std::vector<std::vector<XMMATRIX>>>> m_animations; /// the animations per bone
	XMMATRIX InterpMatrix(XMMATRIX m0, XMMATRIX m1, float f);
};
