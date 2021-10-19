#include "pch.h"

#include "PhysicsEngine.h"
#include "TriangleMeshShape.h"

void PhysicsEngine::TriangleMeshShape::SetScale(Vector3 scale)
{
	m_Scale = scale;

	m_pPxCookedMeshGeo->scale.scale = ToPxVec3(m_Scale);

	m_pPxShape->setGeometry(*m_pPxCookedMeshGeo);
}
