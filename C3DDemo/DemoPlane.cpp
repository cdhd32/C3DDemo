#include "DemoPlane.h"

#include "../C3DEngine/Graphics/C3DShader.h"
#include "../C3DEngine/Objects/C3DMaterial.h"
#include "../C3DEngine/System/C3DTransform.h"
#include "../C3DEngine/Objects/C3DObject.h"

using namespace PhysicsEngine;

void DemoPlane::Initialze()
{
	m_pTransform = new C3DTransform(m_Pos, m_Rot, m_Scale);

	//Gird �߰��ϱ�

	// ���ؽ� ���� ����
	/*UINT size = sizeof(VERTEX) * BOXVTXNUM;
	m_pShader->CreateVB(m_pModel, size, &m_pVB);*/

	//
	m_pRigidBody = Rigidbody::CreatePlaneStatic(this, m_Normal, m_Distance);
}

void DemoPlane::FixedUpdate(float dTime)
{

}

void DemoPlane::Update()
{

}

void DemoPlane::RenderUpdate()
{

}

void DemoPlane::Finalize()
{
	m_pRigidBody->Release();
}
