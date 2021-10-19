#pragma once
#include "Collider.h"

class CapsuleCollider : public Collider
{
	//��ȣ �ۿ��ϴ� Ŭ����
	friend class Transform;
	friend class Rigidbody;

private:
	float m_Radius;
	float m_HalfHeight;
	
public:
	CapsuleCollider(Vector3 center, float radius, float halfHeight, Transform* pTransform) :
		Collider(center, pTransform, eCAPSULE), m_Radius(radius), m_HalfHeight(halfHeight)
	{}

	~CapsuleCollider() = default;

	void Initilze();
	void Update();
	void Finalize();
};

