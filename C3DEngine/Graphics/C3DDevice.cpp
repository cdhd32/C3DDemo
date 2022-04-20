#pragma once
#include "../pch.h"
// DXTK, DXTex ����
#include "C3DDevice.h"

//��ġ ���� �⺻ ����. DXGI_MODE_DESC
//DXGI_MODE_DESC g_Mode = { 1200, 800, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };
//DXGI_MODE_DESC g_Mode = { 800, 600, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };
//DXGI_MODE_DESC g_Mode = { 960, 600, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//16:10
//DXGI_MODE_DESC g_Mode = { 1200, 600, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//2:1
//DXGI_MODE_DESC g_Mode = { 1280, 720, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//16:9
DXGI_MODE_DESC g_Mode = { 1920, 1080, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//16:9 FHD
//DXGI_MODE_DESC g_Mode = { 2560, 1440, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//16:9 QHD
//DXGI_MODE_DESC g_Mode = { 3440, 1440, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//21:9 4K WQHD
//DXGI_MODE_DESC g_Mode = { 3840, 2160, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//16:9 4K UHD

// ����
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
	//DX �¾�
	_DirectXSetup(hWnd);

	// sys �¾� : ��Ÿ �ý��� ���� ��ü ���� 
	// ���� �⺻ ī�޶�, ����, ��Ʈ, ������, �׸���, ���..
	//SysSetup(g_pDevice, g_Mode, 5.0f);

	return S_OK;
}

void C3DDevice::Release()
{
	//DX ����
	_DirectXRelease();
}

HRESULT C3DDevice::CreateVB(void* pBuff, UINT size, ID3D11Buffer** ppVB)
{
	ID3D11Buffer* pVB = nullptr;
	HRESULT hr = S_OK;

	//���� ���� ���� ����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				//���� �����
	bd.ByteWidth = size;						//���� ũ��
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//���� �뵵 : "���� ����" ��� ���� 
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = pBuff;					//���ۿ� �� ������ ���� : "������"..

	//���� ���� ����.
	hr = m_pDevice->CreateBuffer(&bd, &rd, &pVB);
	if (FAILED(hr))
	{
		//���� ���� �� ����ó��..
		MessageBox(NULL, L"Failed, CreateVB on Model", L"Error_Model", MB_OK);
		return hr;
	}
		
	//�ܺη� ����.
	*ppVB = pVB;

	return hr;
}

HRESULT C3DDevice::CreateIB(void* pBuff, UINT size, ID3D11Buffer** ppIB)
{
	ID3D11Buffer* pIB = nullptr;

	// �ε��� ���� ���� ����.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;					//���� �����
	bd.ByteWidth = size;							//���� ũ��
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;			//���� �뵵 : "���� ����" ��� ���� 
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = pBuff;								//���ۿ� �� ������ ���� : "���� ����"..

	//���� ���� ����.
	HRESULT hr = m_pDevice->CreateBuffer(&bd, &rd, &pIB);
	if (FAILED(hr))
	{
		//���� ���� �� ����ó��..
		//...
		MessageBox(NULL, L"Failed, CreateIB on Model", L"Error_Model", MB_OK);
		return hr;
	}

	//�ܺη� ����.
	*ppIB = pIB;

	return hr;
}

void C3DDevice::RTClear(Vector4 col)
{
	//[����] ����Ÿ�ٰ� ����Ʈ ����.
	// DX9 �� �ٸ��� DX10/11 ������ ClearRenderTarget �޼ҵ��
	// ����Ÿ�ٺ� ��ü�� ����� Viewport �� Scissor ������ ������� �ʽ��ϴ�.
	// ...
	// �ذ�1 : DX11.1 / ID3D11DeviceContext1::ClearView �޼ҵ� ���.
	// �ذ�2 : "Drawing Quad" , ��������/�簢�� �޽��� ���� ����� 

	//����Ÿ��(�����)/ ���̹��� �� ���ٽǹ��� �����..
	_ClearBackBuffer(D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,	//�ʱ�ȭ �÷���.
		col,			//����.
		1.0f,			//���� ���� �ʱⰪ.
		0				//���ٽ� ���� �ʱⰪ.
	);
}

