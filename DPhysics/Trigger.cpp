#include "pch.h"

#include "PhysicsEngine.h"

#include "Shape.h"

#include "BoxShape.h"
#include "SphereShape.h"
#include "CapsuleShape.h"
#include "PlaneShape.h"
#include "TriangleMeshShape.h"
#include "Trigger.h"

using namespace PhysicsEngine;

Trigger* Trigger::CreateBox(RigidbodyType type, Vector3 position, Quaternion rotation, Vector3 size, Vector3 centerPos /*= Vector3::Zero*/)
{
	static PhysicsEngineMain& physicEngine = PhysicsEngineMain::GetInstance();

	Shape* pBoxShape = new BoxShape(physicEngine, size, centerPos);

	Trigger* pTriggerBody= new Trigger(physicEngine,
		pBoxShape, type, position, rotation);

	pTriggerBody->Initialize();

	pBoxShape->SetTriggerShapeFlag(true);
	if (pTriggerBody->m_Type == RigidbodyType::Kinematic)
	{
		pTriggerBody->m_pPxRigidDynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	}
	

	return pTriggerBody;
}

Trigger* Trigger::CreateCapsule(RigidbodyType type, Vector3 position, Quaternion rotation, float radius, float bodyHeight, Vector3 centerPos /*= Vector3::Zero*/)
{
	Rigidbody::CreateCapsule(type, position, rotation, radius, bodyHeight, centerPos);
}

Trigger* Trigger::CreateSphere(RigidbodyType type, Vector3 position, Quaternion rotation, float radius, Vector3 centerPos /*= Vector3::Zero*/)
{
	Rigidbody::CreateSphere(type, position, rotation, radius, centerPos);
}

