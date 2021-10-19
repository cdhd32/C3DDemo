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
	//상태관련 변수는 따로 빼서 상태만 관리하는 클래스/객체로 관리 #수정
	HWND m_hWnd;

	// D3D 관련 개체 인터페이스 포인터.
	ID3D11Device* m_pDevice;				//DX Device
	ID3D11DeviceContext* m_pDXDC;			//DX DC
	IDXGISwapChain* m_pSwapChain;			//스왑 체인
	ID3D11RenderTargetView* m_pRTView;		//렌더 타겟 뷰
	ID3D11Texture2D* m_pDS;					//깊이-스텐실 버퍼.
	ID3D11DepthStencilView* m_pDSView;		//깊이-스텐실 뷰 

	// 렌더링 상태 객체들

	// 깊이/스텐실 상태 객체
	ID3D11DepthStencilState* m_DSState[DS_MAX_] = { nullptr, };
	// 레스터라이져 상태 객체
	ID3D11RasterizerState* m_RState[RS_MAX_] = { nullptr, };
	// Blending State
	ID3D11BlendState* m_BState[BS_MAX_] = { nullptr, };
	// 텍스처 셈플러 객체
	ID3D11SamplerState* m_pSampler[SS_MAX] = { nullptr, };

	//현재 렌더링 모드.
	DWORD m_RMode;

	// 렌더링 상태 컨트롤 변수 Update 쪽에서..
	BOOL m_bCullBack;		//뒷면 제거...On/Off.
	BOOL m_bWireFrame;		//와이어 프레임 On/Off.
	BOOL m_bZEnable;			//깊이 버퍼 연산 On/Off.
	BOOL m_bZWrite;			//깊이버퍼 쓰기 On/Off.

	//전체화면 사용여부.
	BOOL m_bWindowMode = TRUE;

	//수직동기화 사용여부.
	BOOL m_bVSync = FALSE;

	//AA & AF Option.
	DWORD		m_dwAA = 8;		//AA 배수 (최대 8), AA=1 (OFF)
	DWORD		m_dwAF = 16;	//Anisotropic Filter 배수.(최대 16)
	BOOL		m_bMipMap = TRUE;

	//D3D 기능 레벨 (Direct3D feature level) 
	//현재 사용할 DX 버전 지정. DX 렌더링 장치의 호환성 향상
	D3D_FEATURE_LEVEL m_FeatureLevels = D3D_FEATURE_LEVEL_11_0;		//DX11 대응.

public:
	C3DDevice();
	~C3DDevice();

	HRESULT Initialize(HWND hWnd);
	void	Release();

	//기하버퍼 생성. LPDEVICE
	HRESULT CreateVB(void* pBuff, UINT size, ID3D11Buffer** ppVB);
	HRESULT CreateIB(void* pBuff, UINT size, ID3D11Buffer** ppIB);

	// 렌더타겟, 깊이/스텐실 버퍼 클리어.
	void RTClear(Vector4 col);
	void Flip();

	void RenderStateUpdate();

	ID3D11Device* GetDevice() const { return m_pDevice; }
	ID3D11DeviceContext* GetDXDC() const { return m_pDXDC; }

	//랜더링 모드 바꾸기
	void SetRenderMode(DWORD val) { m_RMode = val; }
	//DWORD GetRederMode() const { return m_RMode; }

protected:
	HRESULT _DirectXSetup(HWND hWnd);
	void	_DirectXRelease();

	//XMFLOAT4 r, g, b, a
	int		_ClearBackBuffer(UINT flag, XMFLOAT4 col, float depth = 1.0f, UINT stencil = 0);
	//int     _Flip();

	//나중에 타이머 클래스로 따로 빼기 #수정
	float	_GetEngineTime();

	// 초기 장치 설정 관련 함수들.
	//
	HRESULT _CreateDeviceSwapChain(HWND hwnd);
	HRESULT _CreateRenderTarget();
	HRESULT _CreateDepthStencil();

	void	_SetViewPort();

	
	
	// 렌더링 상태 객체 생성 / 운용 함수들.
	int _RenderStateObjectCreate();
	void _RenderStateObjectRelease();

	// 깊이/스텐실 상태 객체들 : 엔진 전체 공유함.
	int  _DepthStencilStateCreate();
	void _DepthStencilStateRelease();

	// 레스터라이져 상태 객체 Rasterizer State Objects 
	void _RasterStateCreate();
	void _RasterStateRelease();
	void _RenderModeUpdate();

	// 색상/알파 혼합 상태 객체 Blending State Objects 
	void _BlendStateCreate();
	void _BlendStateRelease();
	void _BlendModeUpdate();

	// 텍스처 셈플러.
	void _SamplerCreate();
	void _SamplerRelease();



	//void	_PutFPS(int x, int y);
	//void  ynTextDraw( int x, int y, COLOR col, char* msg, ...);

	

	


};

// 기타 시스템 관련 함수 및 재정의. 다른 헤더파일에서 관리 #수정 