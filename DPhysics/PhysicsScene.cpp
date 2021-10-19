#include "pch.h"

#include <ctime>

#include "ISimulationEventCallback.h"
#include "PhysicsEngine.h"
#include "PhysicsEngineMain.h"
#include "PhysicsScene.h"

using namespace physx;
using namespace PhysicsEngine;

#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}
#define DEFAULT_ALLOC(x)	PhysicsEngineMain::m_DefaultAllocator.allocate(x, 0, __FILE__, __LINE__)
#define	DEFAULT_FREE(x)	if(x)	{ PhysicsEngineMain::m_DefaultAllocator.deallocate(x); x = NULL;	}
#define DEFAULT_NEW(x)	new(#x, __FILE__, __LINE__) x

PX_INLINE void addForceAtPosInternal(PxRigidBody & body, const PxVec3 & force, const PxVec3 & pos, PxForceMode::Enum mode, bool wakeup)
{
	const PxTransform globalPose = body.getGlobalPose();
	const PxVec3 centerOfMass = globalPose.transform(body.getCMassLocalPose().p);

	const PxVec3 torque = (pos - centerOfMass).cross(force);
	body.addForce(force, mode, wakeup);
	body.addTorque(torque, mode, wakeup);
}

static void addForceAtLocalPos(PxRigidBody & body, const PxVec3 & force, const PxVec3 & pos, PxForceMode::Enum mode, bool wakeup = true)
{
	//transform pos to world space
	const PxVec3 globalForcePos = body.getGlobalPose().transform(pos);

	addForceAtPosInternal(body, force, globalForcePos, mode, wakeup);
}

void defaultCCTInteraction(const PxControllerShapeHit & hit)
{
	PxRigidDynamic* actor = hit.shape->getActor()->is<PxRigidDynamic>();
	if (actor)
	{
		if (actor->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC)
			return;

		// We only allow horizontal pushes. Vertical pushes when we stand on dynamic objects creates
		// useless stress on the solver. It would be possible to enable/disable vertical pushes on
		// particular objects, if the gameplay requires it.
		const PxVec3 upVector = hit.controller->getUpDirection();
		const PxF32 dp = hit.dir.dot(upVector);
		//		shdfnd::printFormatted("%f\n", fabsf(dp));
		if (fabsf(dp) < 1e-3f)
			//		if(hit.dir.y==0.0f)
		{
			const PxTransform globalPose = actor->getGlobalPose();
			const PxVec3 localPos = globalPose.transformInv(toVec3(hit.worldPos));
			::addForceAtLocalPos(*actor, hit.dir * hit.length * 100.0f, localPos, PxForceMode::eACCELERATION);
		}
	}
}

/////////필터쉐이터 설정
//https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxapi/files/group__physics.html#ga235380b2a5b3cc89a4cd89d089d72b33
//몇가지 경우에 해당하는 Pair는 무시된다
//Pair of static rigid actors 정적 강체 액터 쌍
//static rigid & kinematic actor(트리거가 아니거나 PxPairFilteringMode::eKEEP를 통해 명시 적으로 활성화 된 경우 제외)
//Two kinematic actors(하나가 트리거가 아니거나 PxPairFilteringMode::eKEEP를 통해 명시 적으로 활성화 된 경우 제외)
//Two jointed rigid bodiesand the joint was defined to disable collision
//Two articulation links if connected through an articulation joint

