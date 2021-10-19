#include "pch.h"

#include "PhysicsEngine.h"
#include "ISimulationEventCallback.h"
#include "ICCTEventCallBack.h"
#include "PhysicsEngineInterface.h"


using namespace PhysicsEngine;
using namespace DirectX::SimpleMath;
using namespace physx;

CCT_PTR PhysicsEngineInterface::CreateCCTBox(ISimulationEventCallback* pOwnerCallback, Vector3 initPos, Vector3 halfExtent)
{
	CCT_PTR actorPtr = NULL;

	float crouchHeight = halfExtent.y / 2;

	//CCT 생성에 필요한 정보 set
	PhysicsEngine::ControlledActorDesc desc;
	desc.type = PxControllerShapeType::Enum::eBOX;
	desc.proxyDensity = 10.0f;
	desc.proxyScale = 1.0f; //Default: 0.9f
	desc.position = PxExtendedVec3(initPos.x, initPos.y, initPos.z);
	desc.slopeLimit = 0.f;
	desc.contactOffset = 0.01f; //Default: 0.1
	desc.stepOffset = 0.f;
	desc.invisibleWallHeight = 0.f;
	desc.maxJumpHeight = 0.f;
	//cDesc.nonWalkableMode	= PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	desc.pReportCallback = &PhysicsEngine::PhysicsEngineMain::GetInstance();
	desc.pBehaviorCallback = &PhysicsEngine::PhysicsEngineMain::GetInstance();
	desc.volumeGrowth = 1.5f;

	PxController* pPxController = PhysicsEngine::PhysicsEngineMain::GetInstance().CreateCharacterContollerBox(desc, PxVec3(halfExtent.x, halfExtent.y, halfExtent.z), crouchHeight);

	PxRigidActor* pActor = pPxController->getActor();

	//callback 해야하는 owner의 포인터 set
	pActor->userData = pOwnerCallback;

	//Actor Collection에 추가
	RRIGIDACTOR_PTR actorID = PhysicsEngine::PhysicsEngineMain::GetInstance().IdGenerator(pActor);
	PhysicsEngine::PhysicsEngineMain::GetInstance().AddCollection(*pActor, actorID);

	//Actor를 가지고 있는 Controller의 포인터 set
	pPxController->setUserData(pActor);

	actorPtr = reinterpret_cast<CCT_PTR>(pPxController);


	return actorPtr;
}

CCT_PTR PhysicsEngineInterface::CreateCCTCapsule(ISimulationEventCallback* pOwnerCallback, Vector3 initPos, float radius, float height)
{
	using namespace physx;

	CCT_PTR actorPtr = NULL;


	float crouchHeight = height / 2;

	//CCT 생성에 필요한 정보 set
	PhysicsEngine::ControlledActorDesc desc;
	desc.type = PxControllerShapeType::Enum::eCAPSULE;
	desc.proxyDensity = 10.0f;
	desc.proxyScale = 1.0f; //Default: 0.9
	desc.position = PxExtendedVec3(initPos.x, initPos.y, initPos.z);
	desc.slopeLimit = 0.f;
	desc.contactOffset = 0.01f; //Default: 0.1
	desc.stepOffset = 0.f;
	desc.invisibleWallHeight = 0.f;
	desc.maxJumpHeight = 0.f;
	//cDesc.nonWalkableMode	= PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	desc.pReportCallback = &PhysicsEngine::PhysicsEngineMain::GetInstance();;
	desc.pBehaviorCallback = &PhysicsEngine::PhysicsEngineMain::GetInstance();;
	desc.volumeGrowth = 1.5f;

	PxController* pPxController = PhysicsEngine::PhysicsEngineMain::GetInstance().CreateCharacterContollerCapsule(desc, height, radius, crouchHeight);

	PxRigidActor* pActor = pPxController->getActor();

	//callback 해야하는 owner의 포인터 set
	pActor->userData = pOwnerCallback;

	//Actor Collection에 추가
	RRIGIDACTOR_PTR actorID = PhysicsEngine::PhysicsEngineMain::GetInstance().IdGenerator(pActor);
	PhysicsEngine::PhysicsEngineMain::GetInstance().AddCollection(*pActor, actorID);

	//Actor를 가지고 있는 Controller의 포인터 set
	pPxController->setUserData(pActor);

	actorPtr = reinterpret_cast<CCT_PTR>(pPxController);

	return actorPtr;
}

