#pragma once
#include "Collider.h"

class TriangleMeshCollider : public Collider
{
	//��ȣ �ۿ��ϴ� Ŭ����
	friend class Transform;
	friend class Rigidbody;

protected:
	//TriangleMesh Ư��
	unsigned int m_VertNum;		//���ؽ� ����
	unsigned int m_FaceNum;		//���̽� ����
	const void* m_pVertData;	//���ؽ� ������, const void* ���� �������� ����ϴ� �ڷ����� ����� �������
	const void* m_pFaceData;	//���̽� ������, const void* ���� �������� ����ϴ� �ڷ����� ����� �������

	//physx �ɹ�
	//Cooking �� Geometry Model
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

	//�ٸ� ����ü, Ŭ������ �ְ� ���� #�߰�
	//void SetMeshData(unsigned int vertNum, unsigned int faceNum, const void* pVertData, const void* pFaceData);
	//void GetMeshData(unsigned int vertNum, unsigned int faceNum, const void* pVertData, const void* pFaceData);
};
