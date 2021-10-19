#pragma once
#include "Collider.h"

//�簢�� �浹ü
class QuadCollider : public TriangleMeshCollider
{
	//��ȣ �ۿ��ϴ� Ŭ����
	friend class Transform;
	friend class Rigidbody;

public:
	//quad ������ �����ϴ� 
	struct QUADVTX {
		Vector3 topLeft;
		Vector3	topRight;
		Vector3 bottomLeft;
		Vector3 bottomRight;

	public:
		QUADVTX() : topLeft(VTXDATA.quad.topLeft),
			topRight(VTXDATA.quad.topRight),
			bottomLeft(VTXDATA.quad.bottomLeft),
			bottomRight(VTXDATA.quad.bottomRight)
		{}
	};

	//quad ����ü
	union QUAD
	{
		Vector3 vtx[4];
		QUADVTX quad;
	};

private:
	static const unsigned int VERTNNUM;
	static const unsigned int FACENUM;
	static const QUAD VTXDATA;
	static const unsigned int FACEDATA[6];

	Vector2 m_Size = Vector2(1.f, 1.f);
	Vector3 m_Normal = Vector3(0.f, 0.f, -1.f); //-z forward

public:
	QuadCollider(Vector3 center, Transform* pTransform, Vector2 size) :
		TriangleMeshCollider(center, pTransform,
			(unsigned int)VERTNNUM, (unsigned int)FACENUM,
			(const void*)&VTXDATA, (const void*)FACEDATA), m_Size(size)
	{}

	void Initialize();
	void Finalize();

	~QuadCollider() = default;
};

