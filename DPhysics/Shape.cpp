#include "pch.h"

#include "PhysicsEngine.h"
#include "Shape.h"

void PhysicsEngine::Shape::Release()
{
	//PX_ASSERT(m_pPxShape);
	if (m_pPxShape)
	{
		m_pPxShape->release();
		m_pPxShape = NULL;
	}
}

void PhysicsEngine::Shape::SetTriggerShapeFlag(bool isTrigger)
{
	//eSIMULATION_SHAPE와 eTRIGGER_SHAPE는 둘다 set이 되면 안됨
	m_pPxShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
	m_pPxShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
}

void PhysicsEngine::Shape::SetShapeSimulateFilterGroup(unsigned int filterGroup, unsigned int FilterMask)
{
	m_SimFilterData.word0 = filterGroup;
	m_SimFilterData.word1 = FilterMask;

	m_pPxShape->setSimulationFilterData(m_SimFilterData);
}

void PhysicsEngine::Shape::SetShapeQueryFilterGroup(unsigned int filterGroup, unsigned int FilterMask)
{
	m_QueryFilterData.word0 = filterGroup;
	m_QueryFilterData.word1 = FilterMask;

	m_pPxShape->setQueryFilterData(m_QueryFilterData);
}

void PhysicsEngine::Shape::SetFilterData(PxFilterData& filterData)
{
	m_SimFilterData = filterData;
	m_QueryFilterData = filterData;

	m_pPxShape->setSimulationFilterData(m_SimFilterData);
	m_pPxShape->setQueryFilterData(m_QueryFilterData);
}
