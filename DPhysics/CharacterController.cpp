#include "pch.h"

#include "PhysicsEngine.h"

#include "Shape.h"
#include "BoxShape.h"
#include "CapsuleShape.h"

#include "Rigidbody.h"

#include "ConsoleWindow.h"

#include "CharacterController.h"



using namespace PhysicsEngine;
using namespace DirectX;

void CharacterController::Initialize()
{
	PxControllerShapeType::Enum pxType;
	Shape* shape = nullptr;

	//������ CCTShapeType�� PxControllerShapeType::Enum ���� �ٲٱ�
	//+shape ����
	switch (m_Type)
	{
	case CCTShapeType::Capsule:
		pxType = PxControllerShapeType::Enum::eCAPSULE;
		shape = new CapsuleShape(m_PhysicsEngine, m_Radius, m_BodyHeight);
		break;
	case CCTShapeType::Box:
		pxType = PxControllerShapeType::Enum::eBOX;
		shape = new BoxShape(m_PhysicsEngine, m_Size);
		break;
	case CCTShapeType::None:
	default:
		pxType = PxControllerShapeType::Enum::eFORCE_DWORD;
		break;
	}

	if (m_Type == CCTShapeType::Capsule)
	{
		m_CrouchHeight = m_BodyHeight / 2;

		//CCT ������ �ʿ��� ���� set
		PhysicsEngine::ControlledActorDesc desc;

		desc.type = pxType;
		desc.proxyDensity = m_ProxyDensity;
		desc.proxyScale = m_ProxyScale;
		desc.position = PxExtendedVec3(m_Position.x, m_Position.y, m_Position.z);
		desc.slopeLimit = m_SlopeLimit;
		desc.contactOffset = m_ContactOffset;
		desc.stepOffset = m_StepOffset;
		desc.invisibleWallHeight = m_InvisibleWallHeight;
		desc.maxJumpHeight = m_MaxJumpHeight;
		//cDesc.nonWalkableMode	= PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
		desc.pReportCallback = &m_PhysicsEngine;
		desc.pBehaviorCallback = &m_PhysicsEngine;
		desc.volumeGrowth = m_VolumeGrowth;

		m_pPxController = m_PhysicsEngine.CreateCharacterContollerCapsule(desc, m_BodyHeight, m_Radius, m_CrouchHeight);
	}
	else
	{
		m_CrouchHeight = m_Size.y / 2;

		//CCT ������ �ʿ��� ���� set
		PhysicsEngine::ControlledActorDesc desc;

		desc.type = pxType;
		desc.proxyDensity = m_ProxyDensity;
		desc.proxyScale = m_ProxyScale;
		desc.position = PxExtendedVec3(m_Position.x, m_Position.y, m_Position.z);
		desc.slopeLimit = m_SlopeLimit;
		desc.contactOffset = m_ContactOffset;
		desc.stepOffset = m_StepOffset;
		desc.invisibleWallHeight = m_InvisibleWallHeight;
		desc.maxJumpHeight = m_MaxJumpHeight;
		//cDesc.nonWalkableMode	= PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
		desc.pReportCallback = &m_PhysicsEngine;
		desc.pBehaviorCallback = &m_PhysicsEngine;
		desc.volumeGrowth = m_VolumeGrowth;

		m_pPxController = m_PhysicsEngine.CreateCharacterContollerBox(desc, ToPxVec3(m_Size/2.f), m_CrouchHeight);
	}

	//rigidbody, PxRigidDynamic, eKINEMATIC ���� �����ǹǷ� Rigidbody ����� �˸°� �־��ش�.
	PxRigidDynamic* pxRigidDynamic;
	//PxShape* pxShape;

	if (m_pPxController)
	{

		pxRigidDynamic = m_pPxController->getActor();
		pxRigidDynamic->getShapes(&shape->m_pPxShape, 1);

		//rigidbody, shape ���� physx ��ü �����͸� set���ش�.

		//callback �ؾ��ϴ� owner�� ������ set
		pxRigidDynamic->userData = m_pOwnerCallback;

		//Actor�� ������ �ִ� Controller�� ������ set
		m_pPxController->setUserData(pxRigidDynamic);
		

		//������ ������ �������� Rigidbody ����
		m_pRigidbody = new Rigidbody(m_PhysicsEngine, m_pOwnerCallback, shape, RigidbodyType::Kinematic, m_Position);
		m_pRigidbody->m_pPxRigidDynamic = pxRigidDynamic;

		//m_pRigidbody->SetupCollisionFilter(PhysicsEngineMain::DEFAULT_QUERY_FILTERDATA, PhysicsEngineMain::DEFAULT_QUERY_FILTERDATA);
	}
	
}

