#pragma once
#include "Collider.h"

class PlaneCollider : public Collider
{
	//상호 작용하는 클래스
	friend class Transform;
	friend class Rigidbody;

private:
	Vector3 m_Normal = Vector3(0.f, 1.f, 0.f); //defualt y-up(+)
	float m_Distance = 0.f; 

public:
	PlaneCollider(Vector3 center, Vector3 normal, float distance, Transform* pTransform) :
		Collider(center, pTransform, ePLANE), m_Normal(normal), m_Distance(distance)
	{}

protected:
	//상속 클래스에서 사용하는 생성자
	PlaneCollider(Vector3 center, Vector3 normal, float distance, Transform* pTransform, COLLIDERTYPE type) :
		Collider(center, pTransform, type), m_Normal(normal), m_Distance(distance)
	{}

	~PlaneCollider() = default;

	void Initilze();
	void Update();
	void Finalize();
};

