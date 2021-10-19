#pragma once
#include"AnimatedObject.h"
#include "CollisionObject.h"
#include"UnitObject.h"

enum class HeroState
{
	eIdle, eRun
};

struct PhysicsEngine::CharaterControllerHit;

class WeaponObject;

class HeroObject final: public AnimatedObject, public UnitObject, public CollisionObject
{
	Keyboard* m_pKeyboard;//키보드 입력을 받아 움직일 것임.
	Camera* m_camera;//카메라 시점에 따른 캐릭터 이동이 이루어 질 것임.

	HeroState m_currentState = HeroState::eIdle;
	
	//초당 애니메이션 몇번?
	float m_idleAnimSpeed = 2.f;
	float m_runAnimSpeed = 2.f;

	XMFLOAT3 m_accumMove;

	//점프
	bool m_IsJump = false;
	float m_JumpTime = 0.f;
	float m_InitalVelocity = 20.f;

	bool m_IsGround = false;

	//status
	//float hp;
	//float speed;
	//float defense;

	//테스트용 무기 충돌체
	WeaponObject* m_pTestWeaponCollider = nullptr;
	
	bool m_IsAttack = false;

public:
	HeroObject();
	~HeroObject ()override {};

	void Update(float dTime = 0)override;
	void Move(float dTime) override;

	void Initialize();

	void AttachWeapon(WeaponObject* pWeapon);

	void OnCollisionEnter(CollisionObject* ohter);

	void OnCollisionExit(CollisionObject* ohter);

	void OnCollisionStay(CollisionObject* ohter);

	void OnTriggerEnter(CollisionObject* ohter);

	void OnTriggerExit(CollisionObject* ohter);

	void OnControllerHitCollision(PhysicsEngine::CharaterControllerHit& hit);

private:
	
};