// ��� �׸��� ����.
void C3DDevice::Flip()
{
	m_pSwapChain->Present(m_bVSync, 0);			//ȭ����� : Flip! (+��������ȭ)
}

HRESULT C3DDevice::_DirectXSetup(HWND hWnd)
{
	// D3D ������ ��ġ Device �� ����ü�� Swap Chain ����. 
	_CreateDeviceSwapChain(hWnd);

	// ��ġ-����ü���� ����Ÿ��(�����) ȹ��
	_CreateRenderTarget();

	// ����/���ٽ� ���� ����.
	_CreateDepthStencil();

	// ��ġ ��º��ձ�(Output Merger) �� ���͸� Ÿ�� �� ����-���ٽ� ���� ���.
	m_pDXDC->OMSetRenderTargets(
		1,				// ����Ÿ�� ����.(max: D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
		&m_pRTView,		// ����Ÿ��("�����") ���.	
		m_pDSView		// ����/���ٽ� ���� ���.
	);

	// ����Ʈ ����
	_SetViewPort();

	//��ġ ���� ȹ��
	//GetDeviceInfo();



	// ��Ÿ ������ ���� �߰� ����.
	// �������� �ʿ��� ����� ��ü���� ����/���� �մϴ�.
	// ī�޶�, ����, ��Ʈ, ���̴� ���...

	//������ ���� ��ü ����.
	_RenderStateObjectCreate();

	// �⺻ (����) ���̴� ����.
	//ShaderSetup();

	// Input? �¾�.           
	//----------------------------------------
	/*if (!ynInput_Initial())
	{
		return YN_FAIL;
	}*/

	//��ü���� ������ȯ.
	//g_pSwapChain->SetFullscreenState(TRUE, NULL);

	return S_OK;
}

void C3DDevice::_DirectXRelease()
{
	//��ġ ���� ���� : ���� ���� �ʱ�ȭ�� �ؾ� �մϴ�. (�޸� ���� ����)
	if (m_pDXDC) m_pDXDC->ClearState();

	//���� ��ü ����.
	_RenderStateObjectRelease();

	//�⺻ ���̴� ����.
	//ShaderRelease();

	SAFE_RELEASE(m_pDS);				//����/���ٽ� ���� ����.
	SAFE_RELEASE(m_pDSView);
	SAFE_RELEASE(m_pRTView);			//����Ÿ�� ����.
	SAFE_RELEASE(m_pSwapChain);			//����ü�� ����.
	SAFE_RELEASE(m_pDXDC);
	SAFE_RELEASE(m_pDevice);			//����̽� ����. �� ���߿� �����մϴ�.
}