PhysicsEngineInterface::CCT_COLLISION_DIR::Enum PhysicsEngineInterface::UpdatePositionCCT(const CCT_PTR dest, Vector3 velocity, float dTime)
{
	PxController* pPxController = reinterpret_cast<PxController*>(dest);

	PxVec3 displacement = ToPxVec3(velocity);

	PxControllerCollisionFlags collisionFlags = PhysicsEngineMain::GetInstance().MoveCCT(pPxController, displacement, dTime);

	//방향 끼리 서로 겹칠 수 있게 #수정
	if (collisionFlags & PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		return CCT_COLLISION_DIR::DOWN;
	}
	else if (collisionFlags & PxControllerCollisionFlag::eCOLLISION_SIDES)
	{
		return CCT_COLLISION_DIR::SIDE;
	}
	else if (collisionFlags & PxControllerCollisionFlag::eCOLLISION_UP)
	{
		return CCT_COLLISION_DIR::UP;
	}
	else
	{
		return CCT_COLLISION_DIR::NONE;
	}
}

void PhysicsEngineInterface::GetPositionCCT(const CCT_PTR dest, Vector3& position)
{
	PxController* pPxController = reinterpret_cast<PxController*>(dest);

	/*PxTransform tm = getActor()->getGlobalPose();
	position = ToVector3(tm.p);*/

	PxExtendedVec3 vec3 = pPxController->getPosition();

	position = Vector3(vec3.x, vec3.y, vec3.z);
}

void PhysicsEngineInterface::GetFootPositionCCT(const CCT_PTR dest, Vector3& position)
{
	PxController* pPxController = reinterpret_cast<PxController*>(dest);

	/*PxTransform tm = getActor()->getGlobalPose();
	position = ToVector3(tm.p);*/

	PxExtendedVec3 vec3 = pPxController->getFootPosition();

	position = Vector3(vec3.x, vec3.y, vec3.z);
}

void PhysicsEngineInterface::DeleteCCT(const CCT_PTR dest)
{
	PxController* pPxController = reinterpret_cast<PxController*>(dest);

	pPxController->release();
}


void PhysicsEngineInterface::SetupCollisionFilterCCT(const CCT_PTR dest, FILTERGROUP::Enum filterGroup, FILTERGROUP::Enum filterMask)
{
	PxController* pPxController = reinterpret_cast<PxController*>(dest);

	PxRigidActor* pActor = pPxController->getActor();

	PhysicsEngine::PhysicsEngineMain::GetInstance().SetupFiltering(pActor, filterGroup, filterMask);
	PhysicsEngine::PhysicsEngineMain::GetInstance().SetupQueryFiltering(pActor, filterGroup, filterMask);
}

void PhysicsEngineInterface::SetupCollisionFilter(const RRIGIDACTOR_PTR dest, FILTERGROUP::Enum filterGroup, FILTERGROUP::Enum filterMask)
{
	PxRigidActor* pActor= PhysicsEngineMain::GetInstance().FindActorByID(dest)->is<PxRigidActor>();

	PhysicsEngine::PhysicsEngineMain::GetInstance().SetupFiltering(pActor, filterGroup, filterMask);
	PhysicsEngine::PhysicsEngineMain::GetInstance().SetupQueryFiltering(pActor, filterGroup, filterMask);
}

RRIGIDACTOR_PTR PhysicsEngineInterface::CreateRigidStaticBox(ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, Vector3 size)
{
	return CreateRigidActorBox(pOwnerCallback, position, rotation, size, true, false);
}

RRIGIDACTOR_PTR PhysicsEngineInterface::CreateRigidDynamicBox(ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, Vector3 size)
{
	return CreateRigidActorBox(pOwnerCallback, position, rotation, size, false, false);
}

RRIGIDACTOR_PTR PhysicsEngineInterface::CreateRigidKinematicBox(ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, Vector3 size)
{
	return CreateRigidActorBox(pOwnerCallback, position, rotation, size, false, true);
}

