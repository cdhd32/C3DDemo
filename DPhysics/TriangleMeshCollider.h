#pragma once
#include "Collider.h"

class TriangleMeshCollider : public Collider
{
	//상호 작용하는 클래스
	friend class Transform;
	friend class Rigidbody;

protected:
	//TriangleMesh 특성
	unsigned int m_VertNum;		//버텍스 갯수
	unsigned int m_FaceNum;		//페이스 갯수
	const void* m_pVertData;	//버텍스 데이터, const void* 말고 엔진에서 사용하는 자료형을 만들어 사용하자
	const void* m_pFaceData;	//페이스 데이터, const void* 말고 엔진에서 사용하는 자료형을 만들어 사용하자

	//physx 맴버
	//Cooking 된 Geometry Model
	PxTriangleMeshGeometry* m_pTriangleMeshGeometry = nullptr;

public:
	TriangleMeshCollider(Vector3 center, Transform* pTransform,
		unsigned int vertNum, unsigned int faceNum, const void* pVertData, const void* pFaceData)
		: Collider(center, pTransform, eTRIANLEMESH),
		m_VertNum(vertNum), m_FaceNum(faceNum),
		m_pVertData(pVertData), m_pFaceData(pFaceData)
	{}

	void Initialize();
	void Finalize();

private:
	void CookingMeshData(unsigned int vertNum,
		unsigned int faceNum,
		const void* pVertData,
		const void* pFaceData
	);

	//다른 구조체, 클래스로 주고 받자 #추가
	//void SetMeshData(unsigned int vertNum, unsigned int faceNum, const void* pVertData, const void* pFaceData);
	//void GetMeshData(unsigned int vertNum, unsigned int faceNum, const void* pVertData, const void* pFaceData);
};
