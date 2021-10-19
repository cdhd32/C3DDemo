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

	//�ӽ� �е�(density) ���
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
		//��ȣ�ۿ� Ŭ����
		friend class PhysicsEngineMain;
		friend class CharacterController;

	protected:

		PhysicsEngineMain& m_PhysicsEngine;

		//Shape, ���߿� ���� ���� �ٲ� ���� ����
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

		//�浹 �ݹ� Ȱ��ȭ ����
		bool m_IsCollionCBActive = false;

		bool m_IsSleep = false;

		//Collision Callback Object, ���� ������Ʈ ������
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

		//rigidbody ���� �ùķ��̼� ����
		void Initialize();

		//���� �ùķ��̼� ���� ����Ǵ� Transform ���� ������Ʈ
		void UpdateTransformFixed(Vector3 worldPos, Quaternion worldRot, float dtime);
		//���� �ùķ��̼� ���� Transform �������� �Լ�
		void UpdateAfterSimulate(Vector3& worldPos, Quaternion& worldRot);

		//RigidBody ���� �÷��� ����/Ȯ��
		void SetTrigger(bool isTrigger);
		void SetQueryActive(bool isActive);
		bool IsTrigger() { return m_IsTrigger; }
		bool IsQueryActive() { return m_IsQueryActive; }

		void SetActive(bool isActive);

		//RigidBody �浹 �ݹ�(CB) �Լ� Ȱ��ȭ/��Ȱ��ȭ ���� ����/Ȯ��
		void SetCollionCBActive(bool isCollionCBActive);
		bool IsCollionCBActive() { return m_IsCollionCBActive; }
		
		void SetSleep(bool isSleep);
		bool IsSleep() { return m_IsSleep; }
		//�浹 ���� set
		void SetupCollisionFilter(unsigned int filterGroup, unsigned int filterMask);

		//������ ����
		void SetSize(Vector3 size) { m_pShape->SetSize(size); }
		void SetSize(float radius) { m_pShape->SetSize(radius); }
		void SetSize(float radius, float bodyHeight) { m_pShape->SetSize(radius, bodyHeight); }
		void SetScale(Vector3 scale) { m_pShape->SetScale(scale); }

		//Rigidbody ����/�ʱ�ȭ
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

		//�ܼ� getter, setter
		Vector3 GetWorldPos() const { return m_WorldPos; }
		Quaternion GetWorldRot() const { return m_WorldRot; }

		//����� ���� �Ǿ��ִ� transform ������ get, ���� �ùķ��̼� �ݿ�(X)
		void GetTransform(Vector3& pos, Quaternion& rot)
		{
			pos = m_WorldPos;
			rot = m_WorldRot;
		}

		RigidbodyType GetType() const { return m_Type; }

		
		
	};
}