int C3DDevice::_ClearBackBuffer(UINT flag, XMFLOAT4 col, float depth /*= 1.0f*/, UINT stencil /*= 0*/)
{
	m_pDXDC->ClearRenderTargetView(m_pRTView, (float*)&col);			//����Ÿ�� �����.
	m_pDXDC->ClearDepthStencilView(m_pDSView, flag, depth, stencil);	//����/���ٽ� �����.

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

	// ��ġ (Device) �� ����ü��(SwapChain) ���� ����.
	// ����ü���� ���߹��۸� �ý����� ���ϸ�
	// �������� '�ø���Flipping' ü�ΰ� ������ �ǹ��Դϴ�.  
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Windowed = m_bWindowMode;				//Ǯ��ũ�� �Ǵ� â��� ����.
	sd.OutputWindow = hwnd;						//����� ������ �ڵ�.
	sd.BufferCount = 1;							//����� ����.
	sd.BufferDesc.Width = g_Mode.Width;			//�ػ� ����.(����� ũ��)
	sd.BufferDesc.Height = g_Mode.Height;
	sd.BufferDesc.Format = g_Mode.Format;		//����� ����԰� (A8R8G8B8) â��忡���� ���� ���� 
	sd.BufferDesc.RefreshRate.Numerator = m_bVSync ? 60 : 0;   //���� ������.(��������ȭ VSync Ȱ��ȭ�� ǥ�ذ����� ���� : 60hz)
	//sd.BufferDesc.RefreshRate.Numerator = 0;	//���� ������.(��������ȭ VSync Off)
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�뵵 ����: '����Ÿ��' 
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//sd.Flags = 0;
	sd.SampleDesc.Count = m_dwAA;					//AA ����
	sd.SampleDesc.Quality = 0;



	// D3D ������ '��ġ(Device)' �� ����ü�� ����. 
	//
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,						//���� ��� ������ (�⺻��ġ�� NULL)
		D3D_DRIVER_TYPE_HARDWARE,	//HW ���� 
		NULL,						//SW Rasterizer DLL �ڵ�.  HW ���ӽÿ��� NULL.
		0,							//����̽� ���� �÷���.(�⺻��)
		//D3D11_CREATE_DEVICE_DEBUG,//����̽� ���� �÷���.(�����)
		&m_FeatureLevels,			//(������) ����̽� ��� ����(Feature Level) �迭
		1,							//(������) ����̽� ��� ����(Feature Level) �迭 ũ��.
		D3D11_SDK_VERSION,			//DX SDK ����.
		&sd,						//����̽� ���� �� �ɼ�.
		&m_pSwapChain,				//[���] ����ü�� �������̽� ���.
		&m_pDevice,					//[���] ����̽� �������̽� ���.
		NULL,						//[���] (������) ����̽� ��� ����. �ʿ���ٸ� NULL ����. 
		&m_pDXDC					//[���] ����̽� ���ؽ�Ʈ ���.
	);

	if (FAILED(hr))
	{
		//hr ������ ���� ��� �����ϰ�...#����
		MessageBox(m_hWnd, L"����̽� / ����ü�� ���� ����.", L"Error", MB_OK);
	}

	return hr;
}

HRESULT C3DDevice::_CreateRenderTarget()
{
	HRESULT hr = S_OK;

	// ����� ȹ��.
	ID3D11Texture2D* pBackBuffer;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);		// ����� ���, 2D �ؽ��� ����, ȭ����¿�
	if (FAILED(hr))	return hr;

	//ȹ���� ����ۿ� ����Ÿ�� �� ����.(����Ÿ��'��'���� ������)
	hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRTView);		// '����Ÿ�ٺ�'�� ����.	
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"�����-����Ÿ�ٺ� ���� ����.", L"Error", MB_OK);
	}

	//���ҽ� �� ���� ��, ���ʿ��� DX �ڵ��� �����ؾ� �մϴ�.(�޸� ���� ����)
	SAFE_RELEASE(pBackBuffer);
	
	return hr;
}

HRESULT C3DDevice::_CreateDepthStencil()
{
	HRESULT hr = S_OK;

	//����/���ٽ� ���ۿ� ���� ����.
	D3D11_TEXTURE2D_DESC   td;
	ZeroMemory(&td, sizeof(td));
	td.Width = g_Mode.Width;
	td.Height = g_Mode.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // DXGI_FORMAT_D32_FLOAT;			// 32BIT. ���� ����.
	td.SampleDesc.Count = m_dwAA;					// AA ���� - RT �� ���� �԰� �ؼ�.
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;	// ����-���ٽ� ���ۿ����� ����.
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	// ���� ���� ����.
	//ID3D11Texture2D* pDS = NULL;

	hr = m_pDevice->CreateTexture2D(&td, NULL, &m_pDS);
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"����/���ٽ� ���ۿ� CreateTexture ����.", L"Error", MB_OK);
		return hr;
	}
	
	// ����-���ٽǹ��ۿ� ���ҽ� �� ���� ����. 
	D3D11_DEPTH_STENCIL_VIEW_DESC  dd;
	ZeroMemory(&dd, sizeof(dd));
	dd.Format = td.Format;
	//dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; //AA ����.
	dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS; //AA ����.
	dd.Texture2D.MipSlice = 0;

	//����-���ٽ� ���� �� ����.
	hr = m_pDevice->CreateDepthStencilView(m_pDS, &dd, &m_pDSView);
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"����/���ٽǺ� ���� ����.", L"Error", MB_OK);
		return hr;
	}

	//���ҽ� �� ���� ��, ���ʿ��� DX �ڵ��� �����ؾ� �մϴ�.(�޸� ���� ����)
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

