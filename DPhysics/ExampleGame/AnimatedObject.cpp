#include"AnimatedObject.h"


void AnimatedObject::ResetAnimationTime()
{
	m_animationTime = 0;
	m_animationRatio = 0;
}

void AnimatedObject::SetAnimationName(std::string animationName)
{
	if (m_pInstanceData != nullptr)
		m_pInstanceData->animationName = animationName;
}

void AnimatedObject::SetAnimationTime(float animationTime)
{
	if (m_pInstanceData != nullptr)
		m_pInstanceData->animationTime = animationTime;
}

//void AnimatedObject::SetAnimationTimeByRatio(float ratio)
//{
//	if (m_instanceHandle != 0)
//		m_pHEngine->SetInstanceAnimTimeByRatio(m_instanceHandle, ratio);
//}
