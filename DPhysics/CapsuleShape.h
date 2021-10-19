#pragma once
#include "Shape.h"

namespace PhysicsEngine
{
	class CapsuleShape :
		public Shape
	{
		friend class Rigidbody;

	private:
		float m_Radius = 1.0f;
		float m_BodyHeight;

	public:
		CapsuleShape(PhysicsEngineMain& physicsEngine,
			float radius, float bodyHeight, Vector3 centerPos = Vector3(0, 0, 0)) :
			Shape(physicsEngine, ShapeType::Sphere, centerPos),
			m_Radius(radius), m_BodyHeight(bodyHeight)
		{}

		~CapsuleShape() = default;

	private:
		void SetSize(float radius, float bodyHeight);
	};

}