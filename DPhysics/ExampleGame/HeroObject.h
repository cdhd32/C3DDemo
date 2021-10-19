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
	Keyboard* m_pKeyboard;//Ű���� �Է��� �޾� ������ ����.
	Camera* m_camera;//ī�޶� ������ ���� ĳ���� �̵��� �̷�� �� ����.

	HeroState m_currentState = HeroState::eIdle;
	
	//�ʴ� �ִϸ��̼� ���?
	float m_idleAnimSpeed = 2.f;
	float m_runAnimSpeed = 2.f;

	XMFLOAT3 m_accumMove;

	//����
	bool m_IsJump = false;
	float m_JumpTime = 0.f;
	float m_InitalVelocity = 20.f;

	bool m_IsGround = false;

	//status
	//float hp;
	//float speed;
	//float defense;

	//�׽�Ʈ�� ���� �浹ü
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
