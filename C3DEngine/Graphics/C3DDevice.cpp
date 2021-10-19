#pragma once
#include "../pch.h"
// DXTK, DXTex 설정하고 yena 구조 참고해서 만들어보기기
#include "C3DDevice.h"

//장치 설정 기본 정보. DXGI_MODE_DESC
//DXGI_MODE_DESC g_Mode = { 1200, 800, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };
//DXGI_MODE_DESC g_Mode = { 800, 600, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };
//DXGI_MODE_DESC g_Mode = { 960, 600, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//16:10
//DXGI_MODE_DESC g_Mode = { 1200, 600, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//2:1
//DXGI_MODE_DESC g_Mode = { 1280, 720, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//16:9
DXGI_MODE_DESC g_Mode = { 1920, 1080, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//16:9 FHD
//DXGI_MODE_DESC g_Mode = { 2560, 1440, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//16:9 QHD
//DXGI_MODE_DESC g_Mode = { 3440, 1440, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//21:9 4K WQHD
//DXGI_MODE_DESC g_Mode = { 3840, 2160, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//16:9 4K UHD

// 배경색
const Vector4 C3DDevice::CLEAR_COLOR_DBLUE = XMFLOAT4(0, 0.125f, 0.3f, 1.0f);
const Vector4 C3DDevice::CLEAR_COLOR_GRAY = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0);

C3DDevice::C3DDevice()
	: m_hWnd(nullptr),
	m_pDevice(nullptr),
	m_pDXDC(nullptr),
	m_pSwapChain(nullptr),
	m_pRTView(nullptr),
	m_pDS(nullptr),
	m_pDSView(nullptr),

	m_RMode(RM_DEFAULT),

	m_bCullBack(FALSE),
	m_bWireFrame(FALSE),
	m_bZEnable(TRUE),
	m_bZWrite(TRUE)

{

}

C3DDevice::~C3DDevice()
{

}

HRESULT C3DDevice::Initialize(HWND hWnd)
{
	//DX 셋업
	_DirectXSetup(hWnd);

	// sys 셋업 : 기타 시스템 공통 객체 제공 
	// 내장 기본 카메라, 조명, 폰트, 방향축, 그리드, 등등..
	//SysSetup(g_pDevice, g_Mode, 5.0f);

	return S_OK;
}

void C3DDevice::Release()
{
	//DX 해제
	_DirectXRelease();
}

HRESULT C3DDevice::CreateVB(void* pBuff, UINT size, ID3D11Buffer** ppVB)
{
	ID3D11Buffer* pVB = nullptr;
	HRESULT hr = S_OK;

	//정점 버퍼 정보 생성
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				//버퍼 사용방식
	bd.ByteWidth = size;						//버퍼 크기
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//버퍼 용도 : "정점 버퍼" 용로 설정 
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = pBuff;					//버퍼에 들어갈 데이터 설정 : "정점들"..

	//정점 버퍼 생성.
	hr = m_pDevice->CreateBuffer(&bd, &rd, &pVB);
	if (FAILED(hr))
	{
		//에러 검증 및 예외처리..
		MessageBox(NULL, L"Failed, CreateVB on Model", L"Error_Model", MB_OK);
		return hr;
	}
		
	//외부로 리턴.
	*ppVB = pVB;

	return hr;
}

HRESULT C3DDevice::CreateIB(void* pBuff, UINT size, ID3D11Buffer** ppIB)
{
	ID3D11Buffer* pIB = nullptr;

	// 인덱스 버퍼 정보 구성.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;					//버퍼 사용방식
	bd.ByteWidth = size;							//버퍼 크기
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;			//버퍼 용도 : "색인 버퍼" 용로 설정 
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = pBuff;								//버퍼에 들어갈 데이터 설정 : "색인 정보"..

	//색인 버퍼 생성.
	HRESULT hr = m_pDevice->CreateBuffer(&bd, &rd, &pIB);
	if (FAILED(hr))
	{
		//에러 검증 및 예외처리..
		//...
		MessageBox(NULL, L"Failed, CreateIB on Model", L"Error_Model", MB_OK);
		return hr;
	}

	//외부로 리턴.
	*ppIB = pIB;

	return hr;
}

