#pragma once

class Transform
{
	//상호 작용하는 클래스
	friend class Rigidbody;
	friend class CharacterController;

	//용도별 분류
	enum TypeEnum
	{
		eMODEL = (unsigned int)(1 << 0),
		eLOCAL = (unsigned int)(1 << 1),
		eWORLD = (unsigned int)(1 << 2), eGLOBAL = eWORLD,
		eVIEW = (unsigned int)(1 << 3), eCAMERA = eVIEW,
		//ePROJECTION = (unsigned int)(1 << 4),
		//eVIEWPORT = (unsigned int)(1 << 5), eSCREEN = eVIEWPORT,
		eNONE = 0
	};

private:
	//TRS 정보, 회전은 쿼터니언을 사용하되 외부에서 오일러 각 입력(Vector3)으로 변경할 수 있게 한다.
	Vector3 m_Position = Vector3(0, 0, 0);
	Quaternion m_Rotation = Quaternion::Identity;
	Vector3 m_Scale = Vector3(1.f, 1.f, 1.f);

	//world Transform 렌더링에 사용하는 4x4 행렬
	Matrix m_TransformMatrix = Matrix::Identity;

	//업데이트 이전 위치 정보, 이전 위치 정보와 비교(!=)하여 백엔드(물리, 렌더링 엔진 등) 업데이트/Set 함수의 call 횟수를 줄인다
	Vector3 m_Pos_Prev = Vector3(0, 0, 0);
	Quaternion m_Rot_Prev = Quaternion::Identity;
	Vector3 m_Scale_Prev = Vector3(1.f, 1.f, 1.f);

	//Fixed update에서 m_EnlapsedTime으로 변경된 데이터를 accept 할지 rollback 할지 결정? #checkpoint
	float m_EnlapsedTime = 0.f;

	//로컬 TR(S) 정보, 나중에 빠지거나 월드-로컬 변환이 되게 만들 수도 있음
	//현재 사용하는 모델 중심축(pivot)이 밑면 중앙으로 설정되어 있어 외부(NewBox)에서 변경하여 사용 #수정
	Vector3 m_LocalPosition = Vector3(0, 0, 0);
	Quaternion m_LocalRotation = Quaternion::Identity;
	Vector3 m_LocalScale = Vector3(1.f, 1.f, 1.f);
	Matrix m_LocalMatrix = Matrix::Identity;

public:
	Transform() = default;

	
	Transform(Vector3 pos, Quaternion quatRot, Vector3 scale = Vector3(1.f, 1.f, 1.f))
		: m_Position(pos), m_Rotation(quatRot), m_Scale(scale),
		m_Pos_Prev(m_Position), m_Rot_Prev(m_Rotation), m_Scale_Prev(m_Scale)
	{}

	//오일러 각을 입력받는 생성자
	Transform(Vector3 pos, Vector3 rot = Vector3(0, 0, 0), Vector3 scale = Vector3(1.f, 1.f, 1.f))
		: m_Position(pos),
		m_Rotation(Quaternion::CreateFromYawPitchRoll(rot.y, rot.x, rot.z)),
		m_Scale(scale)
		, m_Pos_Prev(m_Position), m_Rot_Prev(m_Rotation), m_Scale_Prev(m_Scale)
	{}

	~Transform() = default;
	
	void Initialize();
	void FixedUpdate(float dTime);
	void Update();
	void Finalize();

	//Getter & Setter
	Vector3 GetPosition() const { return m_Position; }
	void SetPosition(Vector3 val) { m_Position = val; }

	Quaternion GetRotationQuaternion() const { return m_Rotation; }
	void SetRotationQuaternion(Quaternion val) { m_Rotation = val; }
	
	//euler rotation setter/getter
	Vector3 GetRotation()
	{
		Vector3 eulerRotation;
		QuaternionToEuler(m_Rotation, eulerRotation);
		return eulerRotation;
	}

	void SetRotation(Vector3 eulerRotation)
	{
		m_Rotation = Quaternion::CreateFromYawPitchRoll(eulerRotation.y, eulerRotation.x, eulerRotation.z);
	}

	Vector3 GetScale() const { return m_Scale; }
	void SetScale(Vector3 val) { m_Scale = val; }

	Matrix GetTransformMatrix() const { return m_TransformMatrix; }

	//local transform
	Vector3 GetLocalPosition() const { return m_LocalPosition; }
	void SetLocalPosition(Vector3 val) { m_LocalPosition = val; }
	Quaternion GetLocalRotation() const { return m_LocalRotation; }
	void SetLocalRotation(Vector3 eulerRotation)
	{
		m_LocalRotation = Quaternion::CreateFromYawPitchRoll(eulerRotation.y, eulerRotation.x, eulerRotation.z);
	}
	void SetLocalRotation(Quaternion val) { m_LocalRotation = val; }
	Vector3 GetLocalScale() const { return m_LocalScale; }
	void SetLocalScale(Vector3 val) { m_LocalScale = val; }

	//helper functions
	//p오일러각 - 쿼터니언 변환 함수
	//http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm
	//http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
	static void QuaternionToEuler(const Quaternion& quat /*in*/, Vector3& angle /*out*/);
	static void EulerToQuaternion(const Vector3& angle /*in*/, Quaternion& quat/*out*/);
};