RRIGIDACTOR_PTR PhysicsEngineInterface::CreateRigidActorBox(ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, Vector3 size, bool isStatic, bool isKenematic)
{
	RRIGIDACTOR_PTR actorPtr = NULL;
	PxRigidActor* pxActor;

	PxTransform tm(ToPxTransform(position, rotation));

	pxActor = PhysicsEngineMain::GetInstance().CreateBox(tm, ToPxVec3(size/2.f),
		PxTransform(PxIdentity), PhysicsEngineInterface::DEFAULT_DENSITY, isStatic, isKenematic);

	pxActor->userData = pOwnerCallback;

	//기본 쿼리 처리를 위해 추가, 안하면 쿼리에 의한 상호작용이 작동하지 않음
	PhysicsEngineMain::GetInstance().SetupQueryFiltering(pxActor, PhysicsEngineMain::DEFAULT_QUERY_FILTERDATA, 0);

	actorPtr = PhysicsEngineMain::GetInstance().IdGenerator(pxActor);
	PhysicsEngineMain::GetInstance().AddCollection(*pxActor, actorPtr);

	return actorPtr;
}

RRIGIDACTOR_PTR PhysicsEngineInterface::CreateRigidStaticSphere(ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, float radius)
{
	return CreateRigidActorSphere(pOwnerCallback, position, rotation, radius, true, false);
}

RRIGIDACTOR_PTR PhysicsEngineInterface::CreateRigidDynamicSphere(ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, float radius)
{
	return CreateRigidActorSphere(pOwnerCallback, position, rotation, radius, false, false);
}

RRIGIDACTOR_PTR PhysicsEngineInterface::CreateRigidKinematicSphere(ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, float radius)
{
	return CreateRigidActorSphere(pOwnerCallback, position, rotation, radius, false, true);
}

RRIGIDACTOR_PTR PhysicsEngineInterface::CreateRigidActorSphere(ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, float radius, bool isStatic, bool isKenematic)
{
	RRIGIDACTOR_PTR actorPtr = NULL;
	PxRigidActor* pxActor;

	PxTransform tm(ToPxTransform(position, rotation));

	pxActor = PhysicsEngineMain::GetInstance().CreateSphere(tm, radius,
		PxTransform(PxIdentity), PhysicsEngineInterface::DEFAULT_DENSITY, isStatic, isKenematic);

	pxActor->userData = pOwnerCallback;

	PhysicsEngineMain::GetInstance().SetupQueryFiltering(pxActor, PhysicsEngineMain::DEFAULT_QUERY_FILTERDATA, 0);

	actorPtr = PhysicsEngineMain::GetInstance().IdGenerator(pxActor);
	PhysicsEngineMain::GetInstance().AddCollection(*pxActor, actorPtr);

	return actorPtr;
}

RRIGIDACTOR_PTR PhysicsEngineInterface::CreateRigidStaticCapsule(ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, float radius, float halfHeight)
{
	return CreateRigidActorCapsule(pOwnerCallback, position, rotation, radius, halfHeight, true, false);
}

RRIGIDACTOR_PTR PhysicsEngineInterface::CreateRigidDynamicCapsule(ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, float radius, float halfHeight)
{
	return CreateRigidActorCapsule(pOwnerCallback, position, rotation, radius, halfHeight, false, false);
}

RRIGIDACTOR_PTR PhysicsEngineInterface::CreateRigidKinematicCapsule(ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, float radius, float halfHeight)
{
	return CreateRigidActorCapsule(pOwnerCallback, position, rotation, radius, halfHeight, false, true);
}

RRIGIDACTOR_PTR PhysicsEngineInterface::CreateRigidActorCapsule(ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation, float radius, float halfHeight, bool isStatic, bool isKenematic)
{
	RRIGIDACTOR_PTR actorPtr = NULL;
	PxRigidActor* pxActor;

	PxTransform tm(ToPxTransform(position, rotation));

	pxActor = PhysicsEngineMain::GetInstance().CreateCapsule(tm, radius, halfHeight,
		PxTransform(PxIdentity), PhysicsEngineInterface::DEFAULT_DENSITY, isStatic, isKenematic);

	pxActor->userData = pOwnerCallback;

	PhysicsEngineMain::GetInstance().SetupQueryFiltering(pxActor, PhysicsEngineMain::DEFAULT_QUERY_FILTERDATA, 0);

	actorPtr = PhysicsEngineMain::GetInstance().IdGenerator(pxActor);
	PhysicsEngineMain::GetInstance().AddCollection(*pxActor, actorPtr);

	return actorPtr;
}

