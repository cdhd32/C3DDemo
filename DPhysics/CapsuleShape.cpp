#include "pch.h"

#include "PhysicsEngine.h"
#include "CapsuleShape.h"

void PhysicsEngine::CapsuleShape::SetSize(float radius, float bodyHeight)
{
	//음수 size 예외 처리
	PX_ASSERT(radius > 0.f | halfHeight > 0.f);

	m_Radius = radius;
	m_BodyHeight = bodyHeight;

	PxCapsuleGeometry capsule;
	m_pPxShape->getCapsuleGeometry(capsule);

	capsule.radius = m_Radius;
	//m_BodyHeight에 절반으로 나눈다(중요)
	capsule.halfHeight = m_BodyHeight / 2.f;

	m_pPxShape->setGeometry(capsule);
}