//https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxguide/Manual/MigrationTo40.html#core-physx
//PxSceneFlag::eDEPRECATED_TRIGGER_TRIGGER_REPORTS has been removed.
//It is recommended to use a trigger and a non - trigger shape,
//both with the same geometry and local pose,
//to emulate getting notifications for overlaps between trigger shapes.
PxFilterFlags SceneFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	/*auto ret = PxDefaultSimulationFilterShader(attributes0, filterData0,
		attributes1, filterData1,
		pairFlags, constantBlock, constantBlockSize);*/

		// all initial and persisting reports for everything, with per-point data
	pairFlags
		= PxPairFlag::eDETECT_DISCRETE_CONTACT
		| PxPairFlag::eNOTIFY_TOUCH_FOUND
		| PxPairFlag::eNOTIFY_TOUCH_LOST
		| PxPairFlag::eNOTIFY_CONTACT_POINTS;

	if (!PxFilterObjectIsKinematic(attributes0) || !PxFilterObjectIsKinematic(attributes1))
	{
		pairFlags |= PxPairFlag::eSOLVE_CONTACT;
	}

	if (!PxFilterObjectIsTrigger(attributes0) && !PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
	}

	if ((filterData0.word0 != 0 || filterData1.word0 != 0) &&
		!(filterData0.word0 & filterData1.word1 || filterData1.word0 & filterData0.word1))
		return PxFilterFlag::eSUPPRESS;

	return PxFilterFlags();
}

// Implements PxUserControllerHitReport

//CCT - Actor(CCT가 move 할 때 생기는) 충돌
void PhysicsScene::onShapeHit(const PxControllerShapeHit& hit)
{
	defaultCCTInteraction(hit);
}

//CCT - CCT 충돌
void PhysicsScene::onControllerHit(const PxControllersHit& hit)
{
	PxRigidDynamic* actor = hit.controller->getActor()->is<PxRigidDynamic>();
	PxRigidDynamic* otherActor = hit.other->getActor()->is<PxRigidDynamic>();

	if (actor)
	{
		ISimulationEventCallback* callback1 = reinterpret_cast<ISimulationEventCallback*>(actor->userData);
		ISimulationEventCallback* callback2 = reinterpret_cast<ISimulationEventCallback*>(otherActor->userData);

		//isActive 체크
		if (callback1->m_IsActive && callback2->m_IsActive)
			return;

		PxExtendedVec3 worldPos = hit.worldPos;

		//게임 로직에서도 사용 가능하게 충돌 정보 복사
		PhysicsEngine::CharaterControllerHit cctHit1;
		cctHit1.controller = callback1; //충돌한 자신의 object
		cctHit1.otherController = callback2; //충돌한 상대의 object
		cctHit1.worldPos = Vector3(hit.worldPos.x, hit.worldPos.y, hit.worldPos.z);
		cctHit1.worldNormal = ToVector3(hit.worldNormal);
		cctHit1.dir = ToVector3(hit.dir);
		cctHit1.length = hit.length;

		//callback2 호출에서 사용할 정보 복사
		PhysicsEngine::CharaterControllerHit cctHit2 = cctHit1;
		//자신과 상대 오브젝트 정보만 swap해서 사용
		cctHit2.controller = callback2; //충돌한 상대의 object
		cctHit2.otherController = callback1; //충돌한 자신의 object

		//pair 단위로 호출
		callback1->OnControllerHit(cctHit1);
		callback2->OnControllerHit(cctHit2);
	}
}

// Implements PxControllerBehaviorCallback
PxControllerBehaviorFlags PhysicsScene::getBehaviorFlags(const PxShape& shape, const PxActor& actor)
{
	return PxControllerBehaviorFlags(0);
}

PxControllerBehaviorFlags PhysicsScene::getBehaviorFlags(const PxController& controller)
{
	return PxControllerBehaviorFlags(0);
}

PxControllerBehaviorFlags PhysicsScene::getBehaviorFlags(const PxObstacle& obstacle)
{
	return PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT | PxControllerBehaviorFlag::eCCT_SLIDE;
}

// Implements PxQueryFilterCallback


