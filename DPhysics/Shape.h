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

		PxFilterData m_SimFilterData; //시뮬레이션 필터 정보
		PxFilterData m_QueryFilterData; //쿼리 필터 정보

	public:
		Shape(PhysicsEngineMain& physicsEngine, ShapeType type, Vector3 centerPos)
			: m_PhysicsEngine(physicsEngine), m_type(type), m_CenterPos(centerPos)
		{

		}

		~Shape() = default;

	protected:
		//word0, 1 set, 인게임 충돌 그룹에 대해서 시뮬레이션 설정과 쿼리 그룹 설정을 이원화
		void SetShapeSimulateFilterGroup(unsigned int filterGroup, unsigned int FilterMask);
		void SetShapeQueryFilterGroup(unsigned int filterGroup, unsigned int FilterMask);
		
		//한번에 FilterData set
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

