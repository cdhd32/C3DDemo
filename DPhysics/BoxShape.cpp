#include "pch.h"

#include "PhysicsEngine.h"
#include "BoxShape.h"

void PhysicsEngine::BoxShape::SetSize(Vector3 size)
{
	//���� size ���� ó��
	PX_ASSERT(size.x > 0.f | size.y > 0.f | size.z > 0.f);

	m_Size = size;

	PxBoxGeometry box;
	m_pPxShape->getBoxGeometry(box);

	//���ο����� m_Size�� �������� ������ ���(halfextent);
	box.halfExtents = ToPxVec3(m_Size / 2.f);

	m_pPxShape->setGeometry(box);
}