PxQueryHitType::Enum PhysicsScene::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags)
{

	//부딫힌 대상이 TRIGGER_SHAPE면 통과
	if (shape->getFlags() & PxShapeFlag::eTRIGGER_SHAPE)
		return PxQueryHitType::eNONE;

	//충돌 필터 검사, raycast(), overlap()에서 사용
	PxFilterData ohterFilterData = shape->getQueryFilterData();

	if ((filterData.word0 != 0 || ohterFilterData.word0 != 0) &&
		!((filterData.word0 & ohterFilterData.word1) ||
			(filterData.word1 & ohterFilterData.word0))
		)
		return PxQueryHitType::eNONE;

	return PxQueryHitType::eBLOCK;
}

PxQueryHitType::Enum PhysicsScene::postFilter(const PxFilterData& filterData, const PxQueryHit& hit)
{
	//쿼리 히트 actor, shape에 따라 다르게 처리, 기본 blocking
	return PxQueryHitType::eBLOCK;
}

// Implements PxControllerFilterCallback
//false : 통과, true : 블록
bool PhysicsScene::filter(const PxController& a, const PxController& b)
{
	PxRigidDynamic* actorA = a.getActor();
	PxRigidDynamic* actorB = b.getActor();

	PxShape* shapeA = NULL;
	PxShape* shapeB = NULL;

	//PxController는 capsule 1개만 가지고 있으므로 하나만 불러온다
	actorA->getShapes(&shapeA, 1);
	actorB->getShapes(&shapeB, 1);

	PxFilterData dataA = shapeA->getQueryFilterData();
	PxFilterData dataB = shapeB->getQueryFilterData();

	PxShapeFlags queryFlagsA = shapeA->getFlags();
	PxShapeFlags queryFlagsB = shapeB->getFlags();

	// let triggers through
	if ((queryFlagsA & PxShapeFlag::eTRIGGER_SHAPE) || (queryFlagsB & PxShapeFlag::eTRIGGER_SHAPE))
	{
		return false;
	}

	//SCENE_QUERY_SHAPE가 꺼지면 처리하지 않음(통과)
	if (!(queryFlagsA & PxShapeFlag::eSCENE_QUERY_SHAPE) || !(queryFlagsB & PxShapeFlag::eSCENE_QUERY_SHAPE))
	{
		return false;
	}

	// Filter function, word0 is own group, word1 is target group( equal ).
	// If detect, do not pass.

	PxU32 pass = (dataA.word0 != 0 || dataB.word0 != 0) &&
		!((dataA.word0 & dataB.word1) || (dataA.word1 & dataB.word0));

	if (pass)
		return false;

	return true;
}


void PhysicsScene::Initialize()
{
	const PhysicsEngineMain& engine = PhysicsEngineMain::GetInstance();

	m_pPhysics = engine.m_pPhysics;
	m_pFoundation = engine.m_pFoundation;
	m_pCooking = engine.m_pCooking;
	m_pDispatcher = engine.m_pDispatcher;;
	m_pPVD = engine.m_pPVD;

	///////Scene Descriptor
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	//sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f); //무중력 
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	sceneDesc.filterShader = SceneFilterShader;
	sceneDesc.simulationEventCallback = this;
	//sceneDesc.flags |= PxSceneFlag::eREQUIRE_RW_LOCK;

	//Scene 생성
	m_pScene = m_pPhysics->createScene(sceneDesc);

	//캐릭터 컨트롤러 매니저 추가, 캐릭터 컨트롤러(CCT) 사용할 때 필요한 과정
	m_pControllerManager = PxCreateControllerManager(*m_pScene);
	m_pControllerManager->setOverlapRecoveryModule(true);
	m_pControllerManager->setTessellation(true, 100.f);
	m_pControllerManager->setPreciseSweeps(true);

	m_pControllerManager->setDebugRenderingFlags(PxControllerDebugRenderFlag::eCACHED_BV);
	//m_pControllerManager->computeInteractions()

	//pvd 클라이언트에 Scene 연결
	if (PhysicsEngineMain::m_IsPVDconnet)
	{
		PxPvdSceneClient* pvdClient = m_pScene->getScenePvdClient();

		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	//물리 마테리얼, 마찰 계수(friction, 이동(dynamic)/정지(static)), 반발 계수(coefficient of restitution) 설정
	m_pPhysicsMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.1f);
	m_pPhysicsMaterialStatic = m_pPhysics->createMaterial(0.5f, 0.5f, 0.1f);
}

