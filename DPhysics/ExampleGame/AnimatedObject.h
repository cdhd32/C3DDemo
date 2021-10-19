#pragma once
#include"Object.h"

//enum class AnimationMode
//{
//	eByTime,
//	eByRatio
//};

class AnimatedObject : public virtual Object
{
protected:
	std::string m_animationName;
	float m_animationTime = 0;
	float m_animationRatio = 0;

	bool m_bAnimationDirty;

	//AnimationMode eAnimMode = AnimationMode::eByRatio;
	

public:
	AnimatedObject() :Object() {};
	virtual ~AnimatedObject() override {};

	//virtual void Update(float dTime = 0)override;

	void ResetAnimationTime();
	//set
	void SetAnimationName(std::string animationName);
	void SetAnimationTime(float animationTime);
	//void SetAnimationTimeByRatio(float ratio);

	//get 
	std::string GetAnimationName(){ return m_animationName; }
	float GetAnimationTime() { return m_animationTime; }
	float GetAnimationRatio() { return m_animationRatio; }


};