RRIGIDACTOR_PTR PhysicsEngineInterface::CreateStaticActorFromMeshData(ISimulationEventCallback* pOwnerCallback, const TRIANGLEMESH& meshdata,
	Vector3 position, Quaternion rotation, Vector3 scale
	)
{
	RRIGIDACTOR_PTR actorPtr = NULL;
	PxRigidActor* pxActor;

	PhysicsEngine::TriangleMeshData tiangleMeshData{
		meshdata.vertNum, meshdata.faceNum, (const float*)meshdata.verts, (PxU32*)meshdata.faces
	};

	PxTriangleMeshGeometry* pTriangleMesh = PhysicsEngine::PhysicsEngineMain::GetInstance().CreateTriAngleMeshGeometry(&tiangleMeshData, ToPxVec3(scale));

	pxActor = PhysicsEngineMain::GetInstance().CreateTriangleMesh(
		ToPxTransform(position, rotation), pTriangleMesh);

	pxActor->userData = pOwnerCallback;

	PhysicsEngineMain::GetInstance().SetupQueryFiltering(pxActor, PhysicsEngineMain::DEFAULT_QUERY_FILTERDATA, 0);

	actorPtr = PhysicsEngineMain::GetInstance().IdGenerator(pxActor);
	PhysicsEngineMain::GetInstance().AddCollection(*pxActor, actorPtr);

	return actorPtr;
}

RRIGIDACTOR_PTR PhysicsEngineInterface::CreateStaticActorFromMeshData(ISimulationEventCallback* pOwnerCallback, const TRIANGLEMESH& meshdata,
	Matrix mat
)
{
	RRIGIDACTOR_PTR actorPtr = NULL;
	PxRigidActor* pxActor;

	PhysicsEngine::TriangleMeshData tiangleMeshData{
		meshdata.vertNum, meshdata.faceNum, (const float*)meshdata.verts, (PxU32*)meshdata.faces
	};

	PxTriangleMeshGeometry* pTriangleMesh = PhysicsEngine::PhysicsEngineMain::GetInstance().CreateTriAngleMeshGeometry(&tiangleMeshData, PxVec3(1, 1, 1));

	pxActor = PhysicsEngineMain::GetInstance().CreateTriangleMesh(
		PxTransform(ToPxMat44(mat)), pTriangleMesh);

	pxActor->userData = pOwnerCallback;

	PhysicsEngineMain::GetInstance().SetupQueryFiltering(pxActor, PhysicsEngineMain::DEFAULT_QUERY_FILTERDATA, 0);

	actorPtr = PhysicsEngineMain::GetInstance().IdGenerator(pxActor);
	PhysicsEngineMain::GetInstance().AddCollection(*pxActor, actorPtr);

	return actorPtr;
}

RRIGIDACTOR_PTR PhysicsEngineInterface::CreateRigidStaticQuad(ISimulationEventCallback* pOwnerCallback, Vector3 position, Quaternion rotation,
	Vector2 size)
{
	RRIGIDACTOR_PTR actorPtr = NULL;
	PxRigidActor* pxActor;

	QUADVTX vtxdata;

	if (size != Vector2(1, 1))
	{
		vtxdata.topLeft.x *= size.x / 2.f;
		vtxdata.topRight.x *= size.x / 2.f;
		vtxdata.bottomLeft.x *= size.x / 2.f;
		vtxdata.bottomRight.x *= size.x / 2.f;

		vtxdata.topLeft.y *= size.y / 2.f;
		vtxdata.topRight.y *= size.y / 2.f;
		vtxdata.bottomLeft.y *= size.y / 2.f;
		vtxdata.bottomRight.y *= size.y / 2.f;
	}

	PhysicsEngine::TriangleMeshData quadMeshData{
		QUADVERTNNUM, QUADFACENUM, (const float*)&vtxdata, (PxU32*)QUADFACEDATA
	};
	PxTriangleMeshGeometry* pTriangleMesh = PhysicsEngine::PhysicsEngineMain::GetInstance().CreateTriAngleMeshGeometry(&quadMeshData, PxVec3(size.x, size.y, 1.f));

	pxActor = PhysicsEngineMain::GetInstance().CreateTriangleMesh(
		ToPxTransform(position, rotation), pTriangleMesh);

	pxActor->userData = pOwnerCallback;

	PhysicsEngineMain::GetInstance().SetupQueryFiltering(pxActor, PhysicsEngineMain::DEFAULT_QUERY_FILTERDATA, 0);

	actorPtr = PhysicsEngineMain::GetInstance().IdGenerator(pxActor);
	PhysicsEngineMain::GetInstance().AddCollection(*pxActor, actorPtr);

	return actorPtr;
}

