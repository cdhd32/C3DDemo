#include "pch.h"

#include <ctime>
#include "ConsoleWindow.h"

#include "PhysicsEngine.h"

#include "PhysicsEngineMain.h"




//SnippetUtils.h �� �ִ� release ��ũ��
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}
using namespace physx;
using namespace PhysicsEngine;
using namespace DirectX::SimpleMath;

PxDefaultAllocator PhysicsEngineMain::m_DefaultAllocator;

#define DEFAULT_ALLOC(x)	m_DefaultAllocator.allocate(x, 0, __FILE__, __LINE__)
#define	DEFAULT_FREE(x)	if(x)	{ m_DefaultAllocator.deallocate(x); x = NULL;	}
#define DEFAULT_NEW(x)	new(#x, __FILE__, __LINE__) x

PxDefaultErrorCallback PhysicsEngineMain::m_DefaultErrorCallback;

PxFoundation* PhysicsEngineMain::m_pFoundation = nullptr;

PxPvd* PhysicsEngineMain::m_pPVD = nullptr;

PxPvdTransport* PhysicsEngineMain::m_pPVDTransport = nullptr;

PxPhysics* PhysicsEngineMain::m_pPhysics = nullptr;

PxCooking* PhysicsEngineMain::m_pCooking = nullptr;

PxDefaultCpuDispatcher* PhysicsEngineMain::m_pDispatcher = nullptr;

PhysicsEngineMain* PhysicsEngineMain::m_pPhysicsEngine = nullptr;

void PhysicsEngineMain::Initialize()
{
	//���� Ȯ�� �޸� �Ҵ� �� ���� �ݹ� ���� �����ϴ� ���(Foundation) �̱��� ��ü
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocator, m_DefaultErrorCallback);

	//PhysX�� ����� �� ��ü, ���� ����ſ� ����(tcp/ip) �����Ͽ� ����� ������ ������
	//pvd�� ��Ʈ��ũ�� �����ϸ� �������� �ȵǴ� ���� ���� ���� ���� ���� #���� �ʿ�
	//https://github.com/NVIDIAGameWorks/PhysX/issues/224


	bool isPVDConnect = false;
#ifdef _DEBUG
	m_pPVD = PxCreatePvd(*m_pFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	//PxPvdTransport* transport = PxDefaultPvdFileTransportCreate("D:\GA1stFinal_MaterialNinja\5_Project\MatNinjaTest_DHChoi\PhysicalEngineDemo_DH\x64\Debug\pvdfile.pvd");
	isPVDConnect = m_pPVD->connect(*transport, PxPvdInstrumentationFlag::eALL);
#endif

	//�⺻ ���� ����(���� ����/����/�ӵ�)
	PxTolerancesScale scale;
	scale.length = 1.f;
	scale.speed = 9.81f;

	//top-level PxPhysics object
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, scale, true, m_pPVD);

	//�뷮�� PhysX �����͵��� ����, ��ȯ, ����ȭ�ϴ� ��ü
	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, scale);


///////Cooking �Ķ���� ����, ����� ����� #����

	//�⺻ ������ ��ŷ �Ķ���� ��������
	m_CockingParamsDefault = m_pCooking->getParams();
	m_CockingParamsDefault.scale = scale;

	//�⺻ Cooking mode, ���� cooking ���� �浹 ó��, WELD ó���� ��
	m_CockingParamsDefault.meshPreprocessParams.set(PxMeshPreprocessingFlag::eWELD_VERTICES);
	m_CockingParamsDefault.meshPreprocessParams.set(PxMeshPreprocessingFlag::eFORCE_32BIT_INDICES);

	m_CockingParamsDefault.meshWeldTolerance = 10.f;
	m_CockingParamsDefault.midphaseDesc = PxMeshMidPhase::Enum::eBVH34;
	m_CockingParamsDefault.midphaseDesc.mBVH34Desc.numPrimsPerLeaf = 4;
	
	//�⺻ ������ ��ŷ �Ķ���� ��������
	m_CockingParamsDirect = m_pCooking->getParams();
	m_CockingParamsDirect.scale = scale;
	
	//��� ���� �ϴ� �ɼ����� ����
	// disable mesh cleaning - perform mesh validation on development configurations
	m_CockingParamsDirect.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
	// disable edge precompute, edges are set for each triangle, slows contact generation
	m_CockingParamsDirect.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
	
	//�⺻ �Ķ���ͷ� set
	m_pCooking->setParams(m_CockingParamsDefault);

	//�浹 ó���� �ϴ� leaf �� �⺻ ���� ���� #����
	//������ ��Ÿ�� ������ ������ cooking ������ ��������
	//�ݴ�� ������ ��Ÿ�� ������ �������� cooking ������ ������
	//meshCookingHint = PxMeshCookingHint::eCOOKING_PERFORMANCE �Ǵ� eSIM_PERFORMANCE �� ���� �Ҽ� ������
	//midphaseDesc�� PxBVH33MidphaseDesc(mBVH33Desc) �϶� ����
	//��� 4~15 ���� ���� ����ȭ�� ��ų �� ����
	m_CockingParamsDirect.midphaseDesc.mBVH34Desc.numPrimsPerLeaf = 4;



