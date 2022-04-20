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

// 테스트 용 박스 모델
VERTEX	g_vtxCube[] = {
	{ -1.0, 1.0,-1.0 ,  0.0, 0.0,-1.0,  0.0, 0.0 },		//v0	
	{  1.0, 1.0,-1.0 ,  0.0, 0.0,-1.0,  1.0, 0.0 },		//v1
	{ -1.0,-1.0,-1.0 ,  0.0, 0.0,-1.0,  0.0, 1.0 },		//v2
	//       ( Face#1)
	{ -1.0,-1.0,-1.0 ,  0.0, 0.0,-1.0 ,  0.0, 1.0 },
	{  1.0, 1.0,-1.0 ,  0.0, 0.0,-1.0 ,  1.0, 0.0 },
	{  1.0,-1.0,-1.0 ,  0.0, 0.0,-1.0 ,  1.0, 1.0 },

	//뒷면.  (Face#3) :  
	{  1.0, 1.0, 1.0 ,  0.0, 0.0, 1.0,  0.0, 0.0 },
	{ -1.0, 1.0, 1.0 ,  0.0, 0.0, 1.0,  1.0, 0.0 },
	{ -1.0,-1.0, 1.0 ,  0.0, 0.0, 1.0,  1.0, 1.0 },
	//		  (Face#4)
	{  1.0, 1.0, 1.0 ,  0.0, 0.0, 1.0,  0.0, 0.0 },
	{ -1.0,-1.0, 1.0 ,  0.0, 0.0, 1.0,  1.0, 1.0 },
	{  1.0,-1.0, 1.0 ,  0.0, 0.0, 1.0,  0.0, 1.0 },


	// 우측면. (Face#5)
	{  1.0, 1.0,-1.0 ,  1.0, 0.0, 0.0,  0.0, 0.0 },
	{  1.0, 1.0, 1.0 ,  1.0, 0.0, 0.0,  1.0, 0.0 },
	{  1.0,-1.0,-1.0 ,  1.0, 0.0, 0.0,  0.0, 1.0 },
	//			(Face#6)
	{  1.0,-1.0,-1.0 ,  1.0, 0.0, 0.0,  0.0, 1.0 },
	{  1.0, 1.0, 1.0 ,  1.0, 0.0, 0.0,  1.0, 0.0 },
	{  1.0,-1.0, 1.0 ,  1.0, 0.0, 0.0,  1.0, 1.0 },


	// 좌측면. (Face#7)
	{ -1.0, 1.0, 1.0 ,  -1.0, 0.0, 0.0,  0.0, 0.0 },
	{ -1.0, 1.0,-1.0 ,  -1.0, 0.0, 0.0,  1.0, 0.0 },
	{ -1.0,-1.0,-1.0 ,  -1.0, 0.0, 0.0,  1.0, 1.0 },
	//			(Face#8)
	{ -1.0, 1.0, 1.0 ,  -1.0, 0.0, 0.0,  0.0, 0.0 },
	{ -1.0,-1.0,-1.0 ,  -1.0, 0.0, 0.0,  1.0, 1.0 },
	{ -1.0,-1.0, 1.0 ,  -1.0, 0.0, 0.0,  0.0, 1.0 },


	//상부.  ( Face#9)
	{ -1.0, 1.0, 1.0 ,  0.0, 1.0, 0.0,  0.0, 0.0 },
	{  1.0, 1.0, 1.0 ,  0.0, 1.0, 0.0,  1.0, 0.0 },
	{ -1.0, 1.0,-1.0 ,  0.0, 1.0, 0.0,  0.0, 1.0 },
	//		 ( Face#10)
	{ -1.0, 1.0,-1.0 ,  0.0, 1.0, 0.0,  0.0, 1.0 },
	{  1.0, 1.0, 1.0 ,  0.0, 1.0, 0.0,  1.0, 0.0 },
	{  1.0, 1.0,-1.0 ,  0.0, 1.0, 0.0,  1.0, 1.0 },


	//하부.  ( Face#11)
	{  1.0,-1.0, 1.0 , 0.0,-1.0, 0.0,  0.0, 0.0 },
	{ -1.0,-1.0, 1.0 , 0.0,-1.0, 0.0,  1.0, 0.0 },
	{ -1.0,-1.0,-1.0 , 0.0,-1.0, 0.0,  1.0, 1.0 },
	//		 ( Face#12)
	{  1.0,-1.0, 1.0 , 0.0,-1.0, 0.0,  0.0, 0.0 },
	{ -1.0,-1.0,-1.0 , 0.0,-1.0, 0.0,  1.0, 1.0 },
	{  1.0,-1.0,-1.0 , 0.0,-1.0, 0.0,  0.0, 1.0 },

};

