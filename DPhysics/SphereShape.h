#pragma once
#include "Shape.h"

namespace PhysicsEngine
{
	class SphereShape :
		public Shape
	{
		friend class Rigidbody;

	private:
		float m_Radius = 1.0f;

	public:
		SphereShape(PhysicsEngineMain& physicsEngine,
			float radius, Vector3 centerPos = Vector3(0, 0, 0)) :
			Shape(physicsEngine, ShapeType::Sphere, centerPos), m_Radius(radius)
		{}

		~SphereShape() = default;
	private:
		void SetSize(float radius);
	};
}

