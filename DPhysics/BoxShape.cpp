#include "pch.h"

#include "PhysicsEngine.h"
#include "BoxShape.h"

void PhysicsEngine::BoxShape::SetSize(Vector3 size)
{
	//음수 size 예외 처리
	PX_ASSERT(size.x > 0.f | size.y > 0.f | size.z > 0.f);

	m_Size = size;

	PxBoxGeometry box;
	m_pPxShape->getBoxGeometry(box);

	//내부에서는 m_Size의 절반으로 나누어 사용(halfextent);
	box.halfExtents = ToPxVec3(m_Size / 2.f);

	m_pPxShape->setGeometry(box);
}
