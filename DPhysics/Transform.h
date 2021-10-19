#pragma once

class Transform
{
	//��ȣ �ۿ��ϴ� Ŭ����
	friend class Rigidbody;
	friend class CharacterController;

	//�뵵�� �з�
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
	//TRS ����, ȸ���� ���ʹϾ��� ����ϵ� �ܺο��� ���Ϸ� �� �Է�(Vector3)���� ������ �� �ְ� �Ѵ�.
	Vector3 m_Position = Vector3(0, 0, 0);
	Quaternion m_Rotation = Quaternion::Identity;
	Vector3 m_Scale = Vector3(1.f, 1.f, 1.f);

	//world Transform �������� ����ϴ� 4x4 ���
	Matrix m_TransformMatrix = Matrix::Identity;

	//������Ʈ ���� ��ġ ����, ���� ��ġ ������ ��(!=)�Ͽ� �鿣��(����, ������ ���� ��) ������Ʈ/Set �Լ��� call Ƚ���� ���δ�
	Vector3 m_Pos_Prev = Vector3(0, 0, 0);
	Quaternion m_Rot_Prev = Quaternion::Identity;
	Vector3 m_Scale_Prev = Vector3(1.f, 1.f, 1.f);

	//Fixed update���� m_EnlapsedTime���� ����� �����͸� accept ���� rollback ���� ����? #checkpoint
	float m_EnlapsedTime = 0.f;

	//���� TR(S) ����, ���߿� �����ų� ����-���� ��ȯ�� �ǰ� ���� ���� ����
	//���� ����ϴ� �� �߽���(pivot)�� �ظ� �߾����� �����Ǿ� �־� �ܺ�(NewBox)���� �����Ͽ� ��� #����
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

	//���Ϸ� ���� �Է¹޴� ������
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
	//p���Ϸ��� - ���ʹϾ� ��ȯ �Լ�
	//http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm
	//http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
	static void QuaternionToEuler(const Quaternion& quat /*in*/, Vector3& angle /*out*/);
	static void EulerToQuaternion(const Vector3& angle /*in*/, Quaternion& quat/*out*/);
};