RRIGIDACTOR_PTR PhysicsEngineInterface::CreateRigidStaticPlane(Vector3 normal, float distance)
{
	RRIGIDACTOR_PTR actorPtr = NULL;
	PxRigidActor* pxActor;

	PxPlane plane(ToPxVec3(normal), distance);
	pxActor = PhysicsEngineMain::GetInstance().CreatePlane(plane);

	PhysicsEngineMain::GetInstance().SetupQueryFiltering(pxActor, PhysicsEngineMain::DEFAULT_QUERY_FILTERDATA, 0);

	actorPtr = PhysicsEngineMain::GetInstance().IdGenerator(pxActor);
	PhysicsEngineMain::GetInstance().AddCollection(*pxActor, actorPtr);

	return actorPtr;
}

void PhysicsEngineInterface::SetSizeBox(const RRIGIDACTOR_PTR& dest, Vector3 size)
{
	PhysicsEngineMain& pEngine = PhysicsEngineMain::GetInstance();

	PxRigidActor* rigidActor = pEngine.FindActorByID(dest);
	
	pEngine.SetSizeRigidActorBox(rigidActor, ToPxVec3(size));
}


void PhysicsEngineInterface::SetSizeSphere(const RRIGIDACTOR_PTR& dest, float radius)
{
	PhysicsEngineMain& pEngine = PhysicsEngineMain::GetInstance();

	PxRigidActor* rigidActor = pEngine.FindActorByID(dest);

	pEngine.SetSizeRigidActorSphere(rigidActor, radius);
}

void PhysicsEngineInterface::SetSizeCapsule(const RRIGIDACTOR_PTR& dest, float radius, float halfHeight)
{
	PhysicsEngineMain& pEngine = PhysicsEngineMain::GetInstance();

	PxRigidActor* rigidActor = pEngine.FindActorByID(dest);

	pEngine.SetSizeRigidActorCapsule(rigidActor, radius, halfHeight);
}

void PhysicsEngineInterface::SetScaleTriangleMesh(const RRIGIDACTOR_PTR& dest, Vector3 scale)
{
	PhysicsEngineMain& pEngine = PhysicsEngineMain::GetInstance();

	PxRigidActor* rigidActor = pEngine.FindActorByID(dest);

	pEngine.SetScaleRigidActorTriangleMesh(rigidActor, ToPxVec3(scale));
}

//나중에 툴이랑 연동할 때 마저 구현 #수정
/*void PhysicsEngineInterface::SetSizeQuad(const RRIGIDACTOR_PTR& dest, Vector2 size)
{
	PhysicsEngineMain& pEngine = PhysicsEngineMain::GetInstance();

	PxRigidActor* rigidActor = pEngine.FindActorByID(dest);

	rigidActor->getGlobalPose();

	pEngine.SetScaleRigidActorTriangleMesh(rigidActor, ToPxVec3(scale));
}*/

//나중에 툴이랑 연동할 때 마저 구현 #수정
//void PhysicsEngineInterface::SetPlane(const RRIGIDACTOR_PTR& dest, Vector3 normal, float distance)
//{
//
//}


void PhysicsEngineInterface::SetTriggerShape(const RRIGIDACTOR_PTR& dest, bool isTrigger)
{
	PxRigidActor* rigidActor = PhysicsEngineMain::GetInstance().FindActorByID(dest);
	if (rigidActor)
	{
		PX_ASSERT(rigidActor);
		PhysicsEngineMain::GetInstance().SetTriggerShape(rigidActor, isTrigger);
	}
	
}

void PhysicsEngineInterface::SetSimulationShape(const RRIGIDACTOR_PTR& dest, bool isSimulated)
{
	SetTriggerShape(dest, !isSimulated);
}