void PhysicsScene::Update(float dTime)
{
	float stepSize = DEFAULT_STEPSIZE; //16.6666ms

	m_AccumTime += dTime;

	if (m_AccumTime < DEFAULT_STEPSIZE)
	{
		return;
	}

	//병목이 생겨 기존 스텝시간(16.6666ms) 보다 루프가 길어지면 
	//시뮬레이션 스텝 간격을 그냥 deltaTime 으로 설정해서 시뮬레이션 한다.
	if (dTime > DEFAULT_STEPSIZE)
	{
		stepSize = dTime;
	}

	m_AccumTime -= stepSize;

	//OnTriggerStay
	for (const PxTriggerPair& pair : m_pTriggerPairSet)
	{
		ISimulationEventCallback* callback1 = nullptr;
		ISimulationEventCallback* callback2 = nullptr;

		if (pair.triggerActor->userData && pair.otherActor->userData)
		{
			callback1 = reinterpret_cast<ISimulationEventCallback*>(pair.triggerActor->userData);
			callback2 = reinterpret_cast<ISimulationEventCallback*>(pair.otherActor->userData);

			if (callback1 && callback2)
			{
				//중간에 삭제된 shape(actor) 예외처리
				if (pair.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER |
					PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
					continue;

				callback1->OnTriggerStay(callback2);
				callback2->OnTriggerStay(callback1);
			}
		}
	}

	m_pScene->simulate(stepSize);
	m_pScene->fetchResults(true);
}

void PhysicsScene::Finalize()
{
	PX_RELEASE(m_pControllerManager);
	PX_RELEASE(m_pScene);
}

void PhysicsScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	//userdata 참조되어 정의된 ISimulationEventCallback의 함수를 실행
	//userdata에 ISimulationEventCallback를 상속한 객체포인터가 있어야 함
	for (PxU32 i = 0; i < count; i++)
	{
		const PxTriggerPair& tp = pairs[i];
		//https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxapi/files/structPxTriggerPair.html
					//The shape pointers might reference deleted shapes.
					//This will be the case if PxPairFlag::eNOTIFY_TOUCH_LOST events were requested for the pair and one of the involved shapes gets deleted.
					//Check the flags member to see whether that is the case. Do not dereference a pointer to a deleted shape.
					//The pointer to a deleted shape is only provided such that user data structures which might depend on the pointer value can be updated.

					//요약 : 중간에 삭제된 shape를 PxTriggerPair::flags(PxTriggerPairFlag)로 체크 해서 예외처리 하기
		if (tp.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER |
			PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
		{
			m_pTriggerPairSet.erase(pairs[i]);
			continue;
		}

		ISimulationEventCallback* callback1 = reinterpret_cast<ISimulationEventCallback*>(pairs[i].triggerActor->userData);
		ISimulationEventCallback* callback2 = reinterpret_cast<ISimulationEventCallback*>(pairs[i].otherActor->userData);

		//isActive 체크
		if (callback1->m_IsActive && callback2->m_IsActive)
		{
			if (tp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				if (callback1 && callback2)
				{
					callback1->OnTriggerEnter(callback2);
					callback2->OnTriggerEnter(callback1);

					m_pTriggerPairSet.insert(pairs[i]);
					break;
				}
			}
			else if (tp.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				if (callback1 && callback2)
				{

					callback1->OnTriggerExit(callback2);
					callback2->OnTriggerExit(callback1);

					m_pTriggerPairSet.erase(pairs[i]);
					break;
				}
			}
		}
	}
}


void PhysicsScene::DeleteTriggerPair(PxRigidActor* actor)
{
	if (actor)
	{
		for (const PxTriggerPair& pair : m_pTriggerPairSet)
		{
			if (pair.triggerActor == actor ||
				pair.otherActor == actor)
			{
				m_pTriggerPairSet.erase(pair);
				return;
			}
		}
	}
}

void PhysicsScene::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	//userdata 참조되어 정의된 ISimulationEventCallback의 함수를 실행
	//userdata에 ISimulationEventCallback를 상속한 객체포인터가 있어야 함

	ISimulationEventCallback* callback1 = reinterpret_cast<ISimulationEventCallback*>(pairHeader.actors[0]->userData);
	ISimulationEventCallback* callback2 = reinterpret_cast<ISimulationEventCallback*>(pairHeader.actors[1]->userData);

	for (PxU32 i = 0; i < nbPairs; i++)
	{
		//https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxapi/files/structPxContactPairHeader.html#ae3a7e00ec5edf59b946196becc840182
		//The actor pointers might reference deleted actors.
		//This will be the case if PxPairFlag::eNOTIFY_TOUCH_LOST or PxPairFlag::eNOTIFY_THRESHOLD_FORCE_LOST events
		//were requested for the pair and one of the involved actors gets deleted or removed from the scene.
		//Check the flags member to see whether that is the case. Do not dereference a pointer to a deleted actor.
		//The pointer to a deleted actor is only provided such that user data structures which might depend on the pointer value can be updated.

		//요약 : 중간에 삭제된 actor를 PxContactPair::flags(PxContactPairHeaderFlags)로 체크 해서 예외 처리 하기
		const PxContactPair& cp = pairs[i];
		if (pairHeader.flags & (PxContactPairHeaderFlag::eREMOVED_ACTOR_0 |
			PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
			continue;

		//IsActive 지원
		if (callback1->m_IsActive && callback2->m_IsActive)
		{
			if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			{
				if (callback1 && callback2)
				{
					callback1->OnCollisionStay(callback2);
					callback2->OnCollisionStay(callback1);

					break;
				}
			}
			else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				if (callback1 && callback2)
				{
					callback1->OnCollisionEnter(callback2);
					callback2->OnCollisionEnter(callback1);

					break;
				}
			}
			else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				if (callback1 && callback2)
				{
					callback1->OnCollisionExit(callback2);
					callback2->OnCollisionExit(callback1);

					break;
				}
			}
		}
	}
}

