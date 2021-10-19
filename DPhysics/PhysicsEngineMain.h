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

	//ControlledActor 초기화 구조체
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

	//PhysicsEngine 메인 루프 + 강체 생성
	class PhysicsEngineMain : virtual public PxSimulationEventCallback,
		virtual public PxUserControllerHitReport,
		virtual public PxControllerBehaviorCallback,
		virtual public PxQueryFilterCallback,
		virtual public PxControllerFilterCallback
	{
	public:
		//CCT - 일반 shape query 충돌 때문에 추가한 FilterData 이게 없으면 충돌 처리가 안된다.
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

		//GPU 가속(CUDA) 사용
		//static physx::PxCudaContextManager* m_pCudaManager;

		//물리 시뮬레이션에 사용하는 워커스레드 개수 -> CPU 스레드 / 2 만큼만 사용
		unsigned int m_WorkerThreadNum = std::thread::hardware_concurrency() / 2;
	
	private:
		static PhysicsEngineMain* m_pPhysicsEngine;

		//임시 객체 컨테이너
		PxCollection* m_pCollection;

		PxControllerManager* m_pControllerManager;

		//임시 Scene -> Scene 클래스에서 공유 예정 #수정
		PxScene* m_pScene;
		std::unordered_set <PxScene*, HashTriggerPair, EQTriggerPair> m_pSceneList;

		//임시 Material -> PhysicsMaterial 클래스로 래핑예정 #수정
		PxMaterial* m_pPhysicsMaterial;
		PxMaterial* m_pPhysicsMaterialStatic;

		//OnTriggerStay 구현을 위한 Trigger Pair 객체 저장소
		//PxPairFlag::eNOTIFY_TOUCH_FOUND 이벤트가 발생하면 해당 객체를 insert,
		//PxPairFlag::eNOTIFY_TOUCH_LOST 이벤트가 발생하면 해당 객체를 erase함
		std::unordered_set<PxTriggerPair, HashTriggerPair, EQTriggerPair> m_pTriggerPairSet;

	public:
		//흔한 싱글턴
		static PhysicsEngineMain& GetInstance()
		{
			if (!m_pPhysicsEngine)
			{
				m_pPhysicsEngine = new PhysicsEngineMain();
			}

			return *m_pPhysicsEngine;
		}

		void Initialize();
		//캐릭터 컨트롤러(CCT) 충돌 콜백 시점을 조금 더 땡겨(?)주는 함수,
		//여러 개의 캐릭터 컨트롤러를 사용할 때 FixedUpdate에서 UpdatePositionCCT 호출 전 시작시 매 프레임 마다 한번 호출해 주고 사용하면 좋음
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
		//Box를 생성하여 Scene에 등록
		PxRigidActor* CreateBox(const PxTransform& transform, PxVec3 halfExtentXYZ, const PxTransform& shapeOffset, PxReal density, bool isKinematic = false, bool isStatic = false);
		
		//Sphere
		PxRigidActor* CreateSphere(const PxTransform& transform, PxReal radius, const PxTransform& shapeOffset, PxReal density, bool isStatic, bool isKinematic);
		
		//Capsule
		PxRigidActor* CreateCapsule(const PxTransform& transform, PxReal radius, PxReal halfHeight, const PxTransform& shapeOffset, PxReal density, bool isStatic, bool isKinematic);
		
		//경계없는 바닥 생성, 앞뒤 구분 있음
		PxRigidActor* CreatePlane(PxPlane plane);

		
		//TriangleMesh RigidActor를 생성하기 전에 Cooking하는 함수
		//memory stream을 통한 사전 생성, 나중에 파일 저장&불러오기도 가능하게 만들 예정 #추가
		//+ Scaling Mesh
		//https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxguide/Manual/Geometry.html#mesh-scaling
		PxTriangleMeshGeometry* CreateTriAngleMeshGeometry(const TriangleMeshData* data, PxVec3 scale);
		//Cooking한 PxTriangleMeshGeometry를 사용하여 TriangleMesh RigidActor를 생성
		PxRigidActor* CreateTriangleMesh(const PxTransform& transform, PxTriangleMeshGeometry* pTriGeom);
		//memory stream을 거치지 않고 즉시 생성(작업 중) #수정
		PxRigidActor* CreateTriangleMeshDirect(const PxTransform& transform, const TriangleMeshData& data);
		
		
		void SetActive(PxController* controller, bool isActive);
		void SetActive(PxRigidStatic* actor, bool isActive);
		void SetActive(PxRigidActor* actor, bool isActive);
		//PxRigidActor의 Shape 유형을 변경
		//true 이면 eTRIGGER_SHAPE(트리거, volume 겹침 여부로 충돌처리 함수 실행)
		//false 이면 eSIMULATION_SHAPE(충돌, 표면 충돌 여부로 충돌처리 함수 실행)
		void SetTriggerShape(PxRigidActor* actor, bool isTrigger);
		//onTriggerStay용으로 복사된 PxTriggerPair를 actor로 찾아서 삭제
		void DeleteTriggerPair(PxRigidActor* actor);
		void DeleteTriggerPair(PxTriggerPair* pair);

		//CharacterContoller 생성
		PxController* CreateCharacterContollerBox(const ControlledActorDesc& desc, PxVec3 halfextentXYZ, float& ownerCrouchHeightY);
		PxController* CreateCharacterContollerCapsule(const ControlledActorDesc& desc, float& ownerHeight, float& ownerRadius, float& ownerCrouchHeight);

		//내부 pxShape의 사이즈를 변경
		void SetSizeRigidActorBox(PxRigidActor* actor, PxVec3 halfExtents);
		void SetSizeRigidActorSphere(PxRigidActor* actor, PxReal radius);
		void SetSizeRigidActorCapsule(PxRigidActor* actor, PxReal radius, PxReal halfHeight);

		void SetScaleRigidActorTriangleMesh(PxRigidActor* actor, PxVec3 scale);

		PxU32 GetShpaeFlags(const PxRigidActor* actor);
		PxU32 GetShpaeFlags(const PxController* cct);
		

		//CCT 이동
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

		//#작성중 #수정
		void RemoveActor(PxRigidActor* pActor);

		bool RaycastSingle(PxVec3 origin, PxVec3 unitDir, PxReal distance, PxSceneQueryFlags outputFlags,
			PxRaycastHit& hit, const PxQueryFilterData& filterData, const PxSceneQueryCache* cache = NULL);
		
		//bool RaycastMultiple();

		//Number of hits in the buffer, or -1 if the buffer overflowed.
		PxI32 OverlapMultiple(const PxGeometry& geo, PxTransform tm, PxOverlapHit* hitBuffer, PxU32 hitBufferSize, const PxQueryFilterData& filterData);
		
	private:
		PhysicsEngineMain() {};
		~PhysicsEngineMain() {};

		//Scene의 객체들을 업데이트, 60step/sec
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
