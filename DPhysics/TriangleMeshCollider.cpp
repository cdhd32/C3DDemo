#include "pch.h"

#include "PhysicsEngine.h"
#include "Component.h"
#include "TriangleMeshCollider.h"


void TriangleMeshCollider::Initialize()
{
	CookingMeshData(m_VertNum, m_FaceNum, m_pVertData, m_pFaceData);
}

void TriangleMeshCollider::Finalize()
{
	if (m_pTriangleMeshGeometry)
	{
		delete m_pTriangleMeshGeometry;
		m_pTriangleMeshGeometry = nullptr;
	}
}

void TriangleMeshCollider::CookingMeshData(unsigned int vertNum,
	unsigned int faceNum, const void* pVertData, const void* pFaceData)
{
	//불안하지만 xyz 순으로 정렬이 되어있다 가정하고...#수정

	PhysicsEngine::TriangleMeshData meshdata{
		vertNum, faceNum, (const float*)pVertData, (PxU32*)pFaceData
	};

	 m_pTriangleMeshGeometry = PhysicsEngine::PhysicsEngineMain::GetInstance().CreateTriAngleMeshGeometry(&meshdata);
}