void PhysicsEngineInterface::SetTriggerShapeCCT(const CCT_PTR& dest, bool isTrigger)
{
	PxController* controller =  reinterpret_cast<PxController*>(dest);

	PxRigidActor* rigidActor = controller->getActor();
	if (rigidActor)
	{
		PX_ASSERT(rigidActor);
		PhysicsEngineMain::GetInstance().SetTriggerShape(rigidActor, isTrigger);
	}
}

void PhysicsEngineInterface::SetSimulationShapeCCT(const CCT_PTR& dest, bool isSimulated)
{
	SetTriggerShapeCCT(dest, !isSimulated);
}

void PhysicsEngineInterface::SetActive(const RRIGIDACTOR_PTR& dest, bool isActive)
{
	PxRigidActor* rigidActor = PhysicsEngineMain::GetInstance().FindActorByID(dest);
	rigidActor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, !isActive);

	PxShape* shape;
	rigidActor->getShapes(&shape, 1);

#ifdef _DEBUG
	rigidActor->setActorFlag(PxActorFlag::eVISUALIZATION, isActive);
	shape->setFlag(PxShapeFlag::eVISUALIZATION, isActive);
#endif

	if (shape)
	{
		PxShapeFlags flag = shape->getFlags();

		if (isActive)
		{
			flag |= PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE;
			flag &= ~PxShapeFlag::eTRIGGER_SHAPE;
		}
		else
		{
			flag &= ~(PxShapeFlag::eSIMULATION_SHAPE | PxShapeFlag::eSCENE_QUERY_SHAPE);
			flag |= PxShapeFlag::eTRIGGER_SHAPE;
		}

		shape->setFlags(flag);
	}
}



void PhysicsEngineInterface::UpdateRigidDynamic(const RRIGIDACTOR_PTR& dest, Vector3 position, Quaternion rotation)
{
	PxRigidActor* rigidActor = PhysicsEngineMain::GetInstance().FindActorByID(dest);
	rigidActor->setGlobalPose(ToPxTransform(position, rotation));
}

void PhysicsEngineInterface::UpdateRigidDynamic(const RRIGIDACTOR_PTR& dest, Vector3 position, Vector3 rotation)
{
	Quaternion quatRot;
	EulerToQuaternion(rotation, quatRot);

	PxRigidActor* rigidActor = PhysicsEngineMain::GetInstance().FindActorByID(dest);
	rigidActor->setGlobalPose(ToPxTransform(position, quatRot));
}

void PhysicsEngineInterface::UpdateRigidKinematic(const RRIGIDACTOR_PTR& dest, Vector3 position, Quaternion rotation)
{
	PxRigidDynamic* pRigidDynamic = PhysicsEngineMain::GetInstance().FindActorByID(dest)->is<PxRigidDynamic>();

	pRigidDynamic->setKinematicTarget(ToPxTransform(position, rotation));
}

void PhysicsEngineInterface::UpdateRigidKinematic(const RRIGIDACTOR_PTR& dest, Vector3 position, Vector3 rotation)
{
	Quaternion quatRot;
	EulerToQuaternion(rotation, quatRot);

	PxRigidDynamic* pRigidDynamic = PhysicsEngineMain::GetInstance().FindActorByID(dest)->is<PxRigidDynamic>();

	pRigidDynamic->setKinematicTarget(ToPxTransform(position, quatRot));
}

void PhysicsEngineInterface::GetTransformRigidActor(const RRIGIDACTOR_PTR& dest, Vector3& position, Quaternion& rotation)
{
	PxRigidActor* rigidActor = PhysicsEngineMain::GetInstance().FindActorByID(dest);

	PxTransform tm = rigidActor->getGlobalPose();
	position = ToVector3(tm.p);
	rotation = ToQuaternion(tm.q);
}

void PhysicsEngineInterface::GetTransformRigidActor(const RRIGIDACTOR_PTR& dest, Vector3& position, Vector3& rotation)
{
	PxRigidActor* rigidActor = PhysicsEngineMain::GetInstance().FindActorByID(dest);

	PxTransform tm = rigidActor->getGlobalPose();
	position = ToVector3(tm.p);
	Quaternion quatRot = ToQuaternion(tm.q);

	QuaternionToEuler(quatRot, rotation);
}

void PhysicsEngineInterface::DeleteRigidActor(RRIGIDACTOR_PTR& dest)
{
	PhysicsEngineMain::GetInstance().RemoveActorByID(dest);
	dest = NULL;
}