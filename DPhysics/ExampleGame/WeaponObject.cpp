#include "WeaponObject.h"
#include "ConsoleWindow.h"

#include "EnemyObject.h"

void WeaponObject::Initialize()
{
	SetFilterGroup(CustomFilterGroup::eWeapon, CustomFilterGroup::eEnemy);
	m_pRigidbody->SetActive(false);

	//y+ 20
	m_WeaponLocalTM = Matrix::CreateFromQuaternion(Quaternion(0.710, 0, 0, 0.710)) * Matrix::CreateTranslation(Vector3(0, 20, -10));

	pPrimitiveBox = m_pHEngine->CreateBox(Vector3(20, 40, 20), ShaderType_Primitive::eNoOption_Primitive);

	ConsoleWindow::Allocate();
	
}

void WeaponObject::FixedUpdate(float dTime /*= 0*/)
{
	//무기 위치 업데이트
	Matrix weaponTM = m_WeaponLocalTM * m_OwnerTM;
	pPrimitiveBox->worldTM = weaponTM;
	pPrimitiveBox->worldTM.Decompose(m_scale, m_quatRot, m_translation);

	if (m_EnlapsedEffectTime < m_EffectTime)
	{
		m_EnlapsedEffectTime += dTime;
	}
	else if (m_currentState == WeaponState::eAttack)
	{
		WeaponIdle();
	}

	if (m_currentState == WeaponState::eAttack)
	{
		//ConsoleWindow::Write(L"Attack\n");
		
	}
	else if (m_currentState == WeaponState::eIdle)
	{
		//ConsoleWindow::Write(L"Idle\n");

		
	}

	CollisionObject::FixedUpdate(dTime);
}

void WeaponObject::Update(float dTime /*= 0*/)
{
	//local tm을 곱한 WorldTM set
	///pPrimitiveBox->worldTM = m_WeaponLocalTM * m_OwnerTM;

	if (m_currentState == WeaponState::eAttack)
	{
		
		SetModelHandle(m_pModelData);
		//pPrimitiveBox->worldTM.CreateScale(Vector3(1, 1, 1));
	}
	else if (m_currentState == WeaponState::eIdle)
	{
		
		DeleteInstance();
		//pPrimitiveBox->worldTM = Matrix::CreateTranslation(Vector3(0, 20, -10)) * m_OwnerTM;
	}

	CollisionObject::Update(dTime);
}

void WeaponObject::WeaponAttack()
{
	m_pRigidbody->SetActive(true);
	m_currentState = WeaponState::eAttack;
	m_EnlapsedEffectTime = 0.f;
}

void WeaponObject::WeaponIdle()
{
	m_pRigidbody->SetActive(false);
	m_currentState = WeaponState::eIdle;
}

void WeaponObject::OnCollisionEnter(CollisionObject* ohter)
{
	
}

void WeaponObject::OnCollisionExit(CollisionObject* ohter)
{

}

void WeaponObject::OnCollisionStay(CollisionObject* ohter)
{
	
}

void WeaponObject::OnTriggerEnter(CollisionObject* ohter)
{
	
}

void WeaponObject::OnTriggerExit(CollisionObject* ohter)
{

}

void WeaponObject::OnTriggerStay(CollisionObject* ohter)
{
	//enum으로 구별 구현
	EnemyObject* pEnemy = dynamic_cast<EnemyObject*>(ohter);
	if (pEnemy)
	{
		pEnemy->m_HP -= m_Damage;

		wchar_t wdbgStr[256];
		swprintf_s(wdbgStr, L"hit : %d\n", pEnemy->m_HP);

		std::wstring dbgStr = wdbgStr;
		ConsoleWindow::Write(dbgStr.c_str());
	}
}
