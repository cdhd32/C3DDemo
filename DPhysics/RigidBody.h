#pragma once

struct BOOLVECTOR3
{
	bool x = false;
	bool y = false;
	bool z = false;
};

namespace PhysicsEngine
{
	class Shape;
	class ISimulationEventCallback;

	//임시 밀도(density) 상수
	//static const PxReal DEFAULT_DENSITY = 10.0f;

	enum class RigidbodyType : unsigned int
	{
		Static = 0,
		Dynamic = 1,
		Kinematic = 2,
		Default = Dynamic
	};

	class Rigidbody
	{
		//상호작용 클래스
		friend class PhysicsEngineMain;
		friend class CharacterController;

	protected:

		PhysicsEngineMain& m_PhysicsEngine;

		//Shape, 나중에 여러 개로 바뀔 수도 있음
		Shape* m_pShape = nullptr;

		//pxRigidActor
		PxRigidDynamic* m_pPxRigidDynamic = nullptr;
		PxRigidStatic* m_pPxRigidStatic = nullptr;

		RigidbodyType m_Type;

		//instance information
		Vector3 m_WorldPos;
		Quaternion m_WorldRot;

		//trigger
		bool m_IsTrigger = false;

		//query
		bool m_IsQueryActive = true;

		//overlap
		bool m_IsOverlapped = false;

		//충돌 콜백 활성화 여부
		bool m_IsCollionCBActive = false;

		bool m_IsSleep = false;

		//Collision Callback Object, 유저 오브젝트 포인터
		ISimulationEventCallback* m_pOwnerCallback = nullptr;
	protected:
		/*float m_Mass;
		float m_Drag;
		float m_AngularDrag;
		bool m_UseGravity;
		bool m_IsStatic = false;
		bool m_IsKenematic = false;*/
		//var m_interpolate
		//var m_CollisionDetection

		float m_Density = DEFAULT_DENSITY;

		//Constraints
		BOOLVECTOR3 m_PositionLock;
		BOOLVECTOR3 m_RotationLock;

	protected:
		//Info Internal value
		//float m_Speed; //magnitude of m_Velocity
		//Vector3 m_Velocity;
		//Vector3 m_AngularVelocity;
		//Vector3 InertiaTensorPosition;
		//Vector3 InertiaTensorRotation;
		//Vector3 LocalCenterOfMass;
		//Vector3 WorldCenterOfMass;
		//bool SleepState;

	public:
		Rigidbody(PhysicsEngineMain& physicsEngine,
			ISimulationEventCallback* ownerCallback,
			Shape* pShape,
			RigidbodyType type,
			Vector3 worldPos = Vector3(0, 0 ,0),
			Quaternion worldRot = Quaternion::Identity)
			:
			m_PhysicsEngine(physicsEngine),
			m_pShape(pShape),
			m_Type(type),
			m_WorldPos(worldPos),
			m_WorldRot(worldRot),
			m_pOwnerCallback(ownerCallback)
		{}

		~Rigidbody() = default;

		//rigidbody 엔진 시뮬레이션 세팅
		void Initialize();

		//물리 시뮬레이션 전에 실행되는 Transform 정보 업데이트
		void UpdateTransformFixed(Vector3 worldPos, Quaternion worldRot, float dtime);
		//물리 시뮬레이션 이후 Transform 가져오는 함수
		void UpdateAfterSimulate(Vector3& worldPos, Quaternion& worldRot);

		//RigidBody 상태 플래그 설정/확인
		void SetTrigger(bool isTrigger);
		void SetQueryActive(bool isActive);
		bool IsTrigger() { return m_IsTrigger; }
		bool IsQueryActive() { return m_IsQueryActive; }

		void SetActive(bool isActive);

		//RigidBody 충돌 콜백(CB) 함수 활성화/비활성화 여부 설정/확인
		void SetCollionCBActive(bool isCollionCBActive);
		bool IsCollionCBActive() { return m_IsCollionCBActive; }
		
		void SetSleep(bool isSleep);
		bool IsSleep() { return m_IsSleep; }
		//충돌 필터 set
		void SetupCollisionFilter(unsigned int filterGroup, unsigned int filterMask);

		//사이즈 조절
		void SetSize(Vector3 size) { m_pShape->SetSize(size); }
		void SetSize(float radius) { m_pShape->SetSize(radius); }
		void SetSize(float radius, float bodyHeight) { m_pShape->SetSize(radius, bodyHeight); }
		void SetScale(Vector3 scale) { m_pShape->SetScale(scale); }

		//Rigidbody 생성/초기화
		static Rigidbody* CreateBox(ISimulationEventCallback* ownerCallback,
			RigidbodyType type, Vector3 position, Quaternion rotation, Vector3 size,
			Vector3 centerPos = Vector3::Zero, unsigned int filterGroup = 0, unsigned int filterMask = 0);

		static Rigidbody* CreateCapsule(ISimulationEventCallback* ownerCallback,
			RigidbodyType type, Vector3 position, Quaternion rotation, float radius, float bodyHeight,
			Vector3 centerPos = Vector3::Zero, unsigned int filterGroup = 0, unsigned int filterMask = 0);

		static Rigidbody* CreateSphere(ISimulationEventCallback* ownerCallback,
			RigidbodyType type, Vector3 position, Quaternion rotation, float radius,
			Vector3 centerPos = Vector3::Zero, unsigned int filterGroup = 0, unsigned int filterMask = 0);

		static Rigidbody* CreatePlaneStatic(ISimulationEventCallback* ownerCallback,
			Vector3 normal, float distance);
		static Rigidbody* CreateTriangleMeshStatic(ISimulationEventCallback* ownerCallback,
			const TriangleMeshData& mesh, Vector3 position, Quaternion rotation, Vector3 scale,
			Vector3 centerPos = Vector3::Zero);
		
		static Rigidbody* CreateBoxTrigger(ISimulationEventCallback* ownerCallback,
			RigidbodyType type, Vector3 position, Quaternion rotation, Vector3 size,
			Vector3 centerPos = Vector3::Zero, unsigned int filterGroup = 0, unsigned int filterMask = 0);

		static Rigidbody* CreateCapsuleTrigger(ISimulationEventCallback* ownerCallback,
			RigidbodyType type, Vector3 position, Quaternion rotation, float radius, float bodyHeight,
			Vector3 centerPos = Vector3::Zero, unsigned int filterGroup = 0, unsigned int filterMask = 0);

		static Rigidbody* CreateSphereTrigger(ISimulationEventCallback* ownerCallback,
			RigidbodyType type, Vector3 position, Quaternion rotation, float radius,
			Vector3 centerPos = Vector3::Zero, unsigned int filterGroup = 0, unsigned int filterMask = 0);
		
		void Release();
		bool IsReleased()
		{
			if (!m_pPxRigidDynamic && !m_pPxRigidDynamic)
				return true;
			else
				return false;
		}

		//단순 getter, setter
		Vector3 GetWorldPos() const { return m_WorldPos; }
		Quaternion GetWorldRot() const { return m_WorldRot; }

		//멤버에 저장 되어있는 transform 정보만 get, 물리 시뮬레이션 반영(X)
		void GetTransform(Vector3& pos, Quaternion& rot)
		{
			pos = m_WorldPos;
			rot = m_WorldRot;
		}

		RigidbodyType GetType() const { return m_Type; }

		
		
	};
}


