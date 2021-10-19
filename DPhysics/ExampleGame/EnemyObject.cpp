#include "HeroObject.h"
#include "WeaponObject.h"
#include "ConsoleWindow.h"

#include "EnemyObject.h"


void EnemyObject::OnCollisionEnter(CollisionObject* ohter)
{
	
}

void EnemyObject::OnCollisionExit(CollisionObject* ohter)
{
	
}

void EnemyObject::OnCollisionStay(CollisionObject* ohter)
{

}

void EnemyObject::OnTriggerEnter(CollisionObject* ohter)
{
	WeaponObject* pWeapon = dynamic_cast<WeaponObject*>(ohter);
	if (pWeapon)
	{
		std::string weaponName = pWeapon->GetName();

		std::wstring dbgStr;
		dbgStr.assign(weaponName.begin(), weaponName.end());

		dbgStr = L"HeroName : " + dbgStr + L"\n";

		ConsoleWindow::Write(dbgStr.c_str());
	}
}

void EnemyObject::OnTriggerExit(CollisionObject* ohter)
{
	//테스트용 코드
	/*m_currentState = EnemyState::eDead;
	ohter->DeleteInstance();*/
}

void EnemyObject::OnTriggerStay(CollisionObject* ohter)
{

}

void EnemyObject::Hit()
{
	m_currentState = EnemyState::eHit;
}

void EnemyObject::OnControllerHitCollision(PhysicsEngine::CharaterControllerHit& hit)
{
	HeroObject* pHero = dynamic_cast<HeroObject*>(hit.otherController);
	if(pHero)
	{
		std::string heroName = pHero->GetName();

		std::wstring dbgStr;
		dbgStr.assign(heroName.begin(), heroName.end());

		dbgStr = L"HeroName : " + dbgStr + L"\n";

		ConsoleWindow::Write(dbgStr.c_str());
	}
	
}

void EnemyObject::Initialize()
{
	SetFilterGroup(CustomFilterGroup::eEnemy, CustomFilterGroup::eWeapon);

	m_currentState = EnemyState::eIdle;
}

void EnemyObject::FixedUpdate(float dTime /*= 0*/)
{
	m_translation += m_accumMove * dTime;
	CollisionObject::FixedUpdate(dTime);
}

void EnemyObject::Update(float dTime /*= 0*/)
{

	if (m_HP <= 0)
	{
		m_currentState = EnemyState::eDead;
		DeleteInstance();

		if (m_pCCT)
		{
			m_pCCT->Release();
		}
		else
		{
			m_pRigidbody->Release();
		}

		return;
	}

	CollisionObject::Update(dTime);
}
