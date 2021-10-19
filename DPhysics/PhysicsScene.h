#pragma once

namespace PhysicsEngine
{
	//수많은 Callback & Filter 인터페이스...
	class PhysicsScene : virtual public PxSimulationEventCallback,
		virtual public PxUserControllerHitReport,
		virtual public PxControllerBehaviorCallback,
		virtual public PxQueryFilterCallback,
		virtual public PxControllerFilterCallback
	{
		friend class PhysicsEngineMain;
		friend class Rigidbody;
		friend class CharacterController;
	private:
		//전역 참조 포인터
		PxPhysics* m_pPhysics = nullptr;
		PxFoundation* m_pFoundation = nullptr;
		PxCooking* m_pCooking = nullptr;
		PxDefaultCpuDispatcher* m_pDispatcher = nullptr;
		PxPvd* m_pPVD = nullptr;

	private:
		//Scene 별로 소유하는 객체
		PxScene* m_pScene = nullptr;

		//CCT 전용 매니저
		PxControllerManager* m_pControllerManager = nullptr;

		//임시 Material -> PhysicsMaterial 클래스로 래핑예정 #수정
		PxMaterial* m_pPhysicsMaterial;
		PxMaterial* m_pPhysicsMaterialStatic;

		//OnTriggerStay 구현을 위한 Trigger Pair 객체 저장소
		//PxPairFlag::eNOTIFY_TOUCH_FOUND 이벤트가 발생하면 해당 객체를 insert,
		//PxPairFlag::eNOTIFY_TOUCH_LOST 이벤트가 발생하면 해당 객체를 erase함
		std::unordered_set<PxTriggerPair, HashTriggerPair, EQTriggerPair> m_pTriggerPairSet;

	private:
		//time
		float m_AccumTime = 0.0f;
		const float DEFAULT_STEPSIZE = 1.0f / 60.0f; //16.6666ms


	public:
		PhysicsScene()
			//:
		{};
		~PhysicsScene() = default;

		
		//최소 필요자원 할당 및 초기화
		void Initialize();
		void Update(float dTime);

		//Scene 에서 생성한 자원 해제
		void Finalize();

	public:

		void SetTriggerShape(PxRigidActor* actor, bool isTrigger);

		bool RaycastSingle(PxVec3 origin, PxVec3 unitDir, PxReal distance, PxSceneQueryFlags outputFlags, PxRaycastHit& hit, const PxQueryFilterData& filterData, const PxSceneQueryCache* cache);
		PxI32 OverlapMultiple(const PxGeometry& geo, PxTransform tm, PxOverlapHit* hitBuffer, PxU32 hitBufferSize, const PxQueryFilterData& filterData);
	
	public:
		// Implements PxSimulationEventCallback
		void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
		void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
		void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {};
		void onWake(PxActor** actors, PxU32 count) {};
		void onSleep(PxActor** actors, PxU32 count) {};
		void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) {};
	
		//actor로 m_pTriggerPairSet에 있는 PxTriggerPair를 찾아 즉시 삭제
		void DeleteTriggerPair(PxRigidActor* actor);

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
		
		// Implements PxControllerFilterCallback
		bool filter(const PxController& a, const PxController& b) override;
	};
}


