#include "pch.h"

#include "PhysicsEngine.h"

#include "Shape.h"
#include "BoxShape.h"
#include "SphereShape.h"
#include "CapsuleShape.h"
#include "TriangleMeshShape.h"
#include "PlaneShape.h"

#include "Rigidbody.h"

using namespace PhysicsEngine;

void Rigidbody::Initialize()
{
	PxTransform tm = ToPxTransform(m_WorldPos, m_WorldRot);

	bool isKinematic = false;
	bool isStatic = false;

	PxRigidActor* actor = NULL;

	if (m_Type == RigidbodyType::Static)
	{
		isStatic = true;
	}
	else
	{
		isStatic = false;
		if (m_Type == RigidbodyType::Kinematic)
		{
			isKinematic = true;
		}
		else
		{
			isKinematic = false;
		}
	}

	switch (m_pShape->m_type)
	{
	case ShapeType::Box:
	{
		//내부에서는 m_Size의 절반으로 나누어 사용(halfextent);
		BoxShape* pBoxShape = static_cast<BoxShape*>(m_pShape);
		actor = m_PhysicsEngine.CreateBox(tm, ToPxVec3(pBoxShape->m_Size / 2.f),
			ToPxTransform(pBoxShape->m_CenterPos), m_Density, isStatic, isKinematic);
		break;
	}
	case ShapeType::Capsule:
	{
		CapsuleShape* pCapsuleShape = static_cast<CapsuleShape*>(m_pShape);
		actor = m_PhysicsEngine.CreateCapsule(tm, pCapsuleShape->m_Radius, pCapsuleShape->m_BodyHeight / 2.f,
			ToPxTransform(pCapsuleShape->m_CenterPos), m_Density, isStatic, isKinematic);
		break;
	}
	case ShapeType::Sphere:
	{
		SphereShape* pSphereShape = static_cast<SphereShape*>(m_pShape);
		actor = m_PhysicsEngine.CreateSphere(tm, pSphereShape->m_Radius,
			ToPxTransform(pSphereShape->m_CenterPos), m_Density, isStatic, isKinematic);
		break;
	}
	case ShapeType::Plane:
	{
		PlaneShape* pPlaneShape = static_cast<PlaneShape*>(m_pShape);
		PxPlane plane(ToPxVec3(pPlaneShape->m_Normal), pPlaneShape->m_Distance);
		actor = m_PhysicsEngine.CreatePlane(plane);

		//Plane은 Static으로 만 생성
		m_Type = RigidbodyType::Static;

		break;
	}
	case ShapeType::TriangleMesh:
	{
		TriangleMeshShape* pTriangleMeshShape = static_cast<TriangleMeshShape*>(m_pShape);

		//원본 메시데이터로 충돌 메시데이터 cooking
		pTriangleMeshShape->CreateTriangleMesh();

		//cooking 여부 확인
		if (pTriangleMeshShape->m_pPxCookedMeshGeo)
		{
			actor = m_PhysicsEngine.CreateTriangleMesh(
				ToPxTransform(m_WorldPos, m_WorldRot), pTriangleMeshShape->m_pPxCookedMeshGeo);

			//actor->userData = pOwnerCallback;
		}

		//TriangleMesh은 Static으로만 생성
		m_Type = RigidbodyType::Static;

		break;
	}
	default:
		break;
	}

	//Shape에 PxShape 멤버 set, 원래는 Shape 내부에서 생성해야 하지만
	//PxRigidActor를 생성할 때 하나씩 PxShape를 자동으로 생성해주는 PhysX 함수를 사용하고 있어서
	//PxRigidActor 생성 이후에 가져오는 방식으로 setting을 한다. #수정
	PX_ASSERT(actor);
	if (actor)
	{
		actor->getShapes(&m_pShape->m_pPxShape, 1);
		actor->userData = m_pOwnerCallback;

		//임시 actor 포인터 cast 후 저장
		if (isStatic)
			m_pPxRigidStatic = actor->is<PxRigidStatic>();
		else
			m_pPxRigidDynamic = actor->is<PxRigidDynamic>();
	}
}

