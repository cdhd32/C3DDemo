#include "WeaponObject.h"
#include "EnemyObject.h"

#include "ConsoleWindow.h"

#include "HeroObject.h"

using namespace PhysicsEngine;

HeroObject::HeroObject() :AnimatedObject(), UnitObject(), CollisionObject()
{
	m_pKeyboard = m_pHEngine->GetKeyboard();
	m_camera = m_pHEngine->GetCamera();

	m_speed = 50;
}


void HeroObject::Initialize()
{
	SetFilterGroup(0, 0);
}

void HeroObject::Update(float dTime)
{
	CollisionObject::Update();

	m_currentState = HeroState::eIdle;

	Move(dTime);

	if (m_currentState == HeroState::eIdle)
	{
		SetAnimationName("idle");
	}
	else if (m_currentState == HeroState::eRun)
	{
		SetAnimationName("run");
	}

	m_animationTime += dTime;
	SetAnimationTime(m_animationTime);

}

void HeroObject::Move(float dTime)
{
	XMMATRIX View = m_camera->GetView();

	XMVECTOR det = XMMatrixDeterminant(View);

	XMMATRIX invView = XMMatrixInverse(&det, View);

	static XMMATRIX turn90Degree = XMMatrixRotationY(XMConvertToRadians(90));
	XMVECTOR vPlusX = XMVector3TransformNormal(XMVectorSet(1, 0, 0, 0), invView);
	XMVECTOR vMinusX = XMVector3TransformNormal(XMVectorSet(-1, 0, 0, 0), invView);
	XMVECTOR vPlusY = XMVector3TransformNormal(vMinusX, turn90Degree);
	XMVECTOR vMinusY = XMVector3TransformNormal(vPlusX, turn90Degree);

	Keyboard::State keyState = m_pKeyboard->GetState();

	XMVECTOR addedTranslation = XMVectorSet(0,0,0,0);

	if (keyState.Up)
	{
		addedTranslation += vPlusY * dTime * m_speed;
	}
	if (keyState.Down)
	{
		addedTranslation += vMinusY * dTime * m_speed;
	}
	if (keyState.Left)
	{
		addedTranslation += vMinusX * dTime * m_speed;
	}
	if (keyState.Right)
	{
		addedTranslation += vPlusX * dTime * m_speed;
	}
	//ATTACK!
	if (keyState.Space)
	{
		if (m_pTestWeaponCollider)
		{
			m_pTestWeaponCollider->WeaponAttack();
		}
	}

	if (static_cast<unsigned int>(m_collisionDir & CCTCollisionDirFlag::eCOLLISION_DOWN))
	{
		m_IsJump = false;
		m_IsGround = true;
		m_JumpTime = 0.f;
	}
	else if (!static_cast<unsigned int>(m_collisionDir & CCTCollisionDirFlag::eCOLLISION_SIDES))
	{
		m_IsGround = false;
	}

	//Jump!
	if (!m_IsGround && keyState.Z)
	{
		m_IsJump = true;
	}
	
	if (m_IsJump)
	{
		m_currentState = HeroState::eRun;
		m_JumpTime += dTime;
		addedTranslation += XMVectorSet(0, m_InitalVelocity - m_gravity * m_JumpTime, 0, 0);
	}
	else if (!m_IsGround)
	{
		m_JumpTime += dTime;
		addedTranslation += XMVectorSet(0, - m_gravity * m_JumpTime, 0, 0);
	}


	//Gravity!
	/*if (!m_IsGround)
	{
		addedTranslation += XMVectorSet(0, -m_gravity * dTime, 0, 0);
	}*/
	

	m_accumTranslation += addedTranslation;

	if (keyState.Up || keyState.Down || keyState.Left || keyState.Right)
	{
		m_currentState = HeroState::eRun;
		XMStoreFloat3(&m_destfront, addedTranslation);
	}

	RotateByFrontVector(dTime* 10);

	if (m_pTestWeaponCollider)
	{
		m_pTestWeaponCollider->m_OwnerTM = m_transformMatrix;
	}
	
}

void HeroObject::AttachWeapon(WeaponObject* pWeapon)
{
	if (pWeapon)
	{
		m_pTestWeaponCollider = pWeapon;
		m_pTestWeaponCollider->m_OwnerTM = m_transformMatrix;
	}

	
}


void HeroObject::OnCollisionEnter(CollisionObject* ohter)
{
	/*EnemyObject* pEnemy = dynamic_cast<EnemyObject*>(ohter);
	if (pEnemy)
	{
		pEnemy->m_HP -= 10.f;

		wchar_t wdbgStr[256];
		swprintf_s(wdbgStr, L"hit : %d\n", pEnemy->m_HP);

		std::wstring dbgStr = wdbgStr;
		ConsoleWindow::Write(dbgStr.c_str());
	}*/
}

void HeroObject::OnCollisionExit(CollisionObject* ohter)
{

	//throw std::logic_error("The method or operation is not implemented.");
}

void HeroObject::OnCollisionStay(CollisionObject* ohter)
{

	//throw std::logic_error("The method or operation is not implemented.");
}

void HeroObject::OnTriggerEnter(CollisionObject* ohter)
{
	
	//throw std::logic_error("The method or operation is not implemented.");
}

void HeroObject::OnTriggerExit(CollisionObject* ohter)
{

	//throw std::logic_error("The method or operation is not implemented.");
}

void HeroObject::OnControllerHitCollision(PhysicsEngine::CharaterControllerHit& hit)
{
	if (!m_pTestWeaponCollider)
	{
		EnemyObject* pEnemy = dynamic_cast<EnemyObject*>(hit.otherController);
		if (pEnemy)
		{
			pEnemy->m_HP -= 10.f;

			wchar_t wdbgStr[256];
			swprintf_s(wdbgStr, L"hit : %d\n", pEnemy->m_HP);

			std::wstring dbgStr = wdbgStr;
			ConsoleWindow::Write(dbgStr.c_str());
		}
	}
}