///////Scene Descriptor
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	//sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f); //���߷� 
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	//��� cpu worker ������� �װ͵��� �����ϴ� Dispatcher ����, �ʿ��ϸ� ���� ����� �͵� ����
	m_pDispatcher = PxDefaultCpuDispatcherCreate(m_WorkerThreadNum);
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	//#GPU ��üó�� �÷���
	//sceneDesc.flags = PxSceneFlags(PxSceneFlag::eENABLE_GPU_DYNAMICS);

	CustomizeSceneDesc(sceneDesc);

	//Scene ����
	m_pScene = m_pPhysics->createScene(sceneDesc);

	//ĳ���� ��Ʈ�ѷ� �Ŵ��� �߰�, ĳ���� ��Ʈ�ѷ�(CCT) ����� �� �ʿ��� ����
	m_pControllerManager = PxCreateControllerManager(*m_pScene);
	m_pControllerManager->setOverlapRecoveryModule(true);
	m_pControllerManager->setTessellation(true, 100.f);
	m_pControllerManager->setPreciseSweeps(true);
	
	m_pControllerManager->setDebugRenderingFlags(PxControllerDebugRenderFlag::eCACHED_BV);
	//m_pControllerManager->computeInteractions()

	//pvd
	if (isPVDConnect)
	{
		//pvd Ŭ���̾�Ʈ�� Scene ����
		PxPvdSceneClient* pvdClient = m_pScene->getScenePvdClient();

		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);

		}
	}


	//���� ���׸���, ���� ���(friction, �̵�(dynamic)/����(static)), �ݹ� ���(coefficient of restitution) ����
	m_pPhysicsMaterial = m_pPhysics->createMaterial(0.5f, 0.5f, 0.1f);
	m_pPhysicsMaterialStatic = m_pPhysics->createMaterial(0.5f, 0.5f, 0.1f);

	//���� gen
	std::srand(std::time(nullptr));

	//��ü �����̳� ����
	m_pCollection = PxCreateCollection();
}

void PhysicsEngineMain::Update()
{
	StepPhysics(true);
}

void PhysicsEngineMain::Finalize()
{
	CleanupPhysics(true);
}

//simulation loop 60 simulation per second
//https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxguide/Manual/RigidBodyOverview.html#the-simulation-loop
void PhysicsEngineMain::StepPhysics(bool isUpdate)
{
	static float accumulator = 0.0f;
	float stepSize = 1.0f / 60.0f; //16.6666ms
	static uint64_t lastTime = 0;

	static int stepCnt = 0;
	static uint64_t lastTimePerSec = 0;

	uint64_t curTime = std::clock();
	float deltaTime = (curTime - lastTime) / 1000.f;
	accumulator += deltaTime;

	if (accumulator < stepSize)
	{
		lastTime = curTime;
		return;
	}

	//OnTriggerStay �߸��� �ּ� ���� ���� ������ ������
	//for (const PxTriggerPair& pair : m_pTriggerPairSet)
	//{
	//	ISimulationEventCallback* callback1 = nullptr;
	//	ISimulationEventCallback* callback2 = nullptr;

	//	if (pair.triggerActor->userData && pair.otherActor->userData)
	//	{
	//		callback1 = reinterpret_cast<ISimulationEventCallback*>(pair.triggerActor->userData);
	//		callback2 = reinterpret_cast<ISimulationEventCallback*>(pair.otherActor->userData);

	//		if (callback1 && callback2)
	//		{
	//			//�߰��� ������ shape(actor) ����ó��
	//			if (pair.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER |
	//				PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
	//				continue;

	//			callback1->OnTriggerStay(callback2);
	//			callback2->OnTriggerStay(callback1);
	//		}
	//	}

	//}

	//������ ���� ���� ���ܽð�(16.6666ms) ���� ������ ������� 
	//�ùķ��̼� ���� ������ �׳� deltaTime ���� �����ؼ� �ùķ��̼� �Ѵ�.
	if (deltaTime > stepSize)
	{
		stepSize = deltaTime;
	}

	accumulator -= stepSize;

	m_pScene->simulate(stepSize);
	m_pScene->fetchResults(isUpdate);

	lastTime = curTime;

	stepCnt++;

}

