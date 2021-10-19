#pragma once
#include"EngineInterface.h"
#include"ObjectManager.h"

class CameraControl
{
private:
	Camera* m_pCamera;

	HeroObject* m_pHero;

public:
	void Initialize(HeroObject* pHeroObject);
	void Update(float dTime = 0);

private:
	void FollowHero();

};
