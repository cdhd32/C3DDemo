#pragma once

//ĳ���� ��Ʈ�ѷ�
//�ùķ��̼ǿ� ���� ���� �����鼭�� �ٸ� ��ü�� ��ġ�� �ʴ� ��ü
//����� �Է¿� ���� ��ȣ�ۿ��� ������ �������̽��� ����
namespace PhysicsEngine
{
	enum class CCTShapeType : unsigned int
	{
		None = 0,
		Capsule = 1,
		Box = 2
	};

	/*struct CCTCollisionDir
	{
		enum Enum
		{
			eNone = 0,
			eUp = (1 << 0),
			eDown = (1 << 1),
			eSide = (1 << 2)
		};
	};*/
	

	class ISimulationEventCallback;

	class CharacterController
	{

	private:
		PhysicsEngineMain& m_PhysicsEngine;

		PxController* m_pPxController = nullptr;


		Rigidbody* m_pRigidbody = nullptr;

	private:
		//�⺻ ��� ����
		CCTShapeType m_Type = CCTShapeType::None;

		//ĸ�� ��� Ư��
		float m_Radius;
		float m_BodyHeight;

		//�ڽ� ��� Ư��
		Vector3 m_Size;

		//���� ������(���� shape�� ������ ��)
		float m_ProxyScale = 1.0f; //default : 0.9

		//���� �� ����
		float m_CrouchHeight = 0.f;

		//��ġ
		Vector3 m_Position = Vector3(0, 0, 0);

		//�ִ� ����(�߷��� �����ϴ� ����� �ݴ�Ǵ� ���� �̵�) ����
		float m_MaxJumpHeight = 0.f;

		//�е�
		float m_ProxyDensity = DEFAULT_DENSITY;

		//���, ��� ó�� ���� �Ӽ�
		float m_SlopeLimit = 0.f; //slopeLimit = cosf(PxMath::degToRad(45.0f));
		float m_ContactOffset = 0.1f; //Default: 0.1
		float m_StepOffset = 0.f;

		//slopeLimit ���� ���� �ڵ� �����Ǵ� �߰� �ﰢ��(������ �ʴ� ��)�� ����, 0.0�̸� �������� ����
		float m_InvisibleWallHeight = 0.f;

		//���, ��� ó�� ���� ���� ĳ�� �� �ֺ� ���� ������,
		//�ϴ� 1.0 ���� Ŀ���ϰ� ���� ������ ���� 2.0 ���� ���� ���� ����
		//https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxapi/files/classPxControllerDesc.html#a93930e888fcd4c2f7bafeac83621612e
		float m_VolumeGrowth = 1.5f;
	
		//Collision Callback Object, ���� ������Ʈ ������
		ISimulationEventCallback* m_pOwnerCallback = nullptr;

	public:

		//ĸ�� ��� CCT ������
		CharacterController(PhysicsEngineMain& physicsEngine, ISimulationEventCallback* ownerCallback,
			Vector3 initPos, float radius, float bodyHeight) :
			m_PhysicsEngine(physicsEngine), m_Position(initPos),
			m_Radius(radius), m_BodyHeight(bodyHeight), m_Type(CCTShapeType::Capsule),
			m_pOwnerCallback(ownerCallback)
		{}

		//�ڽ� ��� CCT ������
		CharacterController(PhysicsEngineMain& physicsEngine, ISimulationEventCallback* ownerCallback,
			Vector3 initPos, Vector3 size) :
			m_PhysicsEngine(physicsEngine), m_Position(initPos),
			m_Size(size), m_Type(CCTShapeType::Box),
			m_pOwnerCallback(ownerCallback)
		{}

		~CharacterController() = default;

		//������ �ʱ�ȭ
		void Initialize();
		
		//���� �ùķ��̼� ���� ����Ǵ� position ���� ������Ʈ
		CCTCollisionDirFlags UpdatePosition(Vector3 displacement, float dTime);
		
		//���� �ùķ��̼� ���� position ���� �������� �Լ�
		void GetPosition(Vector3& position);
		void GetFootPosition(Vector3& position);

		//Ȱ��ȭ/��Ȱ��ȭ ����
		void SetActive(bool isActive);

		void SetQueryActive(bool isActive);
		//�浹ó�� callback Ȱ��ȭ/��Ȱ��ȭ ����/Ȯ��
		void SetCollionCBActive(bool isCollionCBActive);
		bool IsCollionCBActive() {
			if (m_pRigidbody)
			{
				return m_pRigidbody->m_IsCollionCBActive;
			}
			
			return false;
		}
		
		//���� ����
		void SetupCollisionFilter(unsigned int filterGroup, unsigned int filterMask);

		

		void Release();
		bool IsReleased()
		{
			if (!m_pPxController)
				return true;
			else
				return false;
		}

	private:

	public:
		//CharacterController ����/�ʱ�ȭ
		static CharacterController* CreateCapsule(ISimulationEventCallback* ownerCallback,
			Vector3 initPos, float radius, float height,
			unsigned int filterGroup = 0, unsigned int filterMask = 0);
		static CharacterController* CreateBox(ISimulationEventCallback* ownerCallback,
			Vector3 initPos, Vector3 size,
			unsigned int filterGroup = 0, unsigned int filterMask = 0);
		
		//SetSize �׽�Ʈ�� �Լ�
		void SetSize(float radius, float bodyheight)
		{
			m_pRigidbody->SetSize(radius, bodyheight);
		}
	};
}



