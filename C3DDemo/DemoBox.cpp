#include "DemoBox.h"

#include "../C3DEngine/Graphics/C3DShader.h"
#include "../C3DEngine/Objects/C3DMaterial.h"
#include "../C3DEngine/System/C3DTransform.h"
#include "../C3DEngine/Objects/C3DObject.h"

using namespace PhysicsEngine;

VERTEX	BOXVTX[] = {
// 정면.  ( Face#0) :    
		// 좌표,    노멀★,   UV (TextureCoordinate) 
		{ -0.5f, 0.5f, -0.5f, 0, 0, -1, 0, 0 },
		{ 0.5f, 0.5f,-0.5f,  0, 0,-1,  1, 0 },
		{ -0.5f,-0.5f,-0.5f,  0, 0,-1,  0, 1 },
			//       ( Face#1)
		{ -0.5f,-0.5f,-0.5f,  0, 0,-1,  0, 1 },
		{ 0.5f, 0.5f,-0.5f,  0, 0,-1,  1, 0 },
		{ 0.5f,-0.5f,-0.5f,  0, 0,-1,  1, 1 },

			//뒷면.  (Face#3) :  
		{ 0.5f, 0.5f, 0.5f,  0, 0, 1,  0, 0 },
		{ -0.5f, 0.5f, 0.5f,  0, 0, 1,  1, 0 },
		{ -0.5f,-0.5f, 0.5f,  0, 0, 1,  1, 1 },
			//		  (Face#4)
		{ 0.5f, 0.5f, 0.5f,  0, 0, 1,  0, 0 },
		{ -0.5f,-0.5f, 0.5f,  0, 0, 1,  1, 1 },
		{ 0.5f,-0.5f, 0.5f,  0, 0, 1,  0, 1 },


			// 우측면. (Face#5)
		{ 0.5f, 0.5f,-0.5f,  1, 0, 0,  0, 0 },
		{ 0.5f, 0.5f, 0.5f,  1, 0, 0,  1, 0 },
		{ 0.5f,-0.5f,-0.5f,  1, 0, 0,  0, 1 },
			//			(Face#6)
		{ 0.5f,-0.5f,-0.5f,  1, 0, 0,  0, 1 },
		{ 0.5f, 0.5f, 0.5f,  1, 0, 0,  1, 0 },
		{ 0.5f,-0.5f, 0.5f,  1, 0, 0,  1, 1 },


			// 좌측면. (Face#7)
		{ -0.5f, 0.5f, 0.5f,  -1, 0, 0,  0, 0 },
		{ -0.5f, 0.5f,-0.5f,  -1, 0, 0,  1, 0 },
		{ -0.5f,-0.5f,-0.5f,  -1, 0, 0,  1, 1 },
			//			(Face#8)
		{ -0.5f, 0.5f, 0.5f,  -1, 0, 0,  0, 0 },
		{ -0.5f,-0.5f,-0.5f,  -1, 0, 0,  1, 1 },
		{ -0.5f,-0.5f, 0.5f,  -1, 0, 0,  0, 1 },


			//상부.  ( Face#9)
		{ -0.5f, 0.5f, 0.5f,  0, 1, 0,  0, 0 },
		{ 0.5f, 0.5f, 0.5f,  0, 1, 0,  1, 0 },
		{ -0.5f, 0.5f,-0.5f,  0, 1, 0,  0, 1 },
			//		 ( Face#10)
		{ -0.5f, 0.5f,-0.5f,  0, 1, 0,  0, 1 },
		{ 0.5f, 0.5f, 0.5f,  0, 1, 0,  1, 0 },
		{ 0.5f, 0.5f,-0.5f,  0, 1, 0,  1, 1 },


			//하부.  ( Face#11)
		{ 0.5f,-0.5f, 0.5f,  0,-1, 0,  0, 0 },
		{ -0.5f,-0.5f, 0.5f,  0,-1, 0,  1, 0 },
		{ -0.5f,-0.5f,-0.5f,  0,-1, 0,  1, 1 },
			//		 ( Face#12)
		{ 0.5f,-0.5f, 0.5f,  0,-1, 0,  0, 0 },
		{ -0.5f,-0.5f,-0.5f,  0,-1, 0,  1, 1 },
		{ 0.5f,-0.5f,-0.5f,  0,-1, 0,  0, 1 },
	};

unsigned int BOXVTXNUM = 36;

void DemoBox::Initialze()
{
	m_pMaterial = new C3DMaterial(C3DMaterial::DIFFUSE_RED);
	m_pTransform = new C3DTransform(m_Pos, m_Rot, m_Scale);

	//m_pModel = CreateBox(m_Size.x, m_Size.y, m_Size.z);
	m_pModel = BOXVTX;

	// 버텍스 버퍼 생성
	UINT size = sizeof(VERTEX) * BOXVTXNUM;
	m_pShader->CreateVB(m_pModel, size, &m_pVB);

	// 
	Quaternion qurtRot;
	EulerToQuaternion(m_Rot, qurtRot);
	m_pRigidBody = Rigidbody::CreateBox(this, RigidbodyType::Dynamic, m_Pos, qurtRot, m_Scale);
}

void DemoBox::FixedUpdate(float dTime)
{
	//떨어지기 
	//m_Pos -= Vector3(0.f, 0.098f, 0.f);

	
}

void DemoBox::Update()
{

}

void DemoBox::RenderUpdate()
{
	//piece의 world 좌표를 surface TM의 부모 좌표로 설정
	//m_pTransform->SetParentTransform(&_pieceTransform);
	Quaternion qurtRot;
	EulerToQuaternion(m_Rot, qurtRot);

	m_pRigidBody->UpdateAfterSimulate(m_Pos, qurtRot);

	//m_pRigidBody->GetTransform(m_Pos, qurtRot);
	QuaternionToEuler(qurtRot, m_Rot);

	m_pTransform->SetPosition(m_Pos);
	m_pTransform->SetRotation(m_Rot);

	/// 쉐이더에 전달
	m_pShader->SetWorldTM(m_pTransform);
	m_pShader->SetMaterial(m_pMaterial);

	m_pShader->UpdateTempCB();
	m_pShader->UpdateConstantBuffer();

	//쉐이더 set
	m_pShader->ShaderUpdate();

	//IA에 버텍스 버퍼 설정(인덱스 버퍼는 없음)
	m_pShader->SetVBtoIASimple(m_pVB, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, sizeof(VERTEX));

	//그리기!
	m_pShader->DrawFullVtx(BOXVTXNUM);
}

void DemoBox::Finalize()
{
	m_pRigidBody->Release();
}