void C3DDevice::RTClear(Vector4 col)
{
	//[주의] 렌터타겟과 뷰포트 설정.
	// DX9 과 다르게 DX10/11 에서의 ClearRenderTarget 메소드는
	// 렌더타겟뷰 전체를 지우며 Viewport 및 Scissor 설정은 적용되지 않습니다.
	// ...
	// 해결1 : DX11.1 / ID3D11DeviceContext1::ClearView 메소드 사용.
	// 해결2 : "Drawing Quad" , 직교투영/사각형 메쉬로 영역 지우기 

	//렌더타겟(백버퍼)/ 깊이버퍼 및 스텐실버퍼 지우기..
	_ClearBackBuffer(D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,	//초기화 플래그.
		col,			//배경색.
		1.0f,			//깊이 버퍼 초기값.
		0				//스텐실 버퍼 초기값.
	);
}

// 장면 그리기 종료.
void C3DDevice::Flip()
{
	m_pSwapChain->Present(m_bVSync, 0);			//화면출력 : Flip! (+수직동기화)
}

HRESULT C3DDevice::_DirectXSetup(HWND hWnd)
{
	// D3D 렌더링 장치 Device 및 스왑체인 Swap Chain 생성. 
	_CreateDeviceSwapChain(hWnd);

	// 장치-스왑체인의 렌더타겟(백버퍼) 획득
	_CreateRenderTarget();

	// 깊이/스텐실 버퍼 생성.
	_CreateDepthStencil();

	// 장치 출력병합기(Output Merger) 에 렌터링 타겟 및 깊이-스텐실 버퍼 등록.
	m_pDXDC->OMSetRenderTargets(
		1,				// 렌더타겟 개수.(max: D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
		&m_pRTView,		// 렌더타겟("백버퍼") 등록.	
		m_pDSView		// 깊이/스텐실 버퍼 등록.
	);

	// 뷰포트 설정
	_SetViewPort();

	//장치 정보 획득
	//GetDeviceInfo();



	// 기타 렌더링 관련 추가 설정.
	// 렌더링에 필요한 사용자 객체등을 생성/관리 합니다.
	// 카메라, 조명, 폰트, 셰이더 등등...

	//렌더링 상태 객체 생성.
	_RenderStateObjectCreate();

	// 기본 (공용) 셰이더 생성.
	//ShaderSetup();

	// Input? 셋업.           
	//----------------------------------------
	/*if (!ynInput_Initial())
	{
		return YN_FAIL;
	}*/

	//전체모드로 강제전환.
	//g_pSwapChain->SetFullscreenState(TRUE, NULL);

	return S_OK;
}

void C3DDevice::_DirectXRelease()
{
	//장치 상태 리셋 : 제거 전에 초기화를 해야 합니다. (메모리 누수 방지)
	if (m_pDXDC) m_pDXDC->ClearState();

	//상태 객체 제거.
	_RenderStateObjectRelease();

	//기본 셰이더 제거.
	//ShaderRelease();

	SAFE_RELEASE(m_pDS);				//깊이/스텐실 버퍼 제거.
	SAFE_RELEASE(m_pDSView);
	SAFE_RELEASE(m_pRTView);			//렌더타겟 제거.
	SAFE_RELEASE(m_pSwapChain);			//스왑체인 제거.
	SAFE_RELEASE(m_pDXDC);
	SAFE_RELEASE(m_pDevice);			//디바이스 제거. 맨 나중에 제거합니다.
}

int C3DDevice::_ClearBackBuffer(UINT flag, XMFLOAT4 col, float depth /*= 1.0f*/, UINT stencil /*= 0*/)
{
	m_pDXDC->ClearRenderTargetView(m_pRTView, (float*)&col);			//렌더타겟 지우기.
	m_pDXDC->ClearDepthStencilView(m_pDSView, flag, depth, stencil);	//깊이/스텐실 지우기.

	return S_OK;
}

float C3DDevice::_GetEngineTime()
{
	LARGE_INTEGER frequency, nowtime;
	static LARGE_INTEGER oldtime{0, 0};
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&nowtime);
	float dTime = (float)(nowtime.QuadPart - oldtime.QuadPart) * 0.001f;
	oldtime = nowtime;

	return dTime;
}

HRESULT C3DDevice::_CreateDeviceSwapChain(HWND hwnd)
{
	HRESULT hr = S_OK;

	// 장치 (Device) 및 스왑체인(SwapChain) 정보 구성.
	// 스왑체인은 다중버퍼링 시스템을 말하며
	// 고전적인 '플립핑Flipping' 체인과 동일한 의미입니다.  
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Windowed = m_bWindowMode;				//풀스크린 또는 창모드 선택.
	sd.OutputWindow = hwnd;						//출력할 윈도우 핸들.
	sd.BufferCount = 1;							//백버퍼 개수.
	sd.BufferDesc.Width = g_Mode.Width;			//해상도 결정.(백버퍼 크기)
	sd.BufferDesc.Height = g_Mode.Height;
	sd.BufferDesc.Format = g_Mode.Format;		//백버퍼 색상규격 (A8R8G8B8) 창모드에서는 생략 가능 
	sd.BufferDesc.RefreshRate.Numerator = m_bVSync ? 60 : 0;   //버퍼 갱신율.(수직동기화 VSync 활성화시 표준갱신율 적용 : 60hz)
	//sd.BufferDesc.RefreshRate.Numerator = 0;	//버퍼 갱신율.(수직동기화 VSync Off)
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//용도 설정: '렌더타겟' 
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//sd.Flags = 0;
	sd.SampleDesc.Count = m_dwAA;					//AA 설정
	sd.SampleDesc.Quality = 0;



	// D3D 렌더링 '장치(Device)' 및 스왑체인 생성. 
	//
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,						//비디오 어뎁터 포인터 (기본장치는 NULL)
		D3D_DRIVER_TYPE_HARDWARE,	//HW 가속 
		NULL,						//SW Rasterizer DLL 핸들.  HW 가속시에는 NULL.
		0,							//디바이스 생성 플래그.(기본값)
		//D3D11_CREATE_DEVICE_DEBUG,//디바이스 생성 플래그.(디버그)
		&m_FeatureLevels,			//(생성할) 디바이스 기능 레벨(Feature Level) 배열
		1,							//(생성할) 디바이스 기능 레벨(Feature Level) 배열 크기.
		D3D11_SDK_VERSION,			//DX SDK 버전.
		&sd,						//디바이스 생성 상세 옵션.
		&m_pSwapChain,				//[출력] 스왑체인 인터페이스 얻기.
		&m_pDevice,					//[출력] 디바이스 인터페이스 얻기.
		NULL,						//[출력] (생성된) 디바이스 기능 레벨. 필요없다면 NULL 설정. 
		&m_pDXDC					//[출력] 디바이스 컨텍스트 얻기.
	);

	if (FAILED(hr))
	{
		//hr 정보도 따로 출력 가능하게...#수정
		MessageBox(m_hWnd, L"디바이스 / 스왑체인 생성 실패.", L"Error", MB_OK);
	}

	return hr;
}

HRESULT C3DDevice::_CreateRenderTarget()
{
	HRESULT hr = S_OK;

	// 백버퍼 획득.
	ID3D11Texture2D* pBackBuffer;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);		// 백버퍼 얻기, 2D 텍스쳐 행태, 화면출력용
	if (FAILED(hr))	return hr;

	//획득한 백버퍼에 렌더타겟 뷰 생성.(렌더타겟'형'으로 설정함)
	hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRTView);		// '렌더타겟뷰'를 생성.	
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"백버퍼-렌더타겟뷰 생성 실패.", L"Error", MB_OK);
	}

	//리소스 뷰 생성 후, 불필요한 DX 핸들은 해제해야 합니다.(메모리 누수 방지)
	SAFE_RELEASE(pBackBuffer);
	
	return hr;
}

