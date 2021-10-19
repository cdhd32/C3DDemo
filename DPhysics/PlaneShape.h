#pragma once
#include "Shape.h"

namespace PhysicsEngine
{
	class PlaneShape :
		public Shape
	{
		friend class Rigidbody;

	private:
		Vector3 m_Normal = Vector3(0.f, 1.f, 0.f); //defualt y-up(+)
		float m_Distance = 0.f;

	public:
		PlaneShape(PhysicsEngineMain& physicsEngine,
			Vector3 normal, float distance, Vector3 centerPos = Vector3(0, 0 ,0)) :
			Shape(physicsEngine, ShapeType::Plane, centerPos),
			m_Normal(normal), m_Distance(distance)
		{}

		~PlaneShape() = default;
	
	public:
		//plane normal에서 회전각 계산 기능 구현되면 추가 #수정
		//void SetNormal(Vector3 normal);
		//void SetDistance(Vector3 distance);
		//void Rotate()
	};
}