void PhysicsScene::SetTriggerShape(PxRigidActor* actor, bool isTrigger)
{
	if (actor)
	{
		PxShape* ctrlShape;
		int shapesNum = actor->getNbShapes();


		for (int i = 1; i <= shapesNum; i++)
		{
			actor->getShapes(&ctrlShape, 1);
			//eSIMULATION_SHAPE와 eTRIGGER_SHAPE는 둘다 set이 되면 안됨
			ctrlShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
			ctrlShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
		}

		PxRigidBody* rigidBody = actor->is<PxRigidBody>();
		if (!(rigidBody->getRigidBodyFlags().isSet(PxRigidBodyFlag::eKINEMATIC)))
		{
			actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
		}
	}
}

bool PhysicsScene::RaycastSingle(PxVec3 origin, PxVec3 unitDir, PxReal distance,
	PxSceneQueryFlags outputFlags,
	PxRaycastHit& hit, const PxQueryFilterData& filterData, const PxSceneQueryCache* cache)
{
	return PxSceneQueryExt::raycastSingle(*m_pScene, origin, unitDir,
		distance, outputFlags, hit, filterData, this, cache);
}

PxI32 PhysicsScene::OverlapMultiple(const PxGeometry& geo, PxTransform tm,
	PxOverlapHit* hitBuffer, PxU32 	hitBufferSize,
	const PxQueryFilterData& filterData)
{
	return PxSceneQueryExt::overlapMultiple(*m_pScene, geo,
		tm, hitBuffer, hitBufferSize, filterData, this);
}