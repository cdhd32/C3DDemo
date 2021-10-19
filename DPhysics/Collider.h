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

//충돌체 클래스, 
class Collider
{
//상호 작용하는 클래스
protected:
	Transform* m_pTransform = nullptr;

protected:
	Vector3 m_CenterPosition = Vector3(0, 0, 0);
	Quaternion m_CenterRotation = Quaternion::Identity;
	bool m_IsTrigger = false; // 트리거 처리 여부, true이면 RigidBody 사용하지 않음

	COLLIDERTYPE m_Collidertype = eNONE;

	//PxMaterial m_PhysicalMaterial; 다른 객체로 만들어 사용
	//BoundingBox 추가
public:
	Collider() = default;

	Collider(Vector3 centerPos, Transform* pTransform, COLLIDERTYPE type = eNONE) :
		m_CenterPosition(centerPos), m_pTransform(pTransform), m_Collidertype(type)
	{}

	~Collider() = default;

	//트리거 관련 함수 추가 예정
	//virtual void OnTriggerEnter();
	//virtual void OnTriggerStay();
	//virtual void OnTriggerExit();

	Vector3 GetCenterPosition() const { return m_CenterPosition; }
	void SetCenterPosition(Vector3 val) { m_CenterPosition = val; }
};

