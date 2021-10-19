#pragma once
#include "CollisionObject.h"

enum class WeaponState
{
	eIdle, eAttack
};

class WeaponObject : public CollisionObject
{
	WeaponState m_currentState = WeaponState::eIdle;

public:
	Matrix m_WeaponLocalTM;
	Matrix m_OwnerTM;

	//game status
	int m_Damage = 10;
	const float m_EffectTime = 0.5f;
	float m_EnlapsedEffectTime = 1.1f;


	//float defense;

	//디버그용 박스 객체
	HSimplePrimitive* pPrimitiveBox = nullptr;

public:
	WeaponObject() {};
	~WeaponObject()override {};

	void Initialize();
	void FixedUpdate(float dTime = 0)override;
	void Update(float dTime = 0)override;

	void OnCollisionEnter(CollisionObject* ohter) override;
	void OnCollisionExit(CollisionObject* ohter) override;
	void OnCollisionStay(CollisionObject* ohter) override;
	void OnTriggerEnter(CollisionObject* ohter) override;
	void OnTriggerExit(CollisionObject* ohter) override;
	void OnTriggerStay(CollisionObject* ohter) override;

public:
	void WeaponAttack();
	void WeaponIdle();
};

