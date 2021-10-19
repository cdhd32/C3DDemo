#pragma once
#include "UnitObject.h"
#include "CollisionObject.h"

enum class EnemyState
{
	eAlive, eDead, eHit, eIdle
};

class EnemyObject : public CollisionObject
{
	
public:
	Vector3 m_accumMove;

	//status
	int m_HP = 100;
	EnemyState m_currentState = EnemyState::eAlive;
	

	//float speed;
	//float defense;

public:
	EnemyObject() {};
	~EnemyObject()override {};

	void Initialize();
	void FixedUpdate(float dTime = 0)override;
	void Update(float dTime = 0)override;

	void OnCollisionEnter(CollisionObject* ohter) override;
	void OnCollisionExit(CollisionObject* ohter) override;
	void OnCollisionStay(CollisionObject* ohter) override;
	void OnTriggerEnter(CollisionObject* ohter) override;
	void OnTriggerExit(CollisionObject* ohter) override;
	void OnTriggerStay(CollisionObject* ohter) override;

	void Hit();

	void OnControllerHitCollision(PhysicsEngine::CharaterControllerHit& hit) override;

};

