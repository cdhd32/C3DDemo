#pragma once
#include "Shape.h"

namespace PhysicsEngine
{
	class BoxShape : public Shape
	{
		friend class Rigidbody;

	private:

		Vector3 m_Size = Vector3(1.f, 1.f, 1.f);

	public:
		BoxShape(PhysicsEngineMain& physicsEngine, Vector3 size, Vector3 centerPos = Vector3(0, 0, 0)) :
			Shape(physicsEngine, ShapeType::Box, centerPos), m_Size(size)
		{}

		~BoxShape() = default;
	private:
		void SetSize(Vector3 size);
	};
}


