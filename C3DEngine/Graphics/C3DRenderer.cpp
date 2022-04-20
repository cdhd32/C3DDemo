#include "../pch.h"
#include "C3DDevice.h"
#include "C3DShader.h"

#include "../System/C3DTimer.h"

#include "../Objects/C3DCamera.h"
#include "../Objects/C3DLight.h"
#include "../Objects/C3DMaterial.h"
#include "../Objects/C3DObject.h"

#include "../Objects/C3DGrid.h"

#include "C3DRenderer.h"

// �׽�Ʈ �� �ڽ� ��
VERTEX	g_vtxCube[] = {
	{ -1.0, 1.0,-1.0 ,  0.0, 0.0,-1.0,  0.0, 0.0 },		//v0	
	{  1.0, 1.0,-1.0 ,  0.0, 0.0,-1.0,  1.0, 0.0 },		//v1
	{ -1.0,-1.0,-1.0 ,  0.0, 0.0,-1.0,  0.0, 1.0 },		//v2
	//       ( Face#1)
	{ -1.0,-1.0,-1.0 ,  0.0, 0.0,-1.0 ,  0.0, 1.0 },
	{  1.0, 1.0,-1.0 ,  0.0, 0.0,-1.0 ,  1.0, 0.0 },
	{  1.0,-1.0,-1.0 ,  0.0, 0.0,-1.0 ,  1.0, 1.0 },

	//�޸�.  (Face#3) :  
	{  1.0, 1.0, 1.0 ,  0.0, 0.0, 1.0,  0.0, 0.0 },
	{ -1.0, 1.0, 1.0 ,  0.0, 0.0, 1.0,  1.0, 0.0 },
	{ -1.0,-1.0, 1.0 ,  0.0, 0.0, 1.0,  1.0, 1.0 },
	//		  (Face#4)
	{  1.0, 1.0, 1.0 ,  0.0, 0.0, 1.0,  0.0, 0.0 },
	{ -1.0,-1.0, 1.0 ,  0.0, 0.0, 1.0,  1.0, 1.0 },
	{  1.0,-1.0, 1.0 ,  0.0, 0.0, 1.0,  0.0, 1.0 },


	// ������. (Face#5)
	{  1.0, 1.0,-1.0 ,  1.0, 0.0, 0.0,  0.0, 0.0 },
	{  1.0, 1.0, 1.0 ,  1.0, 0.0, 0.0,  1.0, 0.0 },
	{  1.0,-1.0,-1.0 ,  1.0, 0.0, 0.0,  0.0, 1.0 },
	//			(Face#6)
	{  1.0,-1.0,-1.0 ,  1.0, 0.0, 0.0,  0.0, 1.0 },
	{  1.0, 1.0, 1.0 ,  1.0, 0.0, 0.0,  1.0, 0.0 },
	{  1.0,-1.0, 1.0 ,  1.0, 0.0, 0.0,  1.0, 1.0 },


	// ������. (Face#7)
	{ -1.0, 1.0, 1.0 ,  -1.0, 0.0, 0.0,  0.0, 0.0 },
	{ -1.0, 1.0,-1.0 ,  -1.0, 0.0, 0.0,  1.0, 0.0 },
	{ -1.0,-1.0,-1.0 ,  -1.0, 0.0, 0.0,  1.0, 1.0 },
	//			(Face#8)
	{ -1.0, 1.0, 1.0 ,  -1.0, 0.0, 0.0,  0.0, 0.0 },
	{ -1.0,-1.0,-1.0 ,  -1.0, 0.0, 0.0,  1.0, 1.0 },
	{ -1.0,-1.0, 1.0 ,  -1.0, 0.0, 0.0,  0.0, 1.0 },


	//���.  ( Face#9)
	{ -1.0, 1.0, 1.0 ,  0.0, 1.0, 0.0,  0.0, 0.0 },
	{  1.0, 1.0, 1.0 ,  0.0, 1.0, 0.0,  1.0, 0.0 },
	{ -1.0, 1.0,-1.0 ,  0.0, 1.0, 0.0,  0.0, 1.0 },
	//		 ( Face#10)
	{ -1.0, 1.0,-1.0 ,  0.0, 1.0, 0.0,  0.0, 1.0 },
	{  1.0, 1.0, 1.0 ,  0.0, 1.0, 0.0,  1.0, 0.0 },
	{  1.0, 1.0,-1.0 ,  0.0, 1.0, 0.0,  1.0, 1.0 },


	//�Ϻ�.  ( Face#11)
	{  1.0,-1.0, 1.0 , 0.0,-1.0, 0.0,  0.0, 0.0 },
	{ -1.0,-1.0, 1.0 , 0.0,-1.0, 0.0,  1.0, 0.0 },
	{ -1.0,-1.0,-1.0 , 0.0,-1.0, 0.0,  1.0, 1.0 },
	//		 ( Face#12)
	{  1.0,-1.0, 1.0 , 0.0,-1.0, 0.0,  0.0, 0.0 },
	{ -1.0,-1.0,-1.0 , 0.0,-1.0, 0.0,  1.0, 1.0 },
	{  1.0,-1.0,-1.0 , 0.0,-1.0, 0.0,  0.0, 1.0 },

};

