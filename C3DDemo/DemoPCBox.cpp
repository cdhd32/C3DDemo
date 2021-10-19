#include "DemoPCBox.h"

using namespace PhysicsEngine;

void DemoPCBox::FixedUpdate(float dTime)
{
	if (IsKeyDown(0x57)) //W
	{
		m_Pos += Vector3(0, 0, m_Speed * dTime);
	}

	if (IsKeyDown(0x41)) //A
	{
		m_Pos += Vector3(-m_Speed * dTime, 0, 0);
	}

	if (IsKeyDown(0x53)) //S
	{
		m_Pos += Vector3(0, 0, -m_Speed * dTime);
	}

	if (IsKeyDown(0x44)) //D
	{
		m_Pos += Vector3(m_Speed * dTime, 0, 0);
	}

	Quaternion qurtRot;
	EulerToQuaternion(m_Rot, qurtRot);

	m_pRigidBody->UpdateTransformFixed(m_Pos, qurtRot, dTime);


}
