#include "pch.h"

#include "PhysicsEngine.h"
#include "SphereShape.h"

void PhysicsEngine::SphereShape::SetSize(float radius)
{
	//음수 size 예외 처리
	PX_ASSERT(radius > 0.f);

	m_Radius = radius;

	PxSphereGeometry sphere;

	m_pPxShape->getSphereGeometry(sphere);

	sphere.radius = radius;

	m_pPxShape->setGeometry(sphere);
	
}