// ����/���ٽ�, ������, ���÷�, ���� ���� ��ü ����
int C3DDevice::_RenderStateObjectCreate()
{

	//����/���ٽ� ���� ��ü ����.
	_DepthStencilStateCreate();

	//������ ���� ��ü ����.
	_RasterStateCreate();

	//���÷� ���� ��ü ����.
	_SamplerCreate();

	//���� ���� ��ü ����.
	_BlendStateCreate();


	return S_OK;
}

void C3DDevice::_RenderStateObjectRelease()
{
	_DepthStencilStateRelease();		//����/���ٽ� ���� ��ü ����.
	_RasterStateRelease();			//������ ���� ��ü ����.
	_SamplerRelease();				//���÷� ���� ��ü ����.
	_BlendStateRelease();			//���� ���� ��ü ����.
}

void C3DDevice::RenderStateUpdate()
{
	// ������ �ɼ� ���� 	 
	if (IsKeyUp(VK_SPACE))	m_bWireFrame ^= TRUE;
	if (IsKeyUp(VK_F4))	m_bCullBack ^= TRUE;

	//if (IsKeyUp(VK_F5))		g_bDiffOn ^= TRUE;		
	//if (IsKeyUp(VK_F6))		g_bSpecOn ^= TRUE;		
	//if (IsKeyUp(VK_F5))		g_bZEnable ^= TRUE;		
	//if (IsKeyUp(VK_F6))		m_bZWrite ^= TRUE;		

	// ���� ����.
	if (m_bWireFrame) CLEAR_COLOR_DBLUE;
	else			  CLEAR_COLOR_GRAY;

	// ������ ��� ��ȯ.	  
	_RenderModeUpdate();

	// ���� ���� ��� ��ȯ.
	//if (m_bZEnable)
	//{
	//	if (m_bZWrite)
	//		  m_pDXDC->OMSetDepthStencilState(m_DSState[DS_DEPTH_ON], 0);			//���� ���� ���� (�⺻��)
	//	else  m_pDXDC->OMSetDepthStencilState(m_DSState[DS_DEPTH_WRITE_OFF], 0);	//���� ����  : Z-Test On + Z-Write Off.
	//}
	//else  m_pDXDC->OMSetDepthStencilState(m_DSState[DS_DEPTH_OFF], 0);	//���� ���� ��Ȱ��ȭ : Z-Test Off + Z-Write Off.
	 

	 //���� ���� ���� (�⺻��)
	m_pDXDC->OMSetDepthStencilState(m_DSState[DS_DEPTH_ON], 1);

	//m_pDXDC->OMSetBlendState(m_BState[BS_ALPHA_BLEND], NULL, 0xFFFFFFFF);

	//���÷� ����.(�⺻����)
	m_pDXDC->PSSetSamplers(0, 1, &m_pSampler[SS_DEFAULT]);
}


int C3DDevice::_DepthStencilStateCreate()
{
	//----------------------------
	// ����/���ٽ� ���� ��ü ����.: "��º��ձ� Output Merger" ���� ����. 
	//----------------------------
	//...	 
	D3D11_DEPTH_STENCIL_DESC  ds;

	//���� ���� ���� (�⺻��)
	ds.DepthEnable = TRUE;
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds.DepthFunc = D3D11_COMPARISON_LESS;
	
	//���ٽ� ���� ���� (�⺻��) 
	ds.StencilEnable = FALSE;								//���ٽ� ���� OFF.
	ds.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;	//���ٽ� �б� ����ũ (8bit: 0xff)
	ds.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK; //���ٽ� ���� ����ũ (8bit: 0xff)
	
	ds.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;	//[�ո�] ���ٽ� �� �Լ� : "Always" ��, �׻� ���� (���, pass)
	ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;	//[�ո�] ���ٽ� �� ������ ���� : ������ ����.
	ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;	//[�ո�] ���ٽ� �� ���н� ���� : ������ ����.	
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	//[�ո�] ���ٽ�/���� �� ���н� ���� : ������ ����.
	
	ds.BackFace = ds.FrontFace;									//[�޸�] ���� ����. �ʿ�� ���� ������ ����.


	//----------------------------------------------------------------------
	// ���� ���� ���� ��ü��.
	//----------------------------------------------------------------------
	// DS ���� ��ü #0 : Z-Test ON! (�⺻��)
	ds.DepthEnable = TRUE;					//���� ���� On.
	ds.StencilEnable = FALSE;				//���ٽ� ���� Off!
	m_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_ON]);

	// DS ���� ��ü #1 : Z-Test OFF ����.
	ds.DepthEnable = FALSE;
	m_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_OFF]);

	// DS ���� ��ü #2 : Z-Test On + Z-Write OFF.
	// Z-Test (ZEnable, DepthEnable) �� ������, Z-Write ���� ��Ȱ��ȭ �˴ϴ�.
	ds.DepthEnable = TRUE;
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;		//���̰� ���� ��.
	m_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_WRITE_OFF]);


	//----------------------------------------------------------------------
	// ���ٽ� ���� ���� ��ü�� ����.
	//----------------------------------------------------------------------
	// ���ٽ� ���� ��Ʈ ���� ����.
	// (Stencil.Ref & Stencil.Mask) Comparison-Func ( StencilBuffer.Value & Stencil.Mask)
	//
	// *StencilBufferValue : ���� �˻��� �ȼ��� ���ٽǰ�.
	// *ComFunc : �� �Լ�. ( > < >= <= ==  Always Never)
	//----------------------------------------------------------------------
	// DS ���°�ü #4 :  ���̹��� On, ���ٽǹ��� ON (�׻�, ������ ����) : "����/���ٽ� ���" 
	ds.DepthEnable = TRUE;										//���̹��� ON! (�⺻��)
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds.DepthFunc = D3D11_COMPARISON_LESS;
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON! 
	ds.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;		//���Լ� : "�׻� ���" (����)
	ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;		//������ : ������(Stencil Reference Value) �� ��ü.
	//ds.FrontFace.StencilFailOp	  = D3D11_STENCIL_OP_KEEP;		//���н� : ����.(�⺻��, ����)
	//ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		//���н� : ����.(�⺻��, ����)
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	m_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_ON_STENCIL_ON]);


	// DS ���°�ü #5 : ���̹��� On, ���ٽǹ��� ON (���Ϻ�, ������ ����) : "���� ��ġ���� �׸���" 
	//ds.DepthEnable	= TRUE;										//���̹��� ON! (�⺻��)(����)
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON! 
	ds.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		//���Լ� : "�����Ѱ�?" 
	ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		//������ : ����.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	m_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_ON_STENCIL_EQUAL_KEEP]);


	// DS ���°�ü #6 : ���̹��� On, ���ٽǹ��� ON (�ٸ���, ������ ����) : "���� ��ġ �̿ܿ� �׸���" 
	//ds.DepthEnable	= TRUE;										//���̹��� ON! (�⺻��)(����)
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON!
	ds.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;	//���Լ� : "���� ������?" 
	ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		//������ : ����.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	m_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_ON_STENCIL_NOTEQUAL_KEEP]);


	/*// DS ���°�ü #7 : ���̹��� Off, ���ٽǹ��� ON (������ ����) : "���ٽǸ� ���"
	ds.DepthEnable	  = FALSE;										//���̹��� OFF!
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON!
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;		//���Լ� : "�׻� ���" (����)
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;		//������ : ������(Stencil Reference Value) �� ��ü.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	g_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_OFF_STENCIL_ON]);
	*/

	/*// DS ���°�ü #8 : ���̹��� On, ���ٽǹ��� ON (���Ϻ�, ���� ����) : "���߱׸��� ������"
	//ds.DepthEnable	= TRUE;										//���̹��� ON! (�⺻��)(����)
	//ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//ds.DepthFunc		= D3D11_COMPARISON_LESS;
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON!
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_EQUAL;		//���Լ� : "�����Ѱ�?"
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_INCR;		//������ : ���� (+1)
	ds.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;		//���н� : ����.
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		//���н� : ����.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	g_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_ON_STENCIL_EQUAL_INCR]);
	*/

	// DS ���°�ü #9 : ���̹��� On, ���ٽǹ��� ON (�׻�, ������ ����) : "���ٽǸ� ���" 
	ds.DepthEnable = TRUE;										//���̹��� ON! (�⺻��)
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;				//���̹��� ���� OFF.
	ds.DepthFunc = D3D11_COMPARISON_LESS;						//���̿��� ON. (�⺻��)
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON! 
	ds.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;		//���Լ� : "�׻� ���" (����)
	ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;		//������ : ������(Stencil Reference Value) �� ��ü.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	m_pDevice->CreateDepthStencilState(&ds, &m_DSState[DS_DEPTH_WRITE_OFF_STENCIL_ON]);


	//���̹��� Off (Write Off), ���ٽǹ��� ON (���Ϻ�, ������ ����) : "���߱׸��� ����.
	ds.DepthEnable = FALSE;										//�����׽�Ʈ Off!
	//ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;				//���̰� ���� Off.
	//ds.DepthFunc		= D3D11_COMPARISON_LESS;
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON! 
	ds.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		//���Լ� : "�����Ѱ�?" 
	ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;		//������ : ���� (+1) 
	ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;		//���н� : ����.
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		//���н� : ����.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
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
	//[���°�ü 1] �⺻ ������ ���� ��ü.
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID;		//�ﰢ�� ���� ä���.(�⺻��)
	rd.CullMode = D3D11_CULL_NONE;		//�ø� ����. (�⺻���� �ø� Back)		
	rd.FrontCounterClockwise = false;   //���� �⺻��...
	rd.DepthBias = 0;				//D3D11_DEFAULT_DEPTH_BIAS
	rd.DepthBiasClamp = 0;			//D3D11_DEFAULT_DEPTH_BIAS_CLAMP
	rd.SlopeScaledDepthBias = 0;	//D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = true;		//AA ����.
	rd.AntialiasedLineEnable = false;	//���� AA ����
	//�����Ͷ����� ���� ��ü ����.
	m_pDevice->CreateRasterizerState(&rd, &m_RState[RS_SOLID]);


	//[���°�ü2] ���̾� ������ �׸���. 
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_NONE;
	//�����Ͷ����� ��ü ����.
	m_pDevice->CreateRasterizerState(&rd, &m_RState[RS_WIREFRM]);

	//[���°�ü3] �ĸ� �ø� On! "CCW"
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	m_pDevice->CreateRasterizerState(&rd, &m_RState[RS_CULLBACK]);

	//[���°�ü4] ���̾� ������ + �ĸ��ø� On! "CCW"
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_BACK;
	m_pDevice->CreateRasterizerState(&rd, &m_RState[RS_WIRECULLBACK]);

	//[���°�ü5] ���� �ø� On! "CW"
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_FRONT;
	m_pDevice->CreateRasterizerState(&rd, &m_RState[RS_CULLFRONT]);

	//[���°�ü6] ���̾� ������ + �����ø� On! "CW" 
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
	// ������ ��� üũ : ����ڰ� ������ ������ ���� ����.
	CheckRMode(m_bCullBack, RM_CULLBACK);
	CheckRMode(m_bWireFrame, RM_WIREFRAME);


	// ������ ��� ��ȯ : ������ ����� ������ ���� ������ ���¸� ����.
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
	// RT #0 (�⺻)����� ���� ���

	//------------------------------
	// ȥ�հ�ü1 : ����ȥ�� ����. "Blending OFF"
	//------------------------------
	//���� ���� ��ü ���� �ɼ� : ���� �⺻����.
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.RenderTarget[0].BlendEnable = FALSE;							//���� ���� ����. �⺻���� FALSE(OFF)		
	//���� ���� ȥ�� : Color Blending.(�⺻��)
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//���� ȥ�� ����(Color - Operation), �⺻���� ���� : ������ = Src.Color + Dest.Color 	
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;					//�ҽ�(����) ȥ�� ����, ���� 100% : Src.Color = Src * 1;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;				//Ÿ��(���) ȥ�� ����, ��� 0%   : Dest.Color = Dest * 0;  ��� 0�� RT �� "�����"�� �ǹ��մϴ�.	
	//���� ���� ȥ�� : Alpha Blending.(�⺻��)
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;			//���� ȥ�� �Լ�(Alpha - Opertion), �⺻���� ����.
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;				//�ҽ�(����) ���� ȥ�� ����.
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;			//Ÿ��(���) ���� ȥ�� ����.
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	//����Ÿ�ٿ� ���� �ɼ�.
	//bd.AlphaToCoverageEnable = FALSE;								//���� �߰� �ɼ�.(�⺻��, ����)
	//bd.IndependentBlendEnable = FALSE;
	m_pDevice->CreateBlendState(&bd, &m_BState[BS_DEFAULT]);		//���� ��ü.����.


	//------------------------------
	// ȥ�հ�ü2 : ���� ȥ�� "Alpha Blending"
	//------------------------------
	bd.RenderTarget[0].BlendEnable = TRUE;							//���� ȥ�� ON! 	
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//���� ȥ�� ���� (����, �⺻��) : ������ = Src.Color + Dest.Color 		
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			//�ҽ�(����) ȥ�� ����, ���� ���ĺ���  : Src.Color = Src * Src.a;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;		//Ÿ��(���) ȥ�� ����, ���� ���ĺ��� ���� : Dest.Color = Dest * (1-Src.a);			 		
	m_pDevice->CreateBlendState(&bd, &m_BState[BS_ALPHA_BLEND]);


	//------------------------------
	// ȥ�հ�ü3 : ���� ȥ�� "Color Blending"
	//------------------------------
	bd.RenderTarget[0].BlendEnable = TRUE;							//���� ȥ�� ON! 	
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//���� ȥ�� ���� (����, �⺻��) : ������ = Src.Color + Dest.Color 		
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;			//�ҽ�(����) ȥ�� ����, ���� ����  : Src.Color = Src * Src.Color;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;		//Ÿ��(���) ȥ�� ����, ���� ���� ���� : Dest.Color = Dest * (1-Src.Color);			 	
	m_pDevice->CreateBlendState(&bd, &m_BState[BS_COLOR_BLEND]);


	//------------------------------
	// ȥ�հ�ü3 : ���� ȥ�� "��� ���� Blending 1:1" 
	//------------------------------
	bd.RenderTarget[0].BlendEnable = TRUE;							//���� ȥ�� ON! 	
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//���� ȥ�� ���� "��Ⱝ��" : ������ = Src.Color + Dest.Color 		
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;					//�ҽ�(����) ȥ�� ����, ���� ����  : Src.Color = Src * 1;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;					//Ÿ��(���) ȥ�� ����, ���� ���� ���� : Dest.Color = Dest * 1;			 	
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

	
	
	//�ؽ�ó ���� : ���� ���͸� AF
	//sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; //���� OFF
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = m_dwAF;					//AF ���ø� ��� �ɼ�
	//�Ӹ���	
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	//���� �⺻�� ó��..
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.BorderColor[0] = 1;
	sd.BorderColor[1] = 1;
	sd.BorderColor[2] = 1;
	sd.BorderColor[3] = 1;

	//���÷� ��ü1 ����. (DX �⺻��)
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	hr = m_pDevice->CreateSamplerState(&sd, &m_pSampler[SS_CLAMP]);

	//���÷� ��ü2 ����.
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = m_pDevice->CreateSamplerState(&sd, &m_pSampler[SS_WRAP]);

	//���÷� ��ü3 ����. (������)
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
