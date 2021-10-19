#pragma once

#include <thread>
//#include <set>
#include <limits>
#include <unordered_set>
#include <functional>

namespace PhysicsEngine
{
	using namespace physx;

	typedef PxU64 PhysicalObjectID;
	typedef PxSerialObjectId PhysicalObjectID;
	struct TriangleMeshData;

	//ControlledActor �ʱ�ȭ ����ü
	struct ControlledActorDesc
	{
		ControlledActorDesc()
			:
			type(PxControllerShapeType::eFORCE_DWORD),
			position(PxExtendedVec3(0, 0, 0)),
			slopeLimit(0.0f),
			contactOffset(0.0f),
			stepOffset(0.0f),
			invisibleWallHeight(0.0f),
			maxJumpHeight(0.0f),
			radius(0.0f),
			height(0.0f),
			crouchHeight(0.0f),
			proxyDensity(10.0f),
			proxyScale(0.9f),
			volumeGrowth(1.5f),
			pReportCallback(NULL),
			pBehaviorCallback(NULL)
		{}

		PxControllerShapeType::Enum		type;
		PxExtendedVec3					position;
		float							slopeLimit;
		float							contactOffset;
		float							stepOffset;
		float							invisibleWallHeight;
		float							maxJumpHeight;
		float							radius;
		float							height;
		float							crouchHeight;
		float							proxyDensity;
		float							proxyScale;
		float							volumeGrowth;
		PxUserControllerHitReport* pReportCallback;
		PxControllerBehaviorCallback* pBehaviorCallback;
	};

	struct PrePassFlag
	{
		enum Enum
		{
			eDEACTIVE = 0,
			ePASS = eDEACTIVE,
			eACTIVE = (1 << 0),
			eNOTPASS = eACTIVE
		};
	};

	//onTriggerStay helper functions
	struct CompTriggerPair
	{
		bool operator () (const PxTriggerPair& lhs, const PxTriggerPair& rhs) const
		{
			if (lhs.triggerActor < rhs.triggerActor)
				return true;
			else
				return false;
		}
	};

	struct EQTriggerPair
	{
		bool operator () (const PxTriggerPair& lhs, const PxTriggerPair& rhs) const
		{
			if (lhs.triggerActor == rhs.triggerActor &&
				lhs.otherActor == rhs.otherActor
				)
				return true;
			else
				return false;
		}
	};

	struct HashTriggerPair
	{
		size_t operator () (const PxTriggerPair& pair) const
		{
			uint64_t hashWord1 = reinterpret_cast<uint64_t>(pair.triggerActor);
			uint64_t hashWord2 = reinterpret_cast<uint64_t>(pair.otherActor);

			return hashWord1 ^ ((hashWord2 + 3090 + 'T' + 'i') << 4);
		}
	};

	

	/*struct AllocTriggerPair
	{
		PxTriggerPair* operator () (const PxTriggerPair& pair) const
		{
			unsigned int hashWord1 = reinterpret_cast<unsigned int>(pair.triggerActor);
			unsigned int hashWord2 = reinterpret_cast<PxU32>(pair.otherActor);

			return hashWord1 ^ ((hashWord2 + 3090 + 'T' + 'i') << 4);
		}
	};*/

