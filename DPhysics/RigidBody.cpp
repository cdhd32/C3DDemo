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
		//���ο����� m_Size�� �������� ������ ���(halfextent);
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

		//Plane�� Static���� �� ����
		m_Type = RigidbodyType::Static;

		break;
	}
	case ShapeType::TriangleMesh:
	{
		TriangleMeshShape* pTriangleMeshShape = static_cast<TriangleMeshShape*>(m_pShape);

		//���� �޽õ����ͷ� �浹 �޽õ����� cooking
		pTriangleMeshShape->CreateTriangleMesh();

		//cooking ���� Ȯ��
		if (pTriangleMeshShape->m_pPxCookedMeshGeo)
		{
			actor = m_PhysicsEngine.CreateTriangleMesh(
				ToPxTransform(m_WorldPos, m_WorldRot), pTriangleMeshShape->m_pPxCookedMeshGeo);

			//actor->userData = pOwnerCallback;
		}

		//TriangleMesh�� Static���θ� ����
		m_Type = RigidbodyType::Static;

		break;
	}
	default:
		break;
	}

	//Shape�� PxShape ��� set, ������ Shape ���ο��� �����ؾ� ������
	//PxRigidActor�� ������ �� �ϳ��� PxShape�� �ڵ����� �������ִ� PhysX �Լ��� ����ϰ� �־
	//PxRigidActor ���� ���Ŀ� �������� ������� setting�� �Ѵ�. #����
	PX_ASSERT(actor);
	if (actor)
	{
		actor->getShapes(&m_pShape->m_pPxShape, 1);
		actor->userData = m_pOwnerCallback;

		//�ӽ� actor ������ cast �� ����
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

	//���� �Ҵ��� �޸� ����
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

	//Initialize ���ο��� set�� QueryFilter�� ���������� �ʰ� �˻�
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

	//Initialize ���ο��� set�� QueryFilter�� ���������� �ʰ� �˻�
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

	//Initialize ���ο��� set�� QueryFilter�� ���������� �ʰ� �˻�
	if (filterGroup != 0 && filterMask != 0)
		pRigidbody->SetupCollisionFilter(filterGroup, filterMask);

	return pRigidbody;
}

Rigidbody* Rigidbody::CreatePlaneStatic(ISimulationEventCallback* ownerCallback,
	Vector3 normal, float distance)
{
	static PhysicsEngineMain& physicEngine = PhysicsEngineMain::GetInstance();

	Shape* pBoxShape = new PlaneShape(physicEngine, normal, distance);

	//���� ��ġ�� ���� ���ϴ� ���� ���ؼ� �߰��ϱ� #����
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

	//eSIMULATION_SHAPE�� eTRIGGER_SHAPE�� �Ѵ� set�� �Ǹ� �ȵ�
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

	//PxRigidDynamic�ε� �ؿ� ��� �ִ� static mesh�� �浹 ���� �ʰ� �հ� ���� ���� �ʰ� �߷�����x
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

	//eSIMULATION_SHAPE�� eTRIGGER_SHAPE�� �Ѵ� set�� �Ǹ� �ȵ�
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

	//PxRigidDynamic�ε� �ؿ� ��� �ִ� static mesh�� �浹 ���� �ʰ� �հ� ���� ���� �ʰ� �߷�����x
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
