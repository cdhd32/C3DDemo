#pragma once
#include "DirectXDefine.h"
#include "RendererDefine.h"
extern DXGI_MODE_DESC g_Mode;

class C3DDevice
{
public:
	static const Vector4 CLEAR_COLOR_DBLUE;
	static const Vector4 CLEAR_COLOR_GRAY;
	
protected:
	//���°��� ������ ���� ���� ���¸� �����ϴ� Ŭ����/��ü�� ���� #����
	HWND m_hWnd;

	// D3D ���� ��ü �������̽� ������.
	ID3D11Device* m_pDevice;				//DX Device
	ID3D11DeviceContext* m_pDXDC;			//DX DC
	IDXGISwapChain* m_pSwapChain;			//���� ü��
	ID3D11RenderTargetView* m_pRTView;		//���� Ÿ�� ��
	ID3D11Texture2D* m_pDS;					//����-���ٽ� ����.
	ID3D11DepthStencilView* m_pDSView;		//����-���ٽ� �� 

	// ������ ���� ��ü��

	// ����/���ٽ� ���� ��ü
	ID3D11DepthStencilState* m_DSState[DS_MAX_] = { nullptr, };
	// �����Ͷ����� ���� ��ü
	ID3D11RasterizerState* m_RState[RS_MAX_] = { nullptr, };
	// Blending State
	ID3D11BlendState* m_BState[BS_MAX_] = { nullptr, };
	// �ؽ�ó ���÷� ��ü
	ID3D11SamplerState* m_pSampler[SS_MAX] = { nullptr, };

	//���� ������ ���.
	DWORD m_RMode;

	// ������ ���� ��Ʈ�� ���� Update �ʿ���..
	BOOL m_bCullBack;		//�޸� ����...On/Off.
	BOOL m_bWireFrame;		//���̾� ������ On/Off.
	BOOL m_bZEnable;			//���� ���� ���� On/Off.
	BOOL m_bZWrite;			//���̹��� ���� On/Off.

	//��üȭ�� ��뿩��.
	BOOL m_bWindowMode = TRUE;

	//��������ȭ ��뿩��.
	BOOL m_bVSync = FALSE;

	//AA & AF Option.
	DWORD		m_dwAA = 8;		//AA ��� (�ִ� 8), AA=1 (OFF)
	DWORD		m_dwAF = 16;	//Anisotropic Filter ���.(�ִ� 16)
	BOOL		m_bMipMap = TRUE;

	//D3D ��� ���� (Direct3D feature level) 
	//���� ����� DX ���� ����. DX ������ ��ġ�� ȣȯ�� ���
	D3D_FEATURE_LEVEL m_FeatureLevels = D3D_FEATURE_LEVEL_11_0;		//DX11 ����.

public:
	C3DDevice();
	~C3DDevice();

	HRESULT Initialize(HWND hWnd);
	void	Release();

	//���Ϲ��� ����. LPDEVICE
	HRESULT CreateVB(void* pBuff, UINT size, ID3D11Buffer** ppVB);
	HRESULT CreateIB(void* pBuff, UINT size, ID3D11Buffer** ppIB);

	// ����Ÿ��, ����/���ٽ� ���� Ŭ����.
	void RTClear(Vector4 col);
	void Flip();

	void RenderStateUpdate();

	ID3D11Device* GetDevice() const { return m_pDevice; }
	ID3D11DeviceContext* GetDXDC() const { return m_pDXDC; }

	//������ ��� �ٲٱ�
	void SetRenderMode(DWORD val) { m_RMode = val; }
	//DWORD GetRederMode() const { return m_RMode; }

protected:
	HRESULT _DirectXSetup(HWND hWnd);
	void	_DirectXRelease();

	//XMFLOAT4 r, g, b, a
	int		_ClearBackBuffer(UINT flag, XMFLOAT4 col, float depth = 1.0f, UINT stencil = 0);
	//int     _Flip();

	//���߿� Ÿ�̸� Ŭ������ ���� ���� #����
	float	_GetEngineTime();

	// �ʱ� ��ġ ���� ���� �Լ���.
	//
	HRESULT _CreateDeviceSwapChain(HWND hwnd);
	HRESULT _CreateRenderTarget();
	HRESULT _CreateDepthStencil();

	void	_SetViewPort();

	
	
	// ������ ���� ��ü ���� / ��� �Լ���.
	int _RenderStateObjectCreate();
	void _RenderStateObjectRelease();

	// ����/���ٽ� ���� ��ü�� : ���� ��ü ������.
	int  _DepthStencilStateCreate();
	void _DepthStencilStateRelease();

	// �����Ͷ����� ���� ��ü Rasterizer State Objects 
	void _RasterStateCreate();
	void _RasterStateRelease();
	void _RenderModeUpdate();

	// ����/���� ȥ�� ���� ��ü Blending State Objects 
	void _BlendStateCreate();
	void _BlendStateRelease();
	void _BlendModeUpdate();

	// �ؽ�ó ���÷�.
	void _SamplerCreate();
	void _SamplerRelease();



	//void	_PutFPS(int x, int y);
	//void  ynTextDraw( int x, int y, COLOR col, char* msg, ...);

	

	


};

// ��Ÿ �ý��� ���� �Լ� �� ������. �ٸ� ������Ͽ��� ���� #���� 