#include "pch.h"

#include "Component.h"
#include "QuadCollider.h"

//-z forward direction, center pivot
// (-0.5, 0.5) ----------- (0.5, 0.5)
//			   |		 |
//			   |	'	 | <--- center(') (0, 0)
//			   |		 |
// (-0.5, -0.5)----------- (0.5, -0.5)
//      
const QuadCollider::QUAD QuadCollider::VTXDATA = {
		Vector3(-0.5f, 0.5f, 0),
		Vector3(0.5f, 0.5f, 0),
		Vector3(-0.5f, -0.5f, 0),
		Vector3(0.5f, -0.5f, 0)
};

const unsigned int QuadCollider::FACEDATA[6] = { 0, 1, 2,
												3, 2, 1 };

const unsigned int QuadCollider::VERTNNUM = 6;

const unsigned int QuadCollider::FACENUM = 2;

void QuadCollider::Initialize()
{
	//ũ�⿡ ���� ���� ���ؽ� ��ġ �ٸ��� �����ϱ�
	//m_pVertData�� VTXDATA�� ������ �ϱ� ������ (const void*)
	//���� �޽������͸� �����ؼ� ���� ������ ������ �ٽ� �����ϰ� �Ѵ�
	//Ŭ���� ���𿡼� �ƿ� �Ž� �����͸� �����ϰ� �ִ°� ���� �� ���� #����

	QUADVTX* pVtxdata;

	if (m_Size != Vector2(1, 1))
	{
		pVtxdata = new QUADVTX();

		pVtxdata->topLeft.x *= m_Size.x;
		pVtxdata->topRight.x *= m_Size.x;
		pVtxdata->bottomLeft.x *= m_Size.x;
		pVtxdata->bottomRight.x *= m_Size.x;

		pVtxdata->topLeft.y *= m_Size.y;
		pVtxdata->topRight.y *= m_Size.y;
		pVtxdata->bottomLeft.y *= m_Size.y;
		pVtxdata->bottomRight.y *= m_Size.y;

		m_pVertData = (const void*)pVtxdata;
	}

	TriangleMeshCollider::Initialize();
}

void QuadCollider::Finalize()
{
	//���� �� �Ž��� �����ؼ� ��ü �ϹǷ� ������ ���⼭ �Ѵ�.
	//�ٵ� ��ӹ��� ������ �ּҷ� ������ �ϴ� �� �̻��ϴ�...

	if (m_Size != Vector2(1, 1) && m_pVertData)
	{
		delete[] m_pVertData;
		m_pVertData = nullptr;
	}


	TriangleMeshCollider::Finalize();
}