void PhysicsEngineMain::CleanupPhysics(bool isCleanUp)
{
	PX_RELEASE(m_pCollection);
	PX_RELEASE(m_pControllerManager);
	PX_RELEASE(m_pCooking);
	PX_RELEASE(m_pScene);
	PX_RELEASE(m_pDispatcher);
	PX_RELEASE(m_pPhysics);
	
	if (m_pPVD)
	{
		PxPvdTransport* transport = m_pPVD->getTransport();
		m_pPVD->release();	m_pPVD = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(m_pFoundation);
	
}

/////////���ͽ����� ����
//https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxapi/files/group__physics.html#ga235380b2a5b3cc89a4cd89d089d72b33
//��� ��쿡 �ش��ϴ� Pair�� ���õȴ�
//Pair of static rigid actors ���� ��ü ���� ��
//static rigid & kinematic actor(Ʈ���Ű� �ƴϰų� PxPairFilteringMode::eKEEP�� ���� ��� ������ Ȱ��ȭ �� ��� ����)
//Two kinematic actors(�ϳ��� Ʈ���Ű� �ƴϰų� PxPairFilteringMode::eKEEP�� ���� ��� ������ Ȱ��ȭ �� ��� ����)
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

void PhysicsEngineMain::CustomizeSceneDesc(PxSceneDesc& sceneDesc)
{
	sceneDesc.filterShader = SceneFilterShader;
	//�Ϲ� actor(shape) �� �浹 �ݹ�
	sceneDesc.simulationEventCallback = this;
	//sceneDesc.flags |= PxSceneFlag::eREQUIRE_RW_LOCK;
}

///////////////////////////////////////////////////////////////////////////////
void PhysicsEngineMain::SetupFiltering(PxRigidActor* actor, const PxFilterData& filterData)
{
	//PxFilterData filterData;
	//filterData.word0 = filterGroup; // word0 = own ID
	//filterData.word1 = filterMask;	// word1 = ID mask to filter pairs that trigger a contact callback;
	
	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)DEFAULT_ALLOC(sizeof(PxShape*) * numShapes);
	actor->getShapes(shapes, numShapes);
	for (PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}
	DEFAULT_FREE(shapes);
}

void PhysicsEngineMain::SetupQueryFiltering(PxRigidActor* actor, const PxFilterData& filterData)
{
	//PxFilterData filterData;
	//filterData.word0 = filterGroup; // word0 = own ID
	//filterData.word1 = filterMask;	// word1 = ID mask to filter pairs that trigger a query contact callback;

	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)DEFAULT_ALLOC(sizeof(PxShape*) * numShapes);
	actor->getShapes(shapes, numShapes);
	for (PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setQueryFilterData(filterData);
	}

	DEFAULT_FREE(shapes);
}



///////////////////////////////////////////////////////////////////////////////