	//PhysicsEngine ���� ���� + ��ü ����
	class PhysicsEngineMain : virtual public PxSimulationEventCallback,
		virtual public PxUserControllerHitReport,
		virtual public PxControllerBehaviorCallback,
		virtual public PxQueryFilterCallback,
		virtual public PxControllerFilterCallback
	{
	public:
		//CCT - �Ϲ� shape query �浹 ������ �߰��� FilterData �̰� ������ �浹 ó���� �ȵȴ�.
		//static const PxU32 DEFAULT_QUERY_FILTERDATA = 0x10000000;

	private:
		static PxDefaultAllocator m_DefaultAllocator;
		static PxDefaultErrorCallback m_DefaultErrorCallback;

		static PxFoundation* m_pFoundation;
		static PxPvd* m_pPVD;
		static PxPvdTransport* m_pPVDTransport;

		static PxPhysics* m_pPhysics;
		static PxCooking* m_pCooking;

		PxCookingParams m_CockingParamsDefault = PxCookingParams(PxTolerancesScale());
		PxCookingParams m_CockingParamsDirect = PxCookingParams(PxTolerancesScale());

		static PxDefaultCpuDispatcher* m_pDispatcher;

		//GPU ����(CUDA) ���
		//static physx::PxCudaContextManager* m_pCudaManager;

		//���� �ùķ��̼ǿ� ����ϴ� ��Ŀ������ ���� -> CPU ������ / 2 ��ŭ�� ���
		unsigned int m_WorkerThreadNum = std::thread::hardware_concurrency() / 2;
	
	private:
		static PhysicsEngineMain* m_pPhysicsEngine;

		//�ӽ� ��ü �����̳�
		PxCollection* m_pCollection;

		PxControllerManager* m_pControllerManager;

		//�ӽ� Scene -> Scene Ŭ�������� ���� ���� #����
		PxScene* m_pScene;
		std::unordered_set <PxScene*, HashTriggerPair, EQTriggerPair> m_pSceneList;

		//�ӽ� Material -> PhysicsMaterial Ŭ������ ���ο��� #����
		PxMaterial* m_pPhysicsMaterial;
		PxMaterial* m_pPhysicsMaterialStatic;

		//OnTriggerStay ������ ���� Trigger Pair ��ü �����
		//PxPairFlag::eNOTIFY_TOUCH_FOUND �̺�Ʈ�� �߻��ϸ� �ش� ��ü�� insert,
		//PxPairFlag::eNOTIFY_TOUCH_LOST �̺�Ʈ�� �߻��ϸ� �ش� ��ü�� erase��
		std::unordered_set<PxTriggerPair, HashTriggerPair, EQTriggerPair> m_pTriggerPairSet;

	public:
		//���� �̱���
		static PhysicsEngineMain& GetInstance()
		{
			if (!m_pPhysicsEngine)
			{
				m_pPhysicsEngine = new PhysicsEngineMain();
			}

			return *m_pPhysicsEngine;
		}

		void Initialize();
		//ĳ���� ��Ʈ�ѷ�(CCT) �浹 �ݹ� ������ ���� �� ����(?)�ִ� �Լ�,
		//���� ���� ĳ���� ��Ʈ�ѷ��� ����� �� FixedUpdate���� UpdatePositionCCT ȣ�� �� ���۽� �� ������ ���� �ѹ� ȣ���� �ְ� ����ϸ� ����
		void ComputeInteractionsCCT(float elapsedTime);
		void Update();
		void Finalize();
	
		// Implements PxUserControllerHitReport
		void onShapeHit(const PxControllerShapeHit& hit) override;
		void onControllerHit(const PxControllersHit& hit) override;
		void onObstacleHit(const PxControllerObstacleHit& hit) {};

		// Implements PxControllerBehaviorCallback
		PxControllerBehaviorFlags getBehaviorFlags(const PxShape& shape, const PxActor& actor) override;
		PxControllerBehaviorFlags getBehaviorFlags(const PxController& controller) override;
		PxControllerBehaviorFlags getBehaviorFlags(const PxObstacle& obstacle) override;

		// Implements PxQueryFilterCallback
		PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) override;
		PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit) override;


		bool filter(const PxController& a, const PxController& b) override;

	public:

	// Implements PxSimulationEventCallback
		void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
		void onTrigger(PxTriggerPair* pairs, PxU32 count);
		void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {};
		void onWake(PxActor** actors, PxU32 count) {};
		void onSleep(PxActor** actors, PxU32 count) {};
		void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) {};
	public:
		//Rigid Actor Functions
		//Box�� �����Ͽ� Scene�� ���
		PxRigidActor* CreateBox(const PxTransform& transform, PxVec3 halfExtentXYZ, const PxTransform& shapeOffset, PxReal density, bool isKinematic = false, bool isStatic = false);
		
		//Sphere
		PxRigidActor* CreateSphere(const PxTransform& transform, PxReal radius, const PxTransform& shapeOffset, PxReal density, bool isStatic, bool isKinematic);
		
		//Capsule
		PxRigidActor* CreateCapsule(const PxTransform& transform, PxReal radius, PxReal halfHeight, const PxTransform& shapeOffset, PxReal density, bool isStatic, bool isKinematic);
		
		//������ �ٴ� ����, �յ� ���� ����
		PxRigidActor* CreatePlane(PxPlane plane);

		
		//TriangleMesh RigidActor�� �����ϱ� ���� Cooking�ϴ� �Լ�
		//memory stream�� ���� ���� ����, ���߿� ���� ����&�ҷ����⵵ �����ϰ� ���� ���� #�߰�
		//+ Scaling Mesh
		//https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxguide/Manual/Geometry.html#mesh-scaling
		PxTriangleMeshGeometry* CreateTriAngleMeshGeometry(const TriangleMeshData* data, PxVec3 scale);
		//Cooking�� PxTriangleMeshGeometry�� ����Ͽ� TriangleMesh RigidActor�� ����
		PxRigidActor* CreateTriangleMesh(const PxTransform& transform, PxTriangleMeshGeometry* pTriGeom);
		//memory stream�� ��ġ�� �ʰ� ��� ����(�۾� ��) #����
		PxRigidActor* CreateTriangleMeshDirect(const PxTransform& transform, const TriangleMeshData& data);
		
		
		void SetActive(PxController* controller, bool isActive);
		void SetActive(PxRigidStatic* actor, bool isActive);
		void SetActive(PxRigidActor* actor, bool isActive);
		//PxRigidActor�� Shape ������ ����
		//true �̸� eTRIGGER_SHAPE(Ʈ����, volume ��ħ ���η� �浹ó�� �Լ� ����)
		//false �̸� eSIMULATION_SHAPE(�浹, ǥ�� �浹 ���η� �浹ó�� �Լ� ����)
		void SetTriggerShape(PxRigidActor* actor, bool isTrigger);
		//onTriggerStay������ ����� PxTriggerPair�� actor�� ã�Ƽ� ����
		void DeleteTriggerPair(PxRigidActor* actor);
		void DeleteTriggerPair(PxTriggerPair* pair);

		//CharacterContoller ����
		PxController* CreateCharacterContollerBox(const ControlledActorDesc& desc, PxVec3 halfextentXYZ, float& ownerCrouchHeightY);
		PxController* CreateCharacterContollerCapsule(const ControlledActorDesc& desc, float& ownerHeight, float& ownerRadius, float& ownerCrouchHeight);

		//���� pxShape�� ����� ����
		void SetSizeRigidActorBox(PxRigidActor* actor, PxVec3 halfExtents);
		void SetSizeRigidActorSphere(PxRigidActor* actor, PxReal radius);
		void SetSizeRigidActorCapsule(PxRigidActor* actor, PxReal radius, PxReal halfHeight);

		void SetScaleRigidActorTriangleMesh(PxRigidActor* actor, PxVec3 scale);

		PxU32 GetShpaeFlags(const PxRigidActor* actor);
		PxU32 GetShpaeFlags(const PxController* cct);
		

		//CCT �̵�
		PxControllerCollisionFlags MoveCCT(PxController* pPxController, PxVec3 velocity, float dTime);

		PhysicalObjectID IdGenerator(PxBase* pObj);

		void AddCollection(PxBase& Obj, PhysicalObjectID id)
		{
			m_pCollection->add(Obj);
			m_pCollection->addId(Obj, id);
		};

		PxRigidActor* FindActorByID(PhysicalObjectID id)
		{
			return m_pCollection->find(id)->is<PxRigidActor>();
		}

		void RemoveActorByID(PhysicalObjectID id)
		{
			m_pCollection->remove(*m_pCollection->find(id));
			m_pCollection->removeId(id);
		}

		unsigned int GetCCTNum()
		{
			return m_pControllerManager->getNbControllers();
		}

		//#�ۼ��� #����
		void RemoveActor(PxRigidActor* pActor);

		bool RaycastSingle(PxVec3 origin, PxVec3 unitDir, PxReal distance, PxSceneQueryFlags outputFlags,
			PxRaycastHit& hit, const PxQueryFilterData& filterData, const PxSceneQueryCache* cache = NULL);
		
		//bool RaycastMultiple();

		//Number of hits in the buffer, or -1 if the buffer overflowed.
		PxI32 OverlapMultiple(const PxGeometry& geo, PxTransform tm, PxOverlapHit* hitBuffer, PxU32 hitBufferSize, const PxQueryFilterData& filterData);
		
	private:
		PhysicsEngineMain() {};
		~PhysicsEngineMain() {};

		//Scene�� ��ü���� ������Ʈ, 60step/sec
		void StepPhysics(bool isUpdate);
		void CleanupPhysics(bool isCleanUp);

		void CustomizeSceneDesc(PxSceneDesc& sceneDesc);

		
	public:
		void SetupFiltering(PxRigidActor* actor, const PxFilterData& filterData); //sim filter word0,1 set
		void SetupQueryFiltering(PxRigidActor* actor, const PxFilterData& filterData); //query filter word0,1 set

	public:
		
};

	void defaultCCTInteraction(const PxControllerShapeHit& hit);
};