// 모델용 기하버퍼.
ID3D11Buffer* g_pVB = nullptr;		//정점 버퍼 포인터.

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

	//C3DDevice 생성 & 초기화
	m_pDevice = new C3DDevice();
	m_pDevice->Initialize(m_hWnd);

	return S_OK;
}

int C3DRenderer::DataLoading()
{
	HRESULT hr = S_OK;
	//파일에서 로딩

	m_pDevice->CreateVB(&g_vtxCube, sizeof(g_vtxCube), &g_pVB);

	

	//쉐이더 생성, Device와 DirectXDC의 인터페이스를 얻는다
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

	//쉐이더에 상수버퍼와 연관된 객체 set
	m_pShader->SetCam(m_pCam);
	m_pShader->SetLight(m_pDirLight);
	m_pShader->SetMaterial(m_pMtrl);

	m_pLineShader->SetCam(m_pCam);

	

	//m_pShader->CreateDynamicConstantBuffer();

	return hr;
}

//업데이트 여기서...
void C3DRenderer::SceneUpdate()
{
	//매 업데이트 시 dTime을 가져온다.
	float dTime = C3DTimer::GetInstanse()->GetDeltaTime();

	//랜더 상태 업테이트, 여기 보다 다른 곳에서 호출하는 게 좋을 것 같다. #수정
	m_pDevice->RenderStateUpdate();

	//신 오브젝트 업데이트

	//ObjUpdate(dTime);
	m_pCam->UpdateCamera(dTime);
	m_pDirLight->Update();
	m_pMtrl->Update();

	//오브젝트의 worldTM 만들기
	XMMATRIX mTM = XMMatrixIdentity();
	XMMATRIX _mScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	XMMATRIX _mRot = XMMatrixRotationRollPitchYaw(0, 0, 0);
	XMMATRIX _mTrans = XMMatrixTranslation(0, 0, 0);
	mTM = _mScale * _mRot * _mTrans;

	//m_pShader->SetTramsform2();// 상수버퍼의 view, proj 만 갱신 

	m_pShader->SetTransform(TS_WORLD, mTM);//오브젝트 worldTM 정보만 업데이트

	m_pGrid->Update(dTime);



	//신 오브젝트 업데이트
	

	//임시 상수버퍼 업데이트
	m_pShader->UpdateTempCB();

	//상수버퍼 업데이트
	m_pShader->UpdateConstantBuffer();
}

//랜더링은 여기서...
void C3DRenderer::SceneRender()
{
	//신 오브젝트 Draw

	//쉐이더 set
	//m_pShader->ShaderUpdate();
	
	//IA에 버텍스 버퍼 설정(인덱스 버퍼는 없음)
	//m_pShader->SetVBtoIASimple(g_pVB, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, sizeof(VERTEX));

	//그리기! Render a triangle
	//m_pShader->DrawFullVtx(36);

	m_pGrid->Draw();
	//신 오브젝트 Draw
}

void C3DRenderer::DataRelease()
{
	SAFE_RELEASE(g_pVB);
}
