#include "C3DModel.h"

#include "../C3DEngine/Graphics/C3DShader.h"
#include "../C3DEngine/Objects/C3DMaterial.h"
#include "../C3DEngine/System/C3DTransform.h"
#include "../C3DEngine/Objects/C3DObject.h"

void C3DModel::Initialze(C3DShader* pShader)
{
	m_pMaterial = new C3DMaterial(C3DMaterial::DIFFUSE_RED);
	m_pTransform = new C3DTransform(m_Pos, m_Rot, m_Scale);

	//m_pModel = CreateBox(m_Size.x, m_Size.y, m_Size.z);
	//m_pModel = BOXVTX;

	m_pShader = pShader;

	// ���ؽ� ���� ����
	UINT size = sizeof(VERTEX) * m_VtxNum;
	m_pShader->CreateVB(m_pModel, size, &m_pVB);
}

void C3DModel::FixedUpdate(float dTime)
{
}

void C3DModel::Update()
{
}

void C3DModel::RenderUpdate()
{
	m_pTransform->SetPosition(m_Pos);
	m_pTransform->SetRotation(m_Rot);

	/// ���̴��� ����
	m_pShader->SetWorldTM(m_pTransform);
	m_pShader->SetMaterial(m_pMaterial);

	m_pShader->UpdateTempCB();
	m_pShader->UpdateConstantBuffer();

	//���̴� set
	m_pShader->ShaderUpdate();

	//IA�� ���ؽ� ���� ����(�ε��� ���۴� ����)
	m_pShader->SetVBtoIASimple(m_pVB, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, sizeof(VERTEX));

	//�׸���!
	m_pShader->DrawFullVtx(m_VtxNum);
}

void C3DModel::Finalize()
{
}