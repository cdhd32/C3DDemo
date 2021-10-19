#pragma once

#include "Collider.h"

class BoxCollider : public Collider
{
	//상호 작용하는 클래스
	friend class Transform;
	friend class Rigidbody;

private:
	Vector3 m_Size = Vector3(1.f, 1.f, 1.f);

public:
	BoxCollider(Vector3 center, Vector3 size, Transform* pTransform) :
		Collider(center, pTransform, eBOX), m_Size(size)
	{}

	~BoxCollider() = default;

};

