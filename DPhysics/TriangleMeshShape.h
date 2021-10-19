#pragma once
#include "Shape.h"

namespace PhysicsEngine
{
	class TriangleMeshShape : public Shape
	{
		friend class Rigidbody;

	private:
		//���� scale�� ���� face normal�� ������,
		//overlap()�� ������ query�� �ùķ��̼ǿ��� �յ� face ��� �Ѵ� �����,
		//����ȭ�� ���� ���� ��ָ� ����ϰ� �ϰ�, �ٱ��� ��ַ� ��Ȯ�ϰ� ����ϰ� �������Ѵ�. #����
		Vector3 m_Scale = Vector3(1.f, 1.f, 1.f);
		const TriangleMeshData& m_MeshData;

		//cooking �� meshdata�� ������ �ִ� ���
		PxTriangleMeshGeometry* m_pPxCookedMeshGeo = nullptr;

	public:
		TriangleMeshShape(PhysicsEngineMain& physicsEngine, const TriangleMeshData& meshData, Vector3 scale, Vector3 centerPos) :
			Shape(physicsEngine, ShapeType::TriangleMesh, centerPos), m_Scale(scale), m_MeshData(meshData)
		{}

		~TriangleMeshShape() = default;

	private:
		//rigidbody ������ ���, ����� RigidBody ���ο��� ���
		//���� scale�� ���� face normal�� ������
		void CreateTriangleMesh()
		{
			m_pPxCookedMeshGeo = m_PhysicsEngine.CreateTriAngleMeshGeometry(&m_MeshData, ToPxVec3(m_Scale));
		}

		//���� scale�� ���� face normal�� ������
		void SetScale(Vector3 scale);
		
	};
}