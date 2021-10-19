#pragma once

#include"Object.h"


enum class CollisionType
{
	eNone, eCharacterControl, eRigidStatic, eRigidKinematic, eRigidDynamic
};

struct CustomFilterGroup
{
	enum Enum
	{
		eNone = (1 << 0),
		ePC = (1 << 1),
		eEnemy = (1 << 2),
		eField = (1 << 3),
		eWeapon = (1 << 4)
	};
};

struct CharaterControllerHit;

class CollisionObject : virtual public Object,
	virtual public PhysicsEngine::ISimulationEventCallback
{
protected:
	PhysicsEngine::PhysicsEngineMain* m_p3DPhysicsEngine;

	PhysicsEngine::Rigidbody* m_pRigidbody = nullptr;
	PhysicsEngine::CharacterController* m_pCCT = nullptr;

	//충돌체 중심점의 상대좌표//
	Vector3 m_center;
	Matrix m_axisModify;
	Vector3 m_axisModifyTranslation;
	Quaternion m_axisModifyQuat;
	Vector3 m_axisModifyScale;


	Vector3 m_accumTranslation;


	CollisionType m_collsionType = CollisionType::eNone;

	PhysicsEngine::CCTCollisionDirFlags m_collisionDir;

	CustomFilterGroup m_FilterGroup;

public:
	CollisionObject() : Object(), m_p3DPhysicsEngine(&PhysicsEngine::PhysicsEngineMain::GetInstance()) {};
	virtual ~CollisionObject() override;

	void CreateCCTBoxFromModelBoundingBox(Vector3 initPos);
	//static 충돌체들은  위치를 바꿔 줄 수 없다.
	//Object 클래스의 Transformation과 상관 없이 최초 생성 시 입력된 매개변수로 생성된다. 
	void CreateRigidStaticBox(Vector3 position, Quaternion rotation, Vector3 size);
	void CreateRigidStaticMeshFromModelData(Vector3 position, Quaternion rotation, Vector3 scale);

	void CreateRigidKinematicBox(Vector3 position, Quaternion rotation, Vector3 size);
	void CreateRigidDynamicBox(Vector3 position, Quaternion rotation, Vector3 size);

	void DeleteRigidActor();
	void DeleteCCT();

	virtual void FixedUpdate(float accumTime = 0);
	virtual void Update(float dTime = 0);

	void SetTriggerType(bool isTrigger);
	void SetAtive(bool isActive);

	void SetFilterGroup(unsigned int filterGroup, unsigned int targetGroup);

	//implements from ISimulationEventCallback
	void OnCollisionEnter(PhysicsEngine::ISimulationEventCallback* ohter) override;
	void OnCollisionExit(PhysicsEngine::ISimulationEventCallback* ohter) override;
	void OnCollisionStay(PhysicsEngine::ISimulationEventCallback* ohter) override;

	void OnTriggerEnter(PhysicsEngine::ISimulationEventCallback* ohter) override;
	void OnTriggerExit(PhysicsEngine::ISimulationEventCallback* ohter) override;
	void OnTriggerStay(PhysicsEngine::ISimulationEventCallback* ohter) override;

	void OnControllerHit(PhysicsEngine::CharaterControllerHit& hit) override;

	virtual void OnCollisionEnter(CollisionObject* ohter) {};
	virtual void OnCollisionExit(CollisionObject* ohter) {};
	virtual void OnCollisionStay(CollisionObject* ohter) {};

	virtual void OnTriggerEnter(CollisionObject* ohter) {};
	virtual void OnTriggerExit(CollisionObject* ohter) {};
	virtual void OnTriggerStay(CollisionObject* ohter) {};

	virtual void OnControllerHitCollision(PhysicsEngine::CharaterControllerHit& hit) {};

private:


};