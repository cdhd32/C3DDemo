#pragma once

//캐릭터 컨트롤러
//시뮬레이션에 영향 받지 않으면서도 다른 강체와 겹치지 않는 강체
//사용자 입력에 대한 상호작용이 가능한 인터페이스를 제공
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
		//기본 모양 정의
		CCTShapeType m_Type = CCTShapeType::None;

		//캡슐 모양 특성
		float m_Radius;
		float m_BodyHeight;

		//박스 모양 특성
		Vector3 m_Size;

		//유도 스케일(원래 shape와 비교했을 때)
		float m_ProxyScale = 1.0f; //default : 0.9

		//앉을 때 높이
		float m_CrouchHeight = 0.f;

		//위치
		Vector3 m_Position = Vector3(0, 0, 0);

		//최대 점프(중력이 적용하는 방향과 반대되는 방향 이동) 높이
		float m_MaxJumpHeight = 0.f;

		//밀도
		float m_ProxyDensity = DEFAULT_DENSITY;

		//경사, 계단 처리 관련 속성
		float m_SlopeLimit = 0.f; //slopeLimit = cosf(PxMath::degToRad(45.0f));
		float m_ContactOffset = 0.1f; //Default: 0.1
		float m_StepOffset = 0.f;

		//slopeLimit 값에 의해 자동 생성되는 추가 삼각형(보이지 않는 벽)의 높이, 0.0이면 생성되지 않음
		float m_InvisibleWallHeight = 0.f;

		//경사, 계단 처리 등의 위해 캐싱 할 주변 범위 스케일,
		//일단 1.0 보다 커야하고 좋은 성능을 위해 2.0 보다 작은 것이 좋음
		//https://gameworksdocs.nvidia.com/PhysX/4.1/documentation/physxapi/files/classPxControllerDesc.html#a93930e888fcd4c2f7bafeac83621612e
		float m_VolumeGrowth = 1.5f;
	
		//Collision Callback Object, 유저 오브젝트 포인터
		ISimulationEventCallback* m_pOwnerCallback = nullptr;

	public:

		//캡슐 모양 CCT 생성자
		CharacterController(PhysicsEngineMain& physicsEngine, ISimulationEventCallback* ownerCallback,
			Vector3 initPos, float radius, float bodyHeight) :
			m_PhysicsEngine(physicsEngine), m_Position(initPos),
			m_Radius(radius), m_BodyHeight(bodyHeight), m_Type(CCTShapeType::Capsule),
			m_pOwnerCallback(ownerCallback)
		{}

		//박스 모양 CCT 생성자
		CharacterController(PhysicsEngineMain& physicsEngine, ISimulationEventCallback* ownerCallback,
			Vector3 initPos, Vector3 size) :
			m_PhysicsEngine(physicsEngine), m_Position(initPos),
			m_Size(size), m_Type(CCTShapeType::Box),
			m_pOwnerCallback(ownerCallback)
		{}

		~CharacterController() = default;

		//생성후 초기화
		void Initialize();
		
		//물리 시뮬레이션 전에 실행되는 position 정보 업데이트
		CCTCollisionDirFlags UpdatePosition(Vector3 displacement, float dTime);
		
		//물리 시뮬레이션 이후 position 정보 가져오는 함수
		void GetPosition(Vector3& position);
		void GetFootPosition(Vector3& position);

		//활성화/비활성화 설정
		void SetActive(bool isActive);

		void SetQueryActive(bool isActive);
		//충돌처리 callback 활성화/비활성화 설정/확인
		void SetCollionCBActive(bool isCollionCBActive);
		bool IsCollionCBActive() {
			if (m_pRigidbody)
			{
				return m_pRigidbody->m_IsCollionCBActive;
			}
			
			return false;
		}
		
		//필터 설정
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
		//CharacterController 생성/초기화
		static CharacterController* CreateCapsule(ISimulationEventCallback* ownerCallback,
			Vector3 initPos, float radius, float height,
			unsigned int filterGroup = 0, unsigned int filterMask = 0);
		static CharacterController* CreateBox(ISimulationEventCallback* ownerCallback,
			Vector3 initPos, Vector3 size,
			unsigned int filterGroup = 0, unsigned int filterMask = 0);
		
		//SetSize 테스트용 함수
		void SetSize(float radius, float bodyheight)
		{
			m_pRigidbody->SetSize(radius, bodyheight);
		}
	};
}



