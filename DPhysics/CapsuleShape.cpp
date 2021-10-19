#include "pch.h"

#include "PhysicsEngine.h"
#include "CapsuleShape.h"

void PhysicsEngine::CapsuleShape::SetSize(float radius, float bodyHeight)
{
	//���� size ���� ó��
	PX_ASSERT(radius > 0.f | halfHeight > 0.f);

	m_Radius = radius;
	m_BodyHeight = bodyHeight;

	PxCapsuleGeometry capsule;
	m_pPxShape->getCapsuleGeometry(capsule);

	capsule.radius = m_Radius;
	//m_BodyHeight�� �������� ������(�߿�)
	capsule.halfHeight = m_BodyHeight / 2.f;

	m_pPxShape->setGeometry(capsule);
}
