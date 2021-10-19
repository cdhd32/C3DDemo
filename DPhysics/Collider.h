#pragma once

class Transform;

enum COLLIDERTYPE
{
	eBOX,
	eSHPERE,
	eCAPSULE,
	ePLANE,
	eTRIANLEMESH,
	eNONE = -1
};

//�浹ü Ŭ����, 
class Collider
{
//��ȣ �ۿ��ϴ� Ŭ����
protected:
	Transform* m_pTransform = nullptr;

protected:
	Vector3 m_CenterPosition = Vector3(0, 0, 0);
	Quaternion m_CenterRotation = Quaternion::Identity;
	bool m_IsTrigger = false; // Ʈ���� ó�� ����, true�̸� RigidBody ������� ����

	COLLIDERTYPE m_Collidertype = eNONE;

	//PxMaterial m_PhysicalMaterial; �ٸ� ��ü�� ����� ���
	//BoundingBox �߰�
public:
	Collider() = default;

	Collider(Vector3 centerPos, Transform* pTransform, COLLIDERTYPE type = eNONE) :
		m_CenterPosition(centerPos), m_pTransform(pTransform), m_Collidertype(type)
	{}

	~Collider() = default;

	//Ʈ���� ���� �Լ� �߰� ����
	//virtual void OnTriggerEnter();
	//virtual void OnTriggerStay();
	//virtual void OnTriggerExit();

	Vector3 GetCenterPosition() const { return m_CenterPosition; }
	void SetCenterPosition(Vector3 val) { m_CenterPosition = val; }
};

