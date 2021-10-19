#pragma once
#include "Shape.h"

namespace PhysicsEngine
{
	class TriangleMeshShape : public Shape
	{
		friend class Rigidbody;

	private:
		//음수 scale이 들어가면 face normal이 뒤집힘,
		//overlap()을 제외한 query나 시뮬레이션에는 앞뒤 face 노멀 둘다 계산함,
		//최적화를 위해 한쪽 노멀만 사용하게 하고, 바깥쪽 노멀로 정확하게 계산하게 만들어야한다. #수정
		Vector3 m_Scale = Vector3(1.f, 1.f, 1.f);
		const TriangleMeshData& m_MeshData;

		//cooking 된 meshdata를 가지고 있는 멤버
		PxTriangleMeshGeometry* m_pPxCookedMeshGeo = nullptr;

	public:
		TriangleMeshShape(PhysicsEngineMain& physicsEngine, const TriangleMeshData& meshData, Vector3 scale, Vector3 centerPos) :
			Shape(physicsEngine, ShapeType::TriangleMesh, centerPos), m_Scale(scale), m_MeshData(meshData)
		{}

		~TriangleMeshShape() = default;

	private:
		//rigidbody 생성전 사용, 현재는 RigidBody 내부에서 사용
		//음수 scale이 들어가면 face normal이 뒤집힘
		void CreateTriangleMesh()
		{
			m_pPxCookedMeshGeo = m_PhysicsEngine.CreateTriAngleMeshGeometry(&m_MeshData, ToPxVec3(m_Scale));
		}

		//음수 scale이 들어가면 face normal이 뒤집힘
		void SetScale(Vector3 scale);
		
	};
}