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
	//크기에 따라 원본 버텍스 위치 다르게 조절하기
	//m_pVertData는 VTXDATA를 참조만 하기 때문에 (const void*)
	//새로 메쉬데이터를 생성해서 값을 변경한 다음에 다시 참조하게 한다
	//클래스 선언에서 아예 매쉬 데이터를 저장하고 있는게 좋을 것 같다 #수정

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
	//직접 새 매쉬를 생성해서 대체 하므로 해제를 여기서 한다.
	//근데 상속받은 변수의 주소로 해제를 하는 게 이상하다...

	if (m_Size != Vector2(1, 1) && m_pVertData)
	{
		delete[] m_pVertData;
		m_pVertData = nullptr;
	}


	TriangleMeshCollider::Finalize();
}