HRESULT C3DDevice::_CreateDepthStencil()
{
	HRESULT hr = S_OK;

	//깊이/스텐실 버퍼용 정보 구성.
	D3D11_TEXTURE2D_DESC   td;
	ZeroMemory(&td, sizeof(td));
	td.Width = g_Mode.Width;
	td.Height = g_Mode.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // DXGI_FORMAT_D32_FLOAT;			// 32BIT. 깊이 버퍼.
	td.SampleDesc.Count = m_dwAA;					// AA 설정 - RT 과 동일 규격 준수.
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;	// 깊이-스텐실 버퍼용으로 설정.
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	// 깊이 버퍼 생성.
	//ID3D11Texture2D* pDS = NULL;

	hr = m_pDevice->CreateTexture2D(&td, NULL, &m_pDS);
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"깊이/스텐실 버퍼용 CreateTexture 실패.", L"Error", MB_OK);
		return hr;
	}
	
	// 깊이-스텐실버퍼용 리소스 뷰 정보 설정. 
	D3D11_DEPTH_STENCIL_VIEW_DESC  dd;
	ZeroMemory(&dd, sizeof(dd));
	dd.Format = td.Format;
	//dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; //AA 없음.
	dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS; //AA 적용.
	dd.Texture2D.MipSlice = 0;

	//깊이-스텐실 버퍼 뷰 생성.
	hr = m_pDevice->CreateDepthStencilView(m_pDS, &dd, &m_pDSView);
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"깊이/스텐실뷰 생성 실패.", L"Error", MB_OK);
		return hr;
	}

	//리소스 뷰 생성 후, 불필요한 DX 핸들은 해제해야 합니다.(메모리 누수 방지)
	//SAFE_RELEASE(pDS);

	return hr;
}

void C3DDevice::_SetViewPort()
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (FLOAT)g_Mode.Width;
	vp.Height = (FLOAT)g_Mode.Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pDXDC->RSSetViewports(1, &vp);
}

// 깊이/스텐실, 레스터, 셈플러, 블렌드 상태 객체 생성
int C3DDevice::_RenderStateObjectCreate()
{

	//깊이/스텐실 상태 객체 생성.
	_DepthStencilStateCreate();

	//레스터 상태 개체 생성.
	_RasterStateCreate();

	//셈플러 상태 개체 생성.
	_SamplerCreate();

	//블렌드 상태 객체 생성.
	_BlendStateCreate();


	return S_OK;
}

void C3DDevice::_RenderStateObjectRelease()
{
	_DepthStencilStateRelease();		//깊이/스텐실 상태 객체 제거.
	_RasterStateRelease();			//레스터 상태 객체 제거.
	_SamplerRelease();				//셈플러 상태 객체 제거.
	_BlendStateRelease();			//블렌드 상태 객체 제거.
}

void C3DDevice::RenderStateUpdate()
{
	// 렌더링 옵션 조절 	 
	if (IsKeyUp(VK_SPACE))	m_bWireFrame ^= TRUE;
	if (IsKeyUp(VK_F4))	m_bCullBack ^= TRUE;

	//if (IsKeyUp(VK_F5))		g_bDiffOn ^= TRUE;		
	//if (IsKeyUp(VK_F6))		g_bSpecOn ^= TRUE;		
	//if (IsKeyUp(VK_F5))		g_bZEnable ^= TRUE;		
	//if (IsKeyUp(VK_F6))		m_bZWrite ^= TRUE;		

	// 배경색 설정.
	if (m_bWireFrame) CLEAR_COLOR_DBLUE;
	else			  CLEAR_COLOR_GRAY;

	// 렌더링 모드 전환.	  
	_RenderModeUpdate();

	// 깊이 연산 모드 전환.
	//if (m_bZEnable)
	//{
	//	if (m_bZWrite)
	//		  m_pDXDC->OMSetDepthStencilState(m_DSState[DS_DEPTH_ON], 0);			//깊이 버퍼 동작 (기본값)
	//	else  m_pDXDC->OMSetDepthStencilState(m_DSState[DS_DEPTH_WRITE_OFF], 0);	//깊이 버퍼  : Z-Test On + Z-Write Off.
	//}
	//else  m_pDXDC->OMSetDepthStencilState(m_DSState[DS_DEPTH_OFF], 0);	//깊이 버퍼 비활성화 : Z-Test Off + Z-Write Off.
	 

	 //깊이 버퍼 동작 (기본값)
	m_pDXDC->OMSetDepthStencilState(m_DSState[DS_DEPTH_ON], 1);

	//m_pDXDC->OMSetBlendState(m_BState[BS_ALPHA_BLEND], NULL, 0xFFFFFFFF);

	//셈플러 설정.(기본설정)
	m_pDXDC->PSSetSamplers(0, 1, &m_pSampler[SS_DEFAULT]);
}


int C3DDevice::_DepthStencilStateCreate()
{
	//----------------------------
	// 깊이/스텐실 상태 개체 생성.: "출력병합기 Output Merger" 상태 조절. 
	//----------------------------
	//...	 
	D3D11_DEPTH_STENCIL_DESC  ds;

	//깊이 버퍼 설정 (기본값)
	ds.DepthEnable = TRUE;
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds.DepthFunc = D3D11_COMPARISON_LESS;
	
	//스텐실 버퍼 설정 (기본값) 
	ds.StencilEnable = FALSE;								//스텐실 버퍼 OFF.
	ds.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;	//스텐실 읽기 마스크 (8bit: 0xff)
	ds.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK; //스텐실 쓰기 마스크 (8bit: 0xff)
	
	ds.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;	//[앞면] 스텐실 비교 함수 : "Always" 즉, 항상 성공 (통과, pass)
	ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;	//[앞면] 스텐실 비교 성공시 동작 : 기존값 유지.
	ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;	//[앞면] 스텐실 비교 실패시 동작 : 기존값 유지.	
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	//[앞면] 스텐실/깊이 비교 실패시 동작 : 기존값 유지.
	
	ds.BackFace = ds.FrontFace;									//[뒷면] 설정 동일. 필요시 개별 설정이 가능.


	//----------------------------------------------------------------------
	// 깊이 버퍼 연산 객체들.
	//----------------------------------------------------------------------
	// DS 상태 객체 #0 : Z-Test ON! (기본값)
	ds.DepthEnable = TRUE;					//깊이 버퍼 On.
	ds.StencilEnable = FALSE;				//스텐실 버퍼 Off!
	m_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_ON]);

	// DS 상태 객체 #1 : Z-Test OFF 상태.
	ds.DepthEnable = FALSE;
	m_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_OFF]);

	// DS 상태 객체 #2 : Z-Test On + Z-Write OFF.
	// Z-Test (ZEnable, DepthEnable) 이 꺼지면, Z-Write 역시 비활성화 됩니다.
	ds.DepthEnable = TRUE;
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;		//깊이값 쓰기 끔.
	m_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_WRITE_OFF]);


	//----------------------------------------------------------------------
	// 스텐실 버퍼 연산 객체들 생성.
	//----------------------------------------------------------------------
	// 스텐실 버퍼 비트 연산 공식.
	// (Stencil.Ref & Stencil.Mask) Comparison-Func ( StencilBuffer.Value & Stencil.Mask)
	//
	// *StencilBufferValue : 현재 검사할 픽셀의 스텐실값.
	// *ComFunc : 비교 함수. ( > < >= <= ==  Always Never)
	//----------------------------------------------------------------------
	// DS 상태객체 #4 :  깊이버퍼 On, 스텐실버퍼 ON (항상, 참조값 쓰기) : "깊이/스텐실 기록" 
	ds.DepthEnable = TRUE;										//깊이버퍼 ON! (기본값)
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds.DepthFunc = D3D11_COMPARISON_LESS;
	ds.StencilEnable = TRUE;										//스텐실 버퍼 ON! 
	ds.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;		//비교함수 : "항상 통과" (성공)
	ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;		//성공시 : 참조값(Stencil Reference Value) 로 교체.
	//ds.FrontFace.StencilFailOp	  = D3D11_STENCIL_OP_KEEP;		//실패시 : 유지.(기본값, 생략)
	//ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		//실패시 : 유지.(기본값, 생략)
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	m_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_ON_STENCIL_ON]);


	// DS 상태객체 #5 : 깊이버퍼 On, 스텐실버퍼 ON (동일비교, 성공시 유지) : "지정 위치에만 그리기" 
	//ds.DepthEnable	= TRUE;										//깊이버퍼 ON! (기본값)(생략)
	ds.StencilEnable = TRUE;										//스텐실 버퍼 ON! 
	ds.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		//비교함수 : "동일한가?" 
	ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		//성공시 : 유지.
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	m_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_ON_STENCIL_EQUAL_KEEP]);


	// DS 상태객체 #6 : 깊이버퍼 On, 스텐실버퍼 ON (다름비교, 성공시 유지) : "지정 위치 이외에 그리기" 
	//ds.DepthEnable	= TRUE;										//깊이버퍼 ON! (기본값)(생략)
	ds.StencilEnable = TRUE;										//스텐실 버퍼 ON!
	ds.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;	//비교함수 : "같이 않은가?" 
	ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		//성공시 : 유지.
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	m_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_ON_STENCIL_NOTEQUAL_KEEP]);


	/*// DS 상태객체 #7 : 깊이버퍼 Off, 스텐실버퍼 ON (참조값 쓰기) : "스텐실만 기록"
	ds.DepthEnable	  = FALSE;										//깊이버퍼 OFF!
	ds.StencilEnable = TRUE;										//스텐실 버퍼 ON!
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;		//비교함수 : "항상 통과" (성공)
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;		//성공시 : 참조값(Stencil Reference Value) 로 교체.
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	g_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_OFF_STENCIL_ON]);
	*/

	/*// DS 상태객체 #8 : 깊이버퍼 On, 스텐실버퍼 ON (동일비교, 성시 증가) : "이중그리기 방지용"
	//ds.DepthEnable	= TRUE;										//깊이버퍼 ON! (기본값)(생략)
	//ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//ds.DepthFunc		= D3D11_COMPARISON_LESS;
	ds.StencilEnable = TRUE;										//스텐실 버퍼 ON!
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_EQUAL;		//비교함수 : "동일한가?"
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_INCR;		//성공시 : 증가 (+1)
	ds.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;		//실패시 : 유지.
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		//실패시 : 유지.
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	g_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_ON_STENCIL_EQUAL_INCR]);
	*/

	// DS 상태객체 #9 : 깊이버퍼 On, 스텐실버퍼 ON (항상, 성공시 증가) : "스텐실만 기록" 
	ds.DepthEnable = TRUE;										//깊이버퍼 ON! (기본값)
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;				//깊이버퍼 쓰기 OFF.
	ds.DepthFunc = D3D11_COMPARISON_LESS;						//깊이연산 ON. (기본값)
	ds.StencilEnable = TRUE;										//스텐실 버퍼 ON! 
	ds.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;		//비교함수 : "항상 통과" (성공)
	ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;		//성공시 : 참조값(Stencil Reference Value) 로 교체.
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	m_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_WRITE_OFF_STENCIL_ON]);


	//깊이버퍼 Off (Write Off), 스텐실버퍼 ON (동일비교, 성공시 증가) : "이중그리기 방지.
	ds.DepthEnable = FALSE;										//깊이테스트 Off!
	//ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;				//깊이값 쓰기 Off.
	//ds.DepthFunc		= D3D11_COMPARISON_LESS;
	ds.StencilEnable = TRUE;										//스텐실 버퍼 ON! 
	ds.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		//비교함수 : "동일한가?" 
	ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;		//성공시 : 증가 (+1) 
	ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;		//실패시 : 유지.
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		//실패시 : 유지.
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	m_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_OFF_STENCIL_EQUAL_INCR]);


	return S_OK;
}

void C3DDevice::_DepthStencilStateRelease()
{
	for (int i = 0; i < DS_MAX_; i++)
		SAFE_RELEASE(m_DSState[i]);
}

void C3DDevice::_RasterStateCreate()
{
	//[상태객체 1] 기본 렌더링 상태 개체.
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID;		//삼각형 색상 채우기.(기본값)
	rd.CullMode = D3D11_CULL_NONE;		//컬링 없음. (기본값은 컬링 Back)		
	rd.FrontCounterClockwise = false;   //이하 기본값...
	rd.DepthBias = 0;				//D3D11_DEFAULT_DEPTH_BIAS
	rd.DepthBiasClamp = 0;			//D3D11_DEFAULT_DEPTH_BIAS_CLAMP
	rd.SlopeScaledDepthBias = 0;	//D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = true;		//AA 적용.
	rd.AntialiasedLineEnable = false;	//라인 AA 적용
	//레스터라이져 상태 객체 생성.
	m_pDevice->CreateRasterizerState(&rd, &m_RState[RS_SOLID]);


	//[상태객체2] 와이어 프레임 그리기. 
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_NONE;
	//레스터라이져 객체 생성.
	m_pDevice->CreateRasterizerState(&rd, &m_RState[RS_WIREFRM]);

	//[상태객체3] 후면 컬링 On! "CCW"
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	m_pDevice->CreateRasterizerState(&rd, &m_RState[RS_CULLBACK]);

	//[상태객체4] 와이어 프레임 + 후면컬링 On! "CCW"
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_BACK;
	m_pDevice->CreateRasterizerState(&rd, &m_RState[RS_WIRECULLBACK]);

	//[상태객체5] 정면 컬링 On! "CW"
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_FRONT;
	m_pDevice->CreateRasterizerState(&rd, &m_RState[RS_CULLFRONT]);

	//[상태객체6] 와이어 프레임 + 정면컬링 On! "CW" 
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_FRONT;
	m_pDevice->CreateRasterizerState(&rd, &m_RState[RS_WIRECULLFRONT]);

	
		
}

void C3DDevice::_RasterStateRelease()
{
	for (int i = 0; i < RS_MAX_; i++)
	{
		SAFE_RELEASE(m_RState[i]);
	}
}

#define CheckRMode(k, v) if((k)) m_RMode |= (v); else m_RMode &= ~(v);

void C3DDevice::_RenderModeUpdate()
{
	// 렌더링 모드 체크 : 사용자가 지정한 렌더링 상태 조합.
	CheckRMode(m_bCullBack, RM_CULLBACK);
	CheckRMode(m_bWireFrame, RM_WIREFRAME);


	// 레스터 모드 전환 : 지정된 모드의 조합을 통해 렌더링 상태를 조절.
	switch (m_RMode)
	{
	default:
	case RM_SOLID:
		m_pDXDC->RSSetState(m_RState[RS_SOLID]);
		break;
	case RM_WIREFRAME:
		m_pDXDC->RSSetState(m_RState[RS_WIREFRM]);
		break;
	case RM_CULLBACK:
		m_pDXDC->RSSetState(m_RState[RS_CULLBACK]);
		break;
	case RM_WIREFRAME | RM_CULLBACK:
		m_pDXDC->RSSetState(m_RState[RS_WIRECULLBACK]);
		break;
	}
}

void C3DDevice::_BlendStateCreate()
{
	// RT #0 (기본)백버퍼 만을 사용

	//------------------------------
	// 혼합객체1 : 색상혼합 없음. "Blending OFF"
	//------------------------------
	//블렌딩 상태 객체 구성 옵션 : 이하 기본값들.
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.RenderTarget[0].BlendEnable = FALSE;							//블렌딩 동작 결정. 기본값은 FALSE(OFF)		
	//색상 성분 혼합 : Color Blending.(기본값)
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//색상 혼합 연산(Color - Operation), 기본값은 덧셈 : 최종색 = Src.Color + Dest.Color 	
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;					//소스(원본) 혼합 비율, 원본 100% : Src.Color = Src * 1;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;				//타겟(대상) 혼합 비율, 대상 0%   : Dest.Color = Dest * 0;  통상 0번 RT 는 "백버퍼"를 의미합니다.	
	//알파 성분 혼합 : Alpha Blending.(기본값)
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;			//알파 혼합 함수(Alpha - Opertion), 기본값은 덧셈.
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;				//소스(원본) 알파 혼합 비율.
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;			//타겟(대상) 알파 혼합 비율.
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	//렌더타겟에 쓰기 옵션.
	//bd.AlphaToCoverageEnable = FALSE;								//이하 추가 옵션.(기본값, 생략)
	//bd.IndependentBlendEnable = FALSE;
	m_pDevice->CreateBlendState(&bd, &m_BState[BS_DEFAULT]);		//상태 개체.생성.


	//------------------------------
	// 혼합객체2 : 알파 혼합 "Alpha Blending"
	//------------------------------
	bd.RenderTarget[0].BlendEnable = TRUE;							//색상 혼합 ON! 	
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//색상 혼합 연산 (덧셈, 기본값) : 최종색 = Src.Color + Dest.Color 		
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			//소스(원본) 혼합 비율, 원본 알파비율  : Src.Color = Src * Src.a;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;		//타겟(대상) 혼합 비율, 원본 알파비율 반전 : Dest.Color = Dest * (1-Src.a);			 		
	m_pDevice->CreateBlendState(&bd, &m_BState[BS_ALPHA_BLEND]);


	//------------------------------
	// 혼합객체3 : 색상 혼합 "Color Blending"
	//------------------------------
	bd.RenderTarget[0].BlendEnable = TRUE;							//색상 혼합 ON! 	
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//색상 혼합 연산 (덧셈, 기본값) : 최종색 = Src.Color + Dest.Color 		
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;			//소스(원본) 혼합 비율, 원본 색상  : Src.Color = Src * Src.Color;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;		//타겟(대상) 혼합 비율, 원본 색상 반전 : Dest.Color = Dest * (1-Src.Color);			 	
	m_pDevice->CreateBlendState(&bd, &m_BState[BS_COLOR_BLEND]);


	//------------------------------
	// 혼합객체3 : 색상 혼합 "밝기 강조 Blending 1:1" 
	//------------------------------
	bd.RenderTarget[0].BlendEnable = TRUE;							//색상 혼합 ON! 	
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//색상 혼합 연산 "밝기강조" : 최종색 = Src.Color + Dest.Color 		
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;					//소스(원본) 혼합 비율, 원본 색상  : Src.Color = Src * 1;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;					//타겟(대상) 혼합 비율, 원본 색상 반전 : Dest.Color = Dest * 1;			 	
	m_pDevice->CreateBlendState(&bd, &m_BState[BS_COLOR_BLEND_ONE]);

}

