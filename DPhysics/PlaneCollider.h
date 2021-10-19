#pragma once
#include "Collider.h"

class PlaneCollider : public Collider
{
	//��ȣ �ۿ��ϴ� Ŭ����
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
	//��� Ŭ�������� ����ϴ� ������
	PlaneCollider(Vector3 center, Vector3 normal, float distance, Transform* pTransform, COLLIDERTYPE type) :
		Collider(center, pTransform, type), m_Normal(normal), m_Distance(distance)
	{}

	~PlaneCollider() = default;

	void Initilze();
	void Update();
	void Finalize();
};

