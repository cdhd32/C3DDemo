#pragma once

namespace PhysicsEngine
{
	enum class ShapeType : unsigned int
	{
		Box = 0,
		Sphere = 1,
		Capsule = 2,

		Plane = 3,
		TriangleMesh = 4
	};

	class Shape
	{
		friend class Rigidbody;
		friend class CharacterController;
		friend class QueryOverlap;
	protected:

		PhysicsEngineMain& m_PhysicsEngine;
		
		PxShape* m_pPxShape = nullptr;
		ShapeType m_type;

		Vector3 m_CenterPos;

		PxFilterData m_SimFilterData; //�ùķ��̼� ���� ����
		PxFilterData m_QueryFilterData; //���� ���� ����

	public:
		Shape(PhysicsEngineMain& physicsEngine, ShapeType type, Vector3 centerPos)
			: m_PhysicsEngine(physicsEngine), m_type(type), m_CenterPos(centerPos)
		{

		}

		~Shape() = default;

	protected:
		//word0, 1 set, �ΰ��� �浹 �׷쿡 ���ؼ� �ùķ��̼� ������ ���� �׷� ������ �̿�ȭ
		void SetShapeSimulateFilterGroup(unsigned int filterGroup, unsigned int FilterMask);
		void SetShapeQueryFilterGroup(unsigned int filterGroup, unsigned int FilterMask);
		
		//�ѹ��� FilterData set
		void SetFilterData(PxFilterData& filterData);

		virtual void SetSize(Vector3 size) {};
		virtual void SetSize(float radius) {};
		virtual void SetSize(float radius, float bodyHeight) {};
		virtual void SetScale(Vector3 scale) {};

		void Release();
	public:
		DirectX::SimpleMath::Vector3 GetCenterPos() const { return m_CenterPos; }

		
	protected:
		void SetTriggerShapeFlag(bool isTrigger);
	};

}