// �𵨿� ���Ϲ���.
ID3D11Buffer* g_pVB = nullptr;		//���� ���� ������.

C3DRenderer::C3DRenderer()
	:m_hWnd(NULL), m_pDevice(nullptr),
	m_pShader(nullptr),
	m_pLineShader(nullptr),

	m_pCam(nullptr),
	m_pDirLight(nullptr),
	m_pMtrl(nullptr)
{

}

C3DRenderer::~C3DRenderer()
{

}

int C3DRenderer::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	//C3DDevice ���� & �ʱ�ȭ
	m_pDevice = new C3DDevice();
	m_pDevice->Initialize(m_hWnd);

	return S_OK;
}

int C3DRenderer::DataLoading()
{
	HRESULT hr = S_OK;
	//���Ͽ��� �ε�

	m_pDevice->CreateVB(&g_vtxCube, sizeof(g_vtxCube), &g_pVB);

	

	//���̴� ����, Device�� DirectXDC�� �������̽��� ��´�
	m_pShader = new C3DShader(m_pDevice);
	m_pShader->ShaderSetup(L"../Data/fx/Default(perVtx).fx", SHADERTYPE::SHADER_DEFALT);

	m_pLineShader = new C3DShader(m_pDevice);
	m_pLineShader->ShaderSetup(L"../Data/fx/Default_WireFrm.fx", SHADERTYPE::SHADER_LINE);

	//Camera 
	m_pCam = new C3DCamera(
		Vector3(-5.0f, 5.0f, -5.0f),
		Vector3( 0.0f, 0.0f, 0.0f),
		Vector3( 0.0f, 1.0f, 1.0f),
		60.f,
		(float)g_Mode.Width / (float)g_Mode.Height,
		1.0f, 1000.0f
	);

	//Light
	m_pDirLight = new C3DLight();
	//m_pDirLight->SetUp();

	//Material
	m_pMtrl = new C3DMaterial();

	//���̴��� ������ۿ� ������ ��ü set
	m_pShader->SetCam(m_pCam);
	m_pShader->SetLight(m_pDirLight);
	m_pShader->SetMaterial(m_pMtrl);

	m_pLineShader->SetCam(m_pCam);

	

	//m_pShader->CreateDynamicConstantBuffer();

	return hr;
}

//������Ʈ ���⼭...
void C3DRenderer::SceneUpdate()
{
	//�� ������Ʈ �� dTime�� �����´�.
	float dTime = C3DTimer::GetInstanse()->GetDeltaTime();

	//���� ���� ������Ʈ, ���� ���� �ٸ� ������ ȣ���ϴ� �� ���� �� ����. #����
	m_pDevice->RenderStateUpdate();

	//�� ������Ʈ ������Ʈ

	//ObjUpdate(dTime);
	m_pCam->UpdateCamera(dTime);
	m_pDirLight->Update();
	m_pMtrl->Update();

	//������Ʈ�� worldTM �����
	XMMATRIX mTM = XMMatrixIdentity();
	XMMATRIX _mScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX _mRot = XMMatrixRotationRollPitchYaw(0, 0, 0);
	XMMATRIX _mTrans = XMMatrixTranslation(0, 0, 0);
	mTM = _mScale * _mRot * _mTrans;

	//m_pShader->SetTramsform2();// ��������� view, proj �� ���� 

	m_pShader->SetTransform(TS_WORLD, mTM);//������Ʈ worldTM ������ ������Ʈ

	m_pGrid->Update(dTime);



	//�� ������Ʈ ������Ʈ
	

	//�ӽ� ������� ������Ʈ
	m_pShader->UpdateTempCB();

	//������� ������Ʈ
	m_pShader->UpdateConstantBuffer();
}

//�������� ���⼭...
void C3DRenderer::SceneRender()
{
	//�� ������Ʈ Draw

	//���̴� set
	//m_pShader->ShaderUpdate();
	
	//IA�� ���ؽ� ���� ����(�ε��� ���۴� ����)
	//m_pShader->SetVBtoIASimple(g_pVB, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, sizeof(VERTEX));

	//�׸���! Render a triangle
	//m_pShader->DrawFullVtx(36);

	m_pGrid->Draw();
	//�� ������Ʈ Draw
}

void C3DRenderer::DataRelease()
{
	SAFE_RELEASE(g_pVB);
}