void Rigidbody::UpdateTransformFixed(Vector3 worldPos, Quaternion worldRot, float dtime)
{
	if (m_pPxRigidDynamic)
	{
		switch (m_Type)
		{
		case RigidbodyType::Static:
			break;
		case RigidbodyType::Dynamic:
			m_pPxRigidDynamic->setGlobalPose(ToPxTransform(worldPos, worldRot));
			break;
		case RigidbodyType::Kinematic:
			m_pPxRigidDynamic->setKinematicTarget(ToPxTransform(worldPos, worldRot));
			break;
		default:
			break;
		}

		PxTransform tm = m_pPxRigidDynamic->getGlobalPose();
		m_WorldPos = ToVector3(tm.p);
		m_WorldRot = ToQuaternion(tm.q);
	}
}

void Rigidbody::UpdateAfterSimulate(Vector3& worldPos, Quaternion& worldRot)
{
	if (m_pPxRigidDynamic)
	{
		PxTransform tm = m_pPxRigidDynamic->getGlobalPose();
		m_WorldPos = ToVector3(tm.p);
		m_WorldRot = ToQuaternion(tm.q);

		worldPos = m_WorldPos;
		worldRot = m_WorldRot;
	}
}

void Rigidbody::Release()
{

	//동적 할당한 메모리 해제
	if (m_pPxRigidDynamic)
	{
		if (m_IsTrigger)
		{
			m_PhysicsEngine.DeleteTriggerPair(m_pPxRigidDynamic);
		}
		m_pPxRigidDynamic->release();
		m_pPxRigidDynamic = NULL;
	}
		
	if (m_pPxRigidStatic)
	{
		if (m_IsTrigger)
		{
			m_PhysicsEngine.DeleteTriggerPair(m_pPxRigidStatic);
		}
		m_pPxRigidStatic->release();
		m_pPxRigidDynamic = NULL;
	}

	/*delete m_pShape;
	m_pShape = nullptr;*/
}

Rigidbody* Rigidbody::CreateBox(ISimulationEventCallback* ownerCallback,
	RigidbodyType type, Vector3 position, Quaternion rotation, Vector3 size, Vector3 centerPos,
	unsigned int filterGroup, unsigned int filterMask)
{
	static PhysicsEngineMain& physicEngine = PhysicsEngineMain::GetInstance();

	Shape* pBoxShape = new BoxShape(physicEngine, size, centerPos);

	Rigidbody* pRigidbody = new Rigidbody(physicEngine, ownerCallback,
		pBoxShape, type, position, rotation);

	pRigidbody->Initialize();

	//Initialize 내부에서 set한 QueryFilter가 덧씌워지지 않게 검사
	if (filterGroup != 0 && filterMask != 0)
		pRigidbody->SetupCollisionFilter(filterGroup, filterMask);

	return pRigidbody;
}

Rigidbody* Rigidbody::CreateCapsule(ISimulationEventCallback* ownerCallback,
	RigidbodyType type, Vector3 position, Quaternion rotation, float radius, float bodyHeight, Vector3 centerPos,
	unsigned int filterGroup, unsigned int filterMask)
{
	static PhysicsEngineMain& physicEngine = PhysicsEngineMain::GetInstance();

	Shape* pCapsuleShape = new CapsuleShape(physicEngine, radius, bodyHeight, centerPos);

	Rigidbody* pRigidbody = new Rigidbody(physicEngine, ownerCallback,
		pCapsuleShape, type, position, rotation);

	pRigidbody->Initialize();

	//Initialize 내부에서 set한 QueryFilter가 덧씌워지지 않게 검사
	if (filterGroup != 0 && filterMask != 0)
		pRigidbody->SetupCollisionFilter(filterGroup, filterMask);


	return pRigidbody;
}

Rigidbody* Rigidbody::CreateSphere(ISimulationEventCallback* ownerCallback,
	RigidbodyType type, Vector3 position, Quaternion rotation, float radius, Vector3 centerPos,
	unsigned int filterGroup, unsigned int filterMask)
{
	static PhysicsEngineMain& physicEngine = PhysicsEngineMain::GetInstance();

	Shape* pSphereShape = new SphereShape(physicEngine, radius, centerPos);

	Rigidbody* pRigidbody = new Rigidbody(physicEngine, ownerCallback,
		pSphereShape, type, position, rotation);

	pRigidbody->Initialize();

	//Initialize 내부에서 set한 QueryFilter가 덧씌워지지 않게 검사
	if (filterGroup != 0 && filterMask != 0)
		pRigidbody->SetupCollisionFilter(filterGroup, filterMask);

	return pRigidbody;
}

Rigidbody* Rigidbody::CreatePlaneStatic(ISimulationEventCallback* ownerCallback,
	Vector3 normal, float distance)
{
	static PhysicsEngineMain& physicEngine = PhysicsEngineMain::GetInstance();

	Shape* pBoxShape = new PlaneShape(physicEngine, normal, distance);

	//원점 위치와 방향 구하는 공식 구해서 추가하기 #수정
	Rigidbody* pRigidbody = new Rigidbody(physicEngine, ownerCallback,
		pBoxShape, RigidbodyType::Static, Vector3::Zero, Quaternion::Identity);

	pRigidbody->Initialize();

	return pRigidbody;
}

Rigidbody* Rigidbody::CreateTriangleMeshStatic(ISimulationEventCallback* ownerCallback,
	const TriangleMeshData& mesh, Vector3 position, Quaternion rotation, Vector3 scale, Vector3 centerPos)
{
	static PhysicsEngineMain& physicEngine = PhysicsEngineMain::GetInstance();

	Shape* pTriangleShape = new TriangleMeshShape(physicEngine, mesh, scale, centerPos);

	Rigidbody* pRigidbody = new Rigidbody(physicEngine, ownerCallback,
		pTriangleShape, RigidbodyType::Static, position, rotation);

	pRigidbody->Initialize();

	return pRigidbody;
}

void Rigidbody::SetTrigger(bool isTrigger)
{
	PxActor* actor;
	PxShape* shape = m_pShape->m_pPxShape;

	//eSIMULATION_SHAPE와 eTRIGGER_SHAPE는 둘다 set이 되면 안됨
	if (shape)
	{
		PxShapeFlags flag = shape->getFlags();

		if (isTrigger)
		{
			flag &= ~(PxShapeFlag::eSIMULATION_SHAPE); // | PxShapeFlag::eSCENE_QUERY_SHAPE
			flag |= PxShapeFlag::eTRIGGER_SHAPE;
		}
		else
		{
			flag |= PxShapeFlag::eSIMULATION_SHAPE; // | PxShapeFlag::eSCENE_QUERY_SHAPE
			flag &= ~PxShapeFlag::eTRIGGER_SHAPE;
		}

		shape->setFlags(flag);
	}


#ifdef _DEBUG
	shape->setFlag(PxShapeFlag::eVISUALIZATION, isTrigger);
#endif

	//PxRigidDynamic인데 밑에 깔려 있는 static mesh와 충돌 하지 않고 뚫고 내려 가지 않게 중력적용x
	if (m_Type == RigidbodyType::Dynamic)
	{
		if(m_pPxRigidDynamic)
			m_pPxRigidDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, isTrigger);
	}

	m_IsTrigger = isTrigger;
}

void Rigidbody::SetQueryActive(bool isActive)
{
	PxActor* actor;
	PxShape* shape = m_pShape->m_pPxShape;

	//eSIMULATION_SHAPE와 eTRIGGER_SHAPE는 둘다 set이 되면 안됨
	if (shape)
	{
		PxShapeFlags flag = shape->getFlags();

		if (isActive)
		{
			flag |= PxShapeFlag::eSCENE_QUERY_SHAPE;
		}
		else
		{
			flag &= ~PxShapeFlag::eSCENE_QUERY_SHAPE;
		}

		shape->setFlags(flag);
	}

	//PxRigidDynamic인데 밑에 깔려 있는 static mesh와 충돌 하지 않고 뚫고 내려 가지 않게 중력적용x
	/*if (m_Type == RigidbodyType::Dynamic)
	{
		if (m_pPxRigidDynamic)
			m_pPxRigidDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, isTrigger);
	}*/

	m_IsQueryActive = isActive;
}

void Rigidbody::SetCollionCBActive(bool isCollionCBActive)
{
	m_pOwnerCallback->m_IsActive = isCollionCBActive;

	m_IsCollionCBActive = isCollionCBActive;
}

void Rigidbody::SetSleep(bool isSleep)
{
	PxActor* actor;

	if (m_pPxRigidDynamic)
		actor = m_pPxRigidDynamic;
	else
		actor = m_pPxRigidStatic;

	actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, !isSleep);
#ifdef _DEBUG
	actor->setActorFlag(PxActorFlag::eVISUALIZATION, isSleep);
#endif

	m_IsSleep = isSleep;
}

void Rigidbody::SetupCollisionFilter(unsigned int filterGroup, unsigned int filterMask)
{
	PxFilterData fd;
	fd.word0 = filterGroup;
	fd.word1 = filterMask;

	if (m_pShape)
	{
		m_pShape->SetFilterData(fd);
	}
}

PhysicsEngine::Rigidbody* Rigidbody::CreateBoxTrigger(ISimulationEventCallback* ownerCallback,
	RigidbodyType type, Vector3 position, Quaternion rotation, Vector3 size,
	Vector3 centerPos /*= Vector3::Zero*/, unsigned int filterGroup /*= 0*/, unsigned int filterMask /*= 0*/)
{
	Rigidbody* rigidbody = CreateBox(ownerCallback, type,
		position, rotation, size, centerPos, filterGroup, filterMask);
	rigidbody->SetTrigger(true);

	return rigidbody;
}

PhysicsEngine::Rigidbody* Rigidbody::CreateCapsuleTrigger(ISimulationEventCallback* ownerCallback,
	RigidbodyType type, Vector3 position, Quaternion rotation, float radius, float bodyHeight,
	Vector3 centerPos /*= Vector3::Zero*/, unsigned int filterGroup /*= 0*/, unsigned int filterMask /*= 0*/)
{
	Rigidbody* rigidbody = CreateCapsule(ownerCallback, type,
		position, rotation, radius, bodyHeight, centerPos, filterGroup, filterMask);
	rigidbody->SetTrigger(true);

	return rigidbody;
}

PhysicsEngine::Rigidbody* Rigidbody::CreateSphereTrigger(ISimulationEventCallback* ownerCallback,
	RigidbodyType type, Vector3 position, Quaternion rotation, float radius,
	Vector3 centerPos /*= Vector3::Zero*/, unsigned int filterGroup /*= 0*/, unsigned int filterMask /*= 0*/)
{
	Rigidbody* rigidbody = CreateSphere(ownerCallback, type,
		position, rotation, radius, centerPos, filterGroup, filterMask);
	rigidbody->SetTrigger(true);

	return rigidbody;
}

void Rigidbody::SetActive(bool isActive)
{
	if(m_pPxRigidDynamic)
		m_PhysicsEngine.SetActive(m_pPxRigidDynamic, isActive);
	else if(m_pPxRigidStatic)
		m_PhysicsEngine.SetActive(m_pPxRigidStatic, isActive);
}
