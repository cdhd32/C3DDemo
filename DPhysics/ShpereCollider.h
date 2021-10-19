#pragma once
#include "Collider.h"
class ShpereCollider : public Collider
{
	//��ȣ �ۿ��ϴ� Ŭ����
	friend class Transform;
	friend class Rigidbody;

private:
	float m_Radius = 1.0f;

public:
	ShpereCollider(Vector3 center, float radius, Transform* pTransform) :
		Collider(center, pTransform, eSHPERE), m_Radius(radius)
	{}

	~ShpereCollider() = default;

};

