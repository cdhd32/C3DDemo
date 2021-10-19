#include"CameraControl.h"

void CameraControl::Initialize(HeroObject* pHeroObject)
{
	m_pCamera = HEngine_DX12_3D::GetInstance()->GetCamera();
	//m_pCamera->SetCameraMode(CameraMode::eOrthgraphic);
	m_pCamera->SetCameraMode(CameraMode::ePerspective);

	m_pHero = pHeroObject;
}

void CameraControl::Update(float dTime)
{
	if(m_pHero!=nullptr)
	FollowHero();
}

void CameraControl::FollowHero()
{
	Vector3 heroTranslation = m_pHero->GetTranslation();

	XMFLOAT3 heroTranslation2 = { heroTranslation.x,heroTranslation.y,heroTranslation.z };
	
	static XMFLOAT3 up = { 0,1,0 };

	XMFLOAT3 newCamPos = { heroTranslation2.x + 300, heroTranslation2.y + 300, heroTranslation2.z - 300 };

	 
	m_pCamera->LookAt(newCamPos, heroTranslation2, up);

}