void PhysicsEngineMain::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	//userdata �����Ǿ� ���ǵ� ISimulationEventCallback�� �Լ��� ����
	//userdata�� ISimulationEventCallback�� ����� ��ü�����Ͱ� �־�� ��

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

		//��� : �߰��� ������ actor�� PxContactPair::flags(PxContactPairHeaderFlags)�� üũ �ؼ� ���� ó�� �ϱ�
		const PxContactPair& cp = pairs[i];
		if (pairHeader.flags & (PxContactPairHeaderFlag::eREMOVED_ACTOR_0 |
			PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
			continue;

		//IsActive ����
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

void PhysicsEngineMain::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	//userdata �����Ǿ� ���ǵ� ISimulationEventCallback�� �Լ��� ����
	//userdata�� ISimulationEventCallback�� ����� ��ü�����Ͱ� �־�� ��
	for (PxU32 i = 0; i < count; i++)
	{
		const PxTriggerPair& tp = pairs[i];
		//https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxapi/files/structPxTriggerPair.html
					//The shape pointers might reference deleted shapes.
					//This will be the case if PxPairFlag::eNOTIFY_TOUCH_LOST events were requested for the pair and one of the involved shapes gets deleted.
					//Check the flags member to see whether that is the case. Do not dereference a pointer to a deleted shape.
					//The pointer to a deleted shape is only provided such that user data structures which might depend on the pointer value can be updated.

					//��� : �߰��� ������ shape�� PxTriggerPair::flags(PxTriggerPairFlag)�� üũ �ؼ� ����ó�� �ϱ�
		if (tp.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER |
			PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
		{
			//m_pTriggerPairSet.erase(pairs[i]);
			continue;
		}

		ISimulationEventCallback* callback1 = reinterpret_cast<ISimulationEventCallback*>(pairs[i].triggerActor->userData);
		ISimulationEventCallback* callback2 = reinterpret_cast<ISimulationEventCallback*>(pairs[i].otherActor->userData);
		
		//isActive üũ
		if (callback1->m_IsActive && callback2->m_IsActive)
		{
			if (tp.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				if (callback1 && callback2)
				{
					callback1->OnTriggerEnter(callback2);
					callback2->OnTriggerEnter(callback1);

					//m_pTriggerPairSet.insert(pairs[i]);
					break;
				}
			}
			else if (tp.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				if (callback1 && callback2)
				{

					callback1->OnTriggerExit(callback2);
					callback2->OnTriggerExit(callback1);

					//m_pTriggerPairSet.erase(pairs[i]);
					break;
				}
			}
		}
	}
}

PX_INLINE void addForceAtPosInternal(PxRigidBody& body, const PxVec3& force, const PxVec3& pos, PxForceMode::Enum mode, bool wakeup)
{
	const PxTransform globalPose = body.getGlobalPose();
	const PxVec3 centerOfMass = globalPose.transform(body.getCMassLocalPose().p);

	const PxVec3 torque = (pos - centerOfMass).cross(force);
	body.addForce(force, mode, wakeup);
	body.addTorque(torque, mode, wakeup);
}

static void addForceAtLocalPos(PxRigidBody& body, const PxVec3& force, const PxVec3& pos, PxForceMode::Enum mode, bool wakeup = true)
{
	//transform pos to world space
	const PxVec3 globalForcePos = body.getGlobalPose().transform(pos);

	addForceAtPosInternal(body, force, globalForcePos, mode, wakeup);
}

void PhysicsEngine::defaultCCTInteraction(const PxControllerShapeHit& hit)
{
	//�΋H�� ����� TRIGGER_SHAPE�� ���
	if (hit.shape->getFlags() & PxShapeFlag::eTRIGGER_SHAPE)
		return;

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

// Implements PxUserControllerHitReport

//CCT - Actor(CCT�� move �� �� �����) �浹
void PhysicsEngineMain::onShapeHit(const PxControllerShapeHit& hit)
{
	PhysicsEngine::defaultCCTInteraction(hit);
}

//CCT - CCT �浹
void PhysicsEngineMain::onControllerHit(const PxControllersHit& hit)
{
	PxRigidDynamic* actor = hit.controller->getActor()->is<PxRigidDynamic>();
	PxRigidDynamic* otherActor = hit.other->getActor()->is<PxRigidDynamic>();

	if (actor)
	{
		ISimulationEventCallback* callback1 = reinterpret_cast<ISimulationEventCallback*>(actor->userData);
		ISimulationEventCallback* callback2 = reinterpret_cast<ISimulationEventCallback*>(otherActor->userData);

		//isActive üũ
		if (callback1->m_IsActive && callback2->m_IsActive)
		{
			PxExtendedVec3 worldPos = hit.worldPos;

			//���� ���������� ��� �����ϰ� �浹 ���� ����
			PhysicsEngine::CharaterControllerHit cctHit1;
			cctHit1.controller = callback1; //�浹�� �ڽ��� object
			cctHit1.otherController = callback2; //�浹�� ����� object
			cctHit1.worldPos = Vector3(hit.worldPos.x, hit.worldPos.y, hit.worldPos.z);
			cctHit1.worldNormal = ToVector3(hit.worldNormal);
			cctHit1.dir = ToVector3(hit.dir);
			cctHit1.length = hit.length;

			//callback2 ȣ�⿡�� ����� ���� ����
			PhysicsEngine::CharaterControllerHit cctHit2 = cctHit1;
			//�ڽŰ� ��� ������Ʈ ������ swap�ؼ� ���
			cctHit2.controller = callback2; //�浹�� ����� object
			cctHit2.otherController = callback1; //�浹�� �ڽ��� object

			//pair ������ ȣ��
			callback1->OnControllerHit(cctHit1);
			callback2->OnControllerHit(cctHit2);
		}
	}
}

// Implements PxControllerBehaviorCallback
PxControllerBehaviorFlags PhysicsEngineMain::getBehaviorFlags(const PxShape& shape, const PxActor& actor)
{
	return PxControllerBehaviorFlags(0);
}

PxControllerBehaviorFlags PhysicsEngineMain::getBehaviorFlags(const PxController& controller)
{
	return PxControllerBehaviorFlags(0);
}

PxControllerBehaviorFlags PhysicsEngineMain::getBehaviorFlags(const PxObstacle& obstacle)
{
	return PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT | PxControllerBehaviorFlag::eCCT_SLIDE;
}

// Implements PxQueryFilterCallback

PxQueryHitType::Enum PhysicsEngineMain::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags)
{

	//�΋H�� ����� TRIGGER_SHAPE�� ���
	/*if (shape->getFlags() & PxShapeFlag::eTRIGGER_SHAPE)
		return PxQueryHitType::eNONE;*/

	//�浹 ���� �˻�, raycast(), overlap()���� ���
	PxFilterData ohterFilterData = shape->getQueryFilterData();

	if ((filterData.word0 != 0 || ohterFilterData.word0 != 0) &&
		!((filterData.word0 & ohterFilterData.word1) ||
			(filterData.word1 & ohterFilterData.word0))
		)
		return PxQueryHitType::eNONE;

	return PxQueryHitType::eBLOCK;
}

PxQueryHitType::Enum PhysicsEngineMain::postFilter(const PxFilterData& filterData, const PxQueryHit& hit)
{
	//���� ��Ʈ actor, shape�� ���� �ٸ��� ó��, �⺻ blocking
	return PxQueryHitType::eBLOCK;
}

// Implements PxControllerFilterCallback
//false : ���, true : ���
bool PhysicsEngineMain::filter(const PxController& a, const PxController& b)
{
	PxRigidDynamic* actorA = a.getActor();
	PxRigidDynamic* actorB = b.getActor();

	if(!actorA->getScene() || !actorB->getScene())
		return false;

	PxShape* shapeA = NULL;
	PxShape* shapeB = NULL;

	//PxController�� capsule 1���� ������ �����Ƿ� �ϳ��� �ҷ��´�
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

	//SCENE_QUERY_SHAPE�� ������ ó������ ����(���)
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

void PhysicsEngineMain::ComputeInteractionsCCT(float elapsedTime)
{
	m_pControllerManager->computeInteractions(elapsedTime, this);
}

PxControllerCollisionFlags PhysicsEngineMain::MoveCCT(PxController* pPxController, PxVec3 displacement, float dTime)
{
	PxRigidActor* actor = pPxController->getActor();

	PxFilterData filterData;

	if (actor->getNbShapes())
	{
		PxShape* shape;
		actor->getShapes(&shape, 1);

		filterData = shape->getQueryFilterData();
	}

	const PxControllerFilters filter(&filterData, this, this);

	PxControllerCollisionFlags collisionFlags =
		pPxController->move(displacement, 0.001f, dTime, filter);


	return collisionFlags;
}


///////////Rigid Actor Functions
PxRigidActor* PhysicsEngineMain::CreateBox(const PxTransform& transform, PxVec3 halfExtentXYZ,
	const PxTransform& shapeOffset, PxReal density,
	bool isStatic, bool isKinematic)
{
	//PxSceneWriteLock scopedLock(*mScene); //�ʿ��ϸ� ��� #����

	PxRigidActor* pBox;

	if (isStatic)
	{
		pBox = PxCreateStatic(*m_pPhysics, transform,
			PxBoxGeometry(halfExtentXYZ.x, halfExtentXYZ.y, halfExtentXYZ.z),
			*m_pPhysicsMaterial, shapeOffset);
	}
	else
	{
		if (!isKinematic)
		{
			pBox = PxCreateDynamic(*m_pPhysics, transform,
				PxBoxGeometry(halfExtentXYZ.x, halfExtentXYZ.y, halfExtentXYZ.z),
				*m_pPhysicsMaterial, density, shapeOffset);
		}
		else
		{
			pBox = PxCreateKinematic(*m_pPhysics, transform,
				PxBoxGeometry(halfExtentXYZ.x, halfExtentXYZ.y, halfExtentXYZ.z),
				*m_pPhysicsMaterial, density, shapeOffset);
		}
	}
	
	m_pScene->addActor(*pBox);
	return pBox;
}

PxRigidActor* PhysicsEngineMain::CreateSphere(const PxTransform& transform, PxReal radius,
	const PxTransform& shapeOffset, PxReal density,
	bool isStatic, bool isKinematic)
{
	//PxSceneWriteLock scopedLock(*mScene); //�ʿ��ϸ� ��� #����

	PxRigidActor* pSphere;

	if (isStatic)
	{
		pSphere = PxCreateStatic(*m_pPhysics, transform, PxSphereGeometry(radius),
			*m_pPhysicsMaterial, shapeOffset);
	}
	else
	{
		if (!isKinematic)
		{
			pSphere = PxCreateDynamic(*m_pPhysics, transform, PxSphereGeometry(radius),
				*m_pPhysicsMaterial, density, shapeOffset);
		}
		else
		{
			pSphere = PxCreateKinematic(*m_pPhysics, transform, PxSphereGeometry(radius),
				*m_pPhysicsMaterial, density, shapeOffset);
		}
	}

	
	m_pScene->addActor(*pSphere);
	return pSphere;
}

PxRigidActor* PhysicsEngineMain::CreateCapsule(const PxTransform& transform,
	PxReal radius, PxReal halfHeight,
	const PxTransform& shapeOffset, PxReal density,
	bool isStatic, bool isKinematic)
{
	//PxSceneWriteLock scopedLock(*mScene); //�ʿ��ϸ� ��� #����

	PxRigidActor* pCapsule;

	//ĸ�� ���� ���� �����, ������ ���������� #Ȯ��
	//������ ���°� default�� �ϱ����� ������ shape�� Local Transform ����
	//rotates relative transform around the Z-axis by a quarter-circle
	//https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxguide/Manual/Geometry.html#capsules
	
	shapeOffset.rotate(PxVec3(0.f, 0.f, PxHalfPi));

	if (isStatic)
	{
		pCapsule = PxCreateStatic(*m_pPhysics, transform, PxCapsuleGeometry(radius, halfHeight),
			*m_pPhysicsMaterial, shapeOffset);
	}
	else
	{
		if (!isKinematic)
		{
			pCapsule = PxCreateDynamic(*m_pPhysics, transform, PxCapsuleGeometry(radius, halfHeight),
				*m_pPhysicsMaterial, density, shapeOffset);
		}
		else
		{
			pCapsule = PxCreateKinematic(*m_pPhysics, transform, PxCapsuleGeometry(radius, halfHeight),
				*m_pPhysicsMaterial, density, shapeOffset);
		}
	}

	m_pScene->addActor(*pCapsule);
	return pCapsule;
}

//���� : RigidStatic�� ���� ����
//"Shapes with a PxPlaneGeometry may only be created for static actors."
//https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxguide/Manual/Geometry.html#planes
PxRigidActor* PhysicsEngineMain::CreatePlane(PxPlane plane)
{
	//PxSceneWriteLock scopedLock(*mScene); //�ʿ��ϸ� ��� #����

	PxRigidActor* pPlane = PxCreatePlane(*m_pPhysics, plane,
		*m_pPhysicsMaterial);
	m_pScene->addActor(*pPlane);

	return pPlane;
}

//Cooking TriangleMeshData to TriAngleMesh
PxTriangleMeshGeometry* PhysicsEngineMain::CreateTriAngleMeshGeometry(const TriangleMeshData* data, PxVec3 scale)
{
	m_pCooking->setParams(m_CockingParamsDefault);

	//Mesh Cooking
	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = data->vertNum;
	meshDesc.triangles.count = data->faceNum;
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.triangles.stride = sizeof(PxU32) * 3;
	meshDesc.points.data = data->verts;
	meshDesc.triangles.data = data->faces;

	PxDefaultMemoryOutputStream writeBuffer;
	PxTriangleMeshCookingResult::Enum result;

	bool status = m_pCooking->cookTriangleMesh(meshDesc, writeBuffer, &result);
	PX_ASSERT(!status);

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());

	PxTriangleMesh* triangleMesh = m_pPhysics->createTriangleMesh(readBuffer);

	//Scaling Mesh
	PxMeshScale meshScale(scale, PxQuat(PxIdentity));

	PxTriangleMeshGeometry* pTriGeom = new PxTriangleMeshGeometry(triangleMesh, meshScale);
	//pTriGeom->triangleMesh = triangleMesh;
	
	return pTriGeom;
}

//PhysXSample::newMesh�� �ִ� �ڵ� �Ϻ� ���, RigidStatic�� ���� ����
PxRigidActor* PhysicsEngineMain::CreateTriangleMesh(const PxTransform& transform, PxTriangleMeshGeometry* pTriGeom)
{
	PxRigidStatic* actor = m_pPhysics->createRigidStatic(transform);
	PxShape* shape = PxRigidActorExt::createExclusiveShape(*actor, *pTriGeom, *m_pPhysicsMaterial);
		
	PX_UNUSED(shape);
	m_pScene->addActor(*actor);

	return actor;
}

//PhysXSample::newMesh�� �ִ� �ڵ� �Ϻ� ���, RigidStatic�� ���� ����
PxRigidActor* PhysicsEngineMain::CreateTriangleMeshDirect(const PxTransform& transform, const TriangleMeshData& data)
{
	//��� Cooking mode, ���� cooking ���� �浹 ó��
	m_CockingParamsDirect.midphaseDesc = PxMeshMidPhase::Enum::eBVH34;
	m_pCooking->setParams(m_CockingParamsDirect);

	//Mesh Cooking
	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = data.vertNum;
	meshDesc.triangles.count = data.faceNum;
	meshDesc.points.stride = sizeof(PxVec3) * 3;
	meshDesc.triangles.stride = sizeof(PxU32) * 3;
	meshDesc.points.data = data.verts;
	meshDesc.triangles.data = data.faces;

#ifdef _DEBUG
	// mesh should be validated before cooked without the mesh cleaning
	bool res = m_pCooking->validateTriangleMesh(meshDesc);
	PX_ASSERT(res);
#endif

	PxTriangleMesh* triangleMesh = m_pCooking->createTriangleMesh(meshDesc,
		m_pPhysics->getPhysicsInsertionCallback());

	if (triangleMesh)
	{
		PxTriangleMeshGeometry triGeom;
		triGeom.triangleMesh = triangleMesh;
		PxRigidStatic* actor = m_pPhysics->createRigidStatic(transform);
		PxShape* shape = PxRigidActorExt::createExclusiveShape(*actor, triGeom, *m_pPhysicsMaterial);
		PX_UNUSED(shape);

		m_pScene->addActor(*actor);

		return actor;
	}

	return nullptr;
}

PxController* PhysicsEngineMain::CreateCharacterContollerBox(const ControlledActorDesc& desc,
	PxVec3 halfextentXYZ, float& ownerCrouchHeightY)
{
	PxControllerDesc* cDesc;
	PxBoxControllerDesc boxDesc;

	//ĸ�� ���� ��� ���� �ڽ� ��翡 �°� ��ȯ
	boxDesc.halfHeight = halfextentXYZ.y;
	boxDesc.halfSideExtent = halfextentXYZ.x;
	boxDesc.halfForwardExtent = halfextentXYZ.z;
	cDesc = &boxDesc;

	cDesc->density = desc.proxyDensity;
	cDesc->scaleCoeff = desc.proxyScale;
	cDesc->material = m_pPhysicsMaterial; //�⺻ ���׸��� ��� #����
	cDesc->position = desc.position;
	cDesc->slopeLimit = desc.slopeLimit;
	cDesc->contactOffset = desc.contactOffset;
	cDesc->stepOffset = desc.stepOffset;
	cDesc->invisibleWallHeight = desc.invisibleWallHeight;
	cDesc->maxJumpHeight = desc.maxJumpHeight;
	//	cDesc->nonWalkableMode		= PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	cDesc->reportCallback = this;
	cDesc->behaviorCallback = this;
	cDesc->volumeGrowth = desc.volumeGrowth;

	//Controller ����
	PxController* ctrl = m_pControllerManager->createController(*cDesc);
	PX_ASSERT(ctrl);

	// remove controller shape from scene query for standup overlap test
	PxRigidDynamic* actor = ctrl->getActor();
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
	
	if (actor)
	{
		if (actor->getNbShapes())
		{
			PxShape* ctrlShape;
			actor->getShapes(&ctrlShape, 1);
			ctrlShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		}
	}

	return ctrl;
}

PxController* PhysicsEngineMain::CreateCharacterContollerCapsule(const ControlledActorDesc& desc,
	float& ownerHeight, float& ownerRadius, float& ownerCrouchHeight)
{
	const float radius = ownerRadius;
	float height = ownerHeight;
	float crouchHeight = ownerCrouchHeight;

	PxControllerDesc* cDesc;
	PxCapsuleControllerDesc capsuleDesc;

	//ĸ�� ��� ����
	capsuleDesc.height = height;
	capsuleDesc.radius = radius;
	capsuleDesc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED; //StepOffset ���� ���� ��ֹ� ���� �ö� �� �� ����
	cDesc = &capsuleDesc;

	cDesc->density = desc.proxyDensity;
	cDesc->scaleCoeff = desc.proxyScale;
	cDesc->material = m_pPhysicsMaterial; //�⺻ ���׸��� ��� #����
	cDesc->position = desc.position;
	cDesc->slopeLimit = desc.slopeLimit;
	cDesc->contactOffset = desc.contactOffset;
	cDesc->stepOffset = desc.stepOffset;
	cDesc->invisibleWallHeight = desc.invisibleWallHeight;
	cDesc->maxJumpHeight = desc.maxJumpHeight;
	//	cDesc->nonWalkableMode		= PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	cDesc->reportCallback = this;
	cDesc->behaviorCallback = this;
	cDesc->volumeGrowth = desc.volumeGrowth;

	//�� if������ ��ȯ �� ����� �°� ��� ���� ���� �ִ´�.
	ownerHeight = height;
	ownerCrouchHeight = crouchHeight;
	ownerRadius = radius;

	//Controller ����
	PxController* ctrl = m_pControllerManager->createController(*cDesc);
	PX_ASSERT(ctrl);

	// remove controller shape from scene query for standup overlap test
	PxRigidDynamic* actor = ctrl->getActor();
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);

	if (actor)
	{
		if (actor->getNbShapes())
		{
			PxShape* ctrlShape;
			actor->getShapes(&ctrlShape, 1);
			ctrlShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
		}
	}

	return ctrl;
}



////////////////helper functions

PhysicalObjectID PhysicsEngineMain::IdGenerator(physx::PxBase* pObj)
{
	physx::PxBase* temp = pObj;

	return reinterpret_cast<uint64_t>(temp);
}

void PhysicsEngineMain::SetActive(PxRigidStatic* actor, bool isActive)
{
	if (isActive)
	{
		m_pScene->addActor(*actor);
	}
	else
	{
		m_pScene->removeActor(*actor);
	}
}

void PhysicsEngineMain::SetActive(PxRigidActor* actor, bool isActive)
{
	if (isActive)
	{
		m_pScene->addActor(*actor);
	}
	else
	{
		m_pScene->removeActor(*actor);
	}
}

void PhysicsEngineMain::SetActive(PxController* controller, bool isActive)
{
	if (isActive)
	{
		m_pScene->addActor(*controller->getActor());
	}
	else
	{
		m_pScene->removeActor(*controller->getActor());
	}
}

void PhysicsEngineMain::SetTriggerShape(PxRigidActor* actor, bool isTrigger)
{
	if (actor)
	{
		PxShape* ctrlShape;
		int shapesNum = actor->getNbShapes();

		
		for(int i = 1; i <= shapesNum; i++)
		{
			actor->getShapes(&ctrlShape, 1);
			//eSIMULATION_SHAPE�� eTRIGGER_SHAPE�� �Ѵ� set�� �Ǹ� �ȵ�
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

void PhysicsEngineMain::SetSizeRigidActorBox(PxRigidActor* actor, PxVec3 halfExtents)
{
	//���� size ���� ó��
	PX_ASSERT(halfExtents.x > 0.f | halfExtents.y > 0.f | halfExtents.z > 0.f);

	if (actor->getNbShapes())
	{
		PxShape* shape;
		actor->getShapes(&shape, 1);
		PxBoxGeometry box;
		shape->getBoxGeometry(box);

		box.halfExtents = halfExtents;

		shape->setGeometry(box);
	}
}

void PhysicsEngineMain::SetSizeRigidActorSphere(PxRigidActor* actor, PxReal radius)
{
	//���� size ���� ó��
	PX_ASSERT(radius > 0.f);

	if (actor->getNbShapes())
	{
		PxShape* shape;
		actor->getShapes(&shape, 1);
		PxSphereGeometry sphere;
		shape->getSphereGeometry(sphere);

		sphere.radius = radius;

		shape->setGeometry(sphere);
	}
}

void PhysicsEngineMain::SetSizeRigidActorCapsule(PxRigidActor* actor, PxReal radius, PxReal halfHeight)
{
	//���� size ���� ó��
	PX_ASSERT(radius > 0.f | halfHeight > 0.f);

	if (actor->getNbShapes())
	{
		PxShape* shape;
		actor->getShapes(&shape, 1);
		PxCapsuleGeometry capsule;
		shape->getCapsuleGeometry(capsule);

		capsule.radius = radius;
		capsule.halfHeight = halfHeight;

		shape->setGeometry(capsule);
	}
}

void PhysicsEngineMain::SetScaleRigidActorTriangleMesh(PxRigidActor* actor, PxVec3 scale)
{
	//���� scale ���� ó��
	PX_ASSERT(scale.x > 0.f | scale.y > 0.f | scale.z > 0.f);

	if (actor->getNbShapes())
	{
		PxShape* shape;
		actor->getShapes(&shape, 1);
		PxTriangleMeshGeometry mesh;
		shape->getTriangleMeshGeometry(mesh);

		mesh.scale.scale = scale;
		
		shape->setGeometry(mesh);
	}
}

inline PxU32 PhysicsEngineMain::GetShpaeFlags(const PxRigidActor* actor)
{
	if (actor->getNbShapes())
	{
		PxShape* shape;
		actor->getShapes(&shape, 1);

		return shape->getFlags();
	}
	return PxShapeFlags(0);
}

inline PxU32 PhysicsEngineMain::GetShpaeFlags(const PxController* cct)
{
	PxRigidActor* actor = cct->getActor();

	return GetShpaeFlags(actor);
}

void PhysicsEngineMain::RemoveActor(PxRigidActor* pActor)
{
// 	if (pActor->isReleasable())
// 	{
// 
// 	}
}

bool PhysicsEngineMain::RaycastSingle(PxVec3 origin, PxVec3 unitDir, PxReal distance,
	PxSceneQueryFlags outputFlags,
	PxRaycastHit& hit, const PxQueryFilterData& filterData, const PxSceneQueryCache* cache)
{
	return PxSceneQueryExt::raycastSingle(*m_pScene, origin, unitDir,
		distance, outputFlags, hit, filterData);
}

PxI32 PhysicsEngineMain::OverlapMultiple(const PxGeometry& geo, PxTransform tm,
	PxOverlapHit* hitBuffer, PxU32 	hitBufferSize,
	const PxQueryFilterData& filterData)
{
	return PxSceneQueryExt::overlapMultiple(*m_pScene, geo,
		tm, hitBuffer, hitBufferSize, filterData);
}

void PhysicsEngineMain::DeleteTriggerPair(PxRigidActor* actor)
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