void C3DDevice::_BlendStateRelease()
{
	for (int i = 0; i < BS_MAX_; i++)
	{
		SAFE_RELEASE(m_BState[i]);
	}
}

void C3DDevice::_SamplerCreate()
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));

	
	
	//텍스처 필터 : 비등방 필터링 AF
	//sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; //필터 OFF
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = m_dwAF;					//AF 샘플링 배수 옵션
	//밉멥핑	
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	//이하 기본값 처리..
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.BorderColor[0] = 1;
	sd.BorderColor[1] = 1;
	sd.BorderColor[2] = 1;
	sd.BorderColor[3] = 1;

	//샘플러 객체1 생성. (DX 기본값)
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	hr = m_pDevice->CreateSamplerState(&sd, &m_pSampler[SS_CLAMP]);

	//샘플러 객체2 생성.
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = m_pDevice->CreateSamplerState(&sd, &m_pSampler[SS_WRAP]);

	//샘플러 객체3 생성. (검정색)
	sd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.BorderColor[0] = 0;
	sd.BorderColor[1] = 0;
	sd.BorderColor[2] = 0;
	sd.BorderColor[3] = 1;
	sd.MaxLOD = 0;
	hr = m_pDevice->CreateSamplerState(&sd, &m_pSampler[SS_BORDER]);
}

void C3DDevice::_SamplerRelease()
{
	for (int i = 0; i < SS_MAX; i++)
		SAFE_RELEASE(m_pSampler[i]);
}