CCTCollisionDirFlags CharacterController::UpdatePosition(Vector3 displacement, float dTime)
{

	PxVec3 pxDisplacement = ToPxVec3(displacement);

	PxControllerCollisionFlags collisionFlags = m_PhysicsEngine.MoveCCT(m_pPxController, pxDisplacement, dTime);

	GetPosition(m_Position);

	return collisionFlags;
}

void CharacterController::GetPosition(Vector3& position)
{
	PxExtendedVec3 vec3 = m_pPxController->getPosition();

	position = Vector3(vec3.x, vec3.y, vec3.z);
	m_Position = position;
}

void CharacterController::GetFootPosition(Vector3& position)
{
	PxExtendedVec3 vec3 = m_pPxController->getPosition();
	m_Position = Vector3(vec3.x, vec3.y, vec3.z);

	vec3 = m_pPxController->getFootPosition();
	position = Vector3(vec3.x, vec3.y, vec3.z);
}

PhysicsEngine::CharacterController* CharacterController::CreateCapsule(ISimulationEventCallback* ownerCallback,
	Vector3 initPos, float radius, float height, unsigned int filterGroup, unsigned int filterMask)
{
	static PhysicsEngineMain& physicEngine = PhysicsEngineMain::GetInstance();

	CharacterController* pCCT = new CharacterController(physicEngine, ownerCallback, initPos, radius, height);
	pCCT->Initialize();

	if(filterGroup != 0 && filterMask != 0)
		pCCT->SetupCollisionFilter(filterGroup, filterMask);

	return pCCT;
}

PhysicsEngine::CharacterController* CharacterController::CreateBox(ISimulationEventCallback* ownerCallback,
	Vector3 initPos, Vector3 size, unsigned int filterGroup, unsigned int filterMask)
{
	static PhysicsEngineMain& physicEngine = PhysicsEngineMain::GetInstance();

	CharacterController* pCCT = new CharacterController(physicEngine, ownerCallback, initPos, size);
	pCCT->Initialize();

	if (filterGroup != 0 && filterMask != 0)
		pCCT->SetupCollisionFilter(filterGroup, filterMask);

	return pCCT;
}

void CharacterController::SetupCollisionFilter(unsigned int filterGroup, unsigned int filterMask)
{
	m_pRigidbody->SetupCollisionFilter(filterGroup, filterMask);
}
void CharacterController::SetCollionCBActive(bool isCollionCBActive)
{
	if (m_pRigidbody)
		m_pRigidbody->SetCollionCBActive(isCollionCBActive);
}
//void CharacterController::SetActive(bool isActive)
//{
//	//���� ���� ������� ����
//	if (isActive)
//	{
//		if (IsReleased())
//		{
//			Initialize();
//		}
//	}
//	else
//	{
//		Release();
//	}
//
//	m_pRigidbody->m_IsActive = isActive;
//}

// CharacterController -> RigidBody -> Shape ������ ����(physx ��ü)/����(DPhysics ��ü)
void CharacterController::Release()
{
	if (m_pPxController)
	{
		m_pPxController->release();
		m_pPxController = NULL;
	}

	/*delete m_pRigidbody;
	m_pRigidbody = nullptr;*/
}

void CharacterController::SetActive(bool isActive)
{
	m_PhysicsEngine.SetActive(m_pPxController, isActive);
	m_pRigidbody->SetActive(isActive);
}

void CharacterController::SetQueryActive(bool isActive)
{
	m_pRigidbody->SetQueryActive(isActive);
}
