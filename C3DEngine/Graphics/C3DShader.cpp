#include "../pch.h"
#include "C3DDevice.h"

//나중에 쉐이더를 상속? 받는 클래스에서 사용하는 것이 좋을 듯 #수정
#include "../System/C3DTransform.h"
#include "../Objects/C3DCamera.h"
#include "../Objects/C3DLight.h"
#include "../Objects/C3DMaterial.h"

#include "C3DShader.h"

C3DShader::C3DShader(C3DDevice* pDevice)
	:
	m_pWorldTranfrom(nullptr),
	m_pDefaultCam(nullptr),
	m_pDefaultLit(nullptr),
	m_pDefaultMtrl(nullptr),

	m_pVS(nullptr),
	m_pPS(nullptr),
	m_pVSCode(nullptr),

	m_pCBDef(nullptr),
	m_pCBLight(nullptr),
	m_pCBMtrl(nullptr),

	m_pVBLayout(nullptr),
	m_pVB(nullptr),
	m_pVBuff(nullptr),
	m_pIB(nullptr),
	m_pIBuff(nullptr),

	m_pDevice(pDevice),
	m_pDXDevice(m_pDevice->GetDevice()),
	m_pDXDC(m_pDevice->GetDXDC())
{

}

C3DShader::~C3DShader()
{

}

//쉐이더와 연결된 디바이스에서 VB를 생성
void C3DShader::CreateVB(void* pBuff, UINT size, ID3D11Buffer** ppVB)
{
	m_pDevice->CreateVB(pBuff, size, ppVB);
}

//쉐이더와 연결된 디바이스에서 IB를 생성
void C3DShader::CreateIB(void* pBuff, UINT size, ID3D11Buffer** ppIB)
{
	m_pDevice->CreateIB(pBuff, size, ppIB);
}

//변환 행렬 업데이트 worldTM, type에 따라 다른 정보도 업데이트 가능함(지금은 worldTM 정보만)
void C3DShader::SetTransform(DWORD type, XMMATRIX& rTM)
{
	m_DefalutCBuffer.tm[type] = rTM;

	//최종 행렬 결합.
	m_DefalutCBuffer.mWV = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView;
	m_DefalutCBuffer.mWVP = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView * m_DefalutCBuffer.mProj;

	//셰이더 및 상수버퍼 갱신 
	//각 행렬 설정시마다 갱신하는 것은 비효율적이지만
	//예제의 간결함을 위해 이곳에서 처리하겠습니다.
	//ShaderUpdate();
	//CBUpdate();
}

//변환 행렬 업데이트 viewTM, projTM
void C3DShader::SetViewProjMatrix()
{
	//g_cbDef.tm[type] = refTM;
	m_DefalutCBuffer.mView = m_pDefaultCam->GetViewMatrix();
	m_DefalutCBuffer.mProj = m_pDefaultCam->GetProjMatrix();

	//최종 행렬 결합.
	m_DefalutCBuffer.mWV = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView;
	m_DefalutCBuffer.mWVP = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView * m_DefalutCBuffer.mProj;

	//셰이더 상수 버퍼 갱신.

	//기본 셰이더 설정.
	//g_pDXDC->VSSetShader(g_pVS, nullptr, 0);
	//g_pDXDC->PSSetShader(g_pPS, nullptr, 0);

	//기본 셰이더 상수버퍼 설정.
	//g_pDXDC->VSSetConstantBuffers(0, 1, &g_pCBDef);
}

//임시 상수버퍼 업데이트
void C3DShader::UpdateTempCB()
{
	//상수 버퍼 객체 별로 있으면 업데이트
	//worldTM 업데이트
	if(m_pWorldTranfrom != nullptr)
	{
		m_DefalutCBuffer.mTM = m_pWorldTranfrom->GetTransformMatrix();

		//최종 행렬 결합.
		m_DefalutCBuffer.mWV = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView;
		m_DefalutCBuffer.mWVP = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView * m_DefalutCBuffer.mProj;
	}
	else
	{
		//오브젝트의 worldTM 만들기
		XMMATRIX mTM = XMMatrixIdentity();
		XMMATRIX _mScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		XMMATRIX _mRot = XMMatrixRotationRollPitchYaw(0, 0, 0);
		XMMATRIX _mTrans = XMMatrixTranslation(0, 0, 0);
		mTM = _mScale * _mRot * _mTrans;

		m_DefalutCBuffer.mTM = mTM;

		//최종 행렬 결합.
		m_DefalutCBuffer.mWV = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView;
		m_DefalutCBuffer.mWVP = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView * m_DefalutCBuffer.mProj;
	}

	if (m_pDefaultCam != nullptr)
	{
		m_DefalutCBuffer.mView = m_pDefaultCam->GetViewMatrix();
		m_DefalutCBuffer.mProj = m_pDefaultCam->GetProjMatrix();

		//최종 행렬 결합.
		m_DefalutCBuffer.mWV = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView;
		m_DefalutCBuffer.mWVP = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView * m_DefalutCBuffer.mProj;
	}

	if (m_pDefaultLit != nullptr)
	{
		XMVECTOR dir = m_pDefaultLit->GetDirection();
		XMStoreFloat3A(&m_LightCBuffer.Direction, XMVector3Normalize(-dir));
		XMStoreFloat4A(&m_LightCBuffer.Diffuse, m_pDefaultLit->GetDiffuse());
		XMStoreFloat4A(&m_LightCBuffer.Ambient, m_pDefaultLit->GetAmbient());
		XMStoreFloat4A(&m_LightCBuffer.Specular, m_pDefaultLit->GetSpecular());
		m_LightCBuffer.Range = m_pDefaultLit->GetRange();
		m_LightCBuffer.bDiffOn = m_pDefaultLit->GetDiffOn();
		m_LightCBuffer.bSpecOn = m_pDefaultLit->GetSpecOn();
	}

	if (m_pDefaultMtrl != nullptr)
	{
		XMStoreFloat4A(&m_MtrlCBuffer.Diffuse, m_pDefaultMtrl->GetDiffuse());
		XMStoreFloat4A(&m_MtrlCBuffer.Ambient, m_pDefaultMtrl->GetAmbient());
		XMStoreFloat4A(&m_MtrlCBuffer.Specular, m_pDefaultMtrl->GetSpecular());
		m_MtrlCBuffer.Power = m_pDefaultMtrl->GetPower();
	}
}

void C3DShader::SetWorldTM(C3DTransform* pWorldTransform)
{
	m_pWorldTranfrom = pWorldTransform;

	m_DefalutCBuffer.mTM = m_pWorldTranfrom->GetTransformMatrix();
}

void C3DShader::SetCam(C3DCamera* pCam)
{
	m_pDefaultCam = pCam;

	m_DefalutCBuffer.mView = m_pDefaultCam->GetViewMatrix();
	m_DefalutCBuffer.mProj = m_pDefaultCam->GetProjMatrix();
}

void C3DShader::SetLight(C3DLight* pLit)
{
	m_pDefaultLit = pLit;

	XMVECTOR dir = m_pDefaultLit->GetDirection();
	XMStoreFloat3A(&m_LightCBuffer.Direction, XMVector3Normalize(-dir));
	XMStoreFloat4A(&m_LightCBuffer.Diffuse, m_pDefaultLit->GetDiffuse());
	XMStoreFloat4A(&m_LightCBuffer.Ambient, m_pDefaultLit->GetAmbient());
	XMStoreFloat4A(&m_LightCBuffer.Specular, m_pDefaultLit->GetSpecular());
	m_LightCBuffer.Range = m_pDefaultLit->GetRange();
	m_LightCBuffer.bDiffOn = m_pDefaultLit->GetDiffOn();
	m_LightCBuffer.bSpecOn = m_pDefaultLit->GetSpecOn();
}

void C3DShader::SetMaterial(C3DMaterial* pMtrl)
{
	m_pDefaultMtrl = pMtrl;

	XMStoreFloat4A(&m_MtrlCBuffer.Diffuse, m_pDefaultMtrl->GetDiffuse());
	XMStoreFloat4A(&m_MtrlCBuffer.Ambient, m_pDefaultMtrl->GetAmbient());
	XMStoreFloat4A(&m_MtrlCBuffer.Specular, m_pDefaultMtrl->GetSpecular());
	m_MtrlCBuffer.Power = m_pDefaultMtrl->GetPower();
}

void C3DShader::ShaderSetup(TCHAR* pShaderFileName, SHADERTYPE shaderTpye)
{

	// 기본셰이더 생성.(전역,공유)
	//_ShaderLoad(L"../Data/fx/Default.fx", &g_pVS, &g_pPS, &g_pVSCode);
	//_ShaderLoad(L"../Data/fx/Default(perVtx).fx", &m_pVS, &m_pPS, &m_pVSCode); //퍼픽셀 라이팅은 나중에 다시 해보자 #수정
	_ShaderLoad(pShaderFileName, &m_pVS, &m_pPS, &m_pVSCode);

	// 기본 정점 입력구조 생성.(전역,공유)
	// 셰이더별 개별운용 필요. 현재 데모에서는 공유 중. #수정
	CreateVBLayout(shaderTpye);

	//CreateVBLayoutCVtx();

	// 기본 상수버퍼 생성.(전역,공유)
	ZeroMemory(&m_DefalutCBuffer, sizeof(cbDEFAULT));
	CreateDynamicConstantBuffer(sizeof(cbDEFAULT), &m_DefalutCBuffer, &m_pCBDef);
	
	// 조명 상수버퍼 생성 (일반+정렬형)
	ZeroMemory(&m_LightCBuffer, sizeof(LIGHT));
	CreateDynamicConstantBuffer(sizeof(LIGHT), &m_LightCBuffer, &m_pCBLight);

	ZeroMemory(&m_MtrlCBuffer, sizeof(MATERIAL));
	CreateDynamicConstantBuffer(sizeof(MATERIAL), &m_MtrlCBuffer, &m_pCBMtrl);

	//셰이더 상수버퍼 갱신.(동적버퍼)
	//UpdateDynamicConstantBuffer(g_pDXDC, g_pCB, &g_CBuffer, sizeof(ConstBuffer));


	//기본 셰이더 설정.
	m_pDXDC->VSSetShader(m_pVS, nullptr, 0);
	m_pDXDC->PSSetShader(m_pPS, nullptr, 0);

	//기본 셰이더 상수버퍼 설정.
	//g_pDXDC->VSSetConstantBuffers(0, 1, &g_pCBDef);
	//g_pDXDC->VSSetConstantBuffers(1, 1, &g_pCBLight);
	//g_pDXDC->VSSetConstantBuffers(2, 1, &g_pCBMtrl);
}

void C3DShader::ShaderUpdate()
{
	//기본 셰이더 설정.
	m_pDXDC->VSSetShader(m_pVS, nullptr, 0);
	m_pDXDC->PSSetShader(m_pPS, nullptr, 0);

	
}

void C3DShader::UpdateConstantBuffer()
{
	//기본 셰이더 상수버퍼 설정.
	m_pDXDC->VSSetConstantBuffers(0, 1, &m_pCBDef);
	m_pDXDC->VSSetConstantBuffers(1, 1, &m_pCBLight);
	m_pDXDC->VSSetConstantBuffers(2, 1, &m_pCBMtrl);

	m_pDXDC->PSSetConstantBuffers(1, 1, &m_pCBDef);
	m_pDXDC->PSSetConstantBuffers(1, 1, &m_pCBLight);
	m_pDXDC->PSSetConstantBuffers(2, 1, &m_pCBMtrl);

	//셰이더 상수 버퍼 갱신.
	UpdateDynamicConstantBuffer(m_pDXDC, m_pCBDef, &m_DefalutCBuffer, sizeof(cbDEFAULT));
	UpdateDynamicConstantBuffer(m_pDXDC, m_pCBLight, &m_LightCBuffer, sizeof(LIGHT));
	UpdateDynamicConstantBuffer(m_pDXDC, m_pCBMtrl, &m_MtrlCBuffer, sizeof(MATERIAL));
}

HRESULT C3DShader::_ShaderLoad(TCHAR* fxname, ID3D11VertexShader** ppVS, ID3D11PixelShader** ppPS, ID3DBlob** ppCode)
{
	HRESULT hr = S_OK;

	// 정점 셰이더 생성.
	hr = _VertexShaderLoad(fxname, "VS_Main", "vs_5_0", ppVS, ppCode);

	// 픽셀 셰이더 생성.
	hr = _PixelShaderLoad(fxname, "PS_Main", "ps_5_0", ppPS);

	return hr;
}

HRESULT C3DShader::_VertexShaderLoad(TCHAR* fxname, char* entry, char* target, ID3D11VertexShader** ppVS, ID3DBlob** ppCode /*= NULL*/)
{
	HRESULT hr = S_OK;

	// 정점 셰이더 컴파일 Compile a VertexShader
	ID3D11VertexShader* pVS = nullptr;
	ID3DBlob* pVSCode = nullptr;
	hr = _ShaderCompile(fxname, entry, target, &pVSCode);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"[실패] ShaderLoad :: Vertex Shader 컴파일 실패", L"Error", MB_OK | MB_ICONERROR);
		return hr;
	}
	// 정점 셰이더 객체 생성 Create a VS Object 
	hr = m_pDXDevice->CreateVertexShader(pVSCode->GetBufferPointer(),
		pVSCode->GetBufferSize(),
		nullptr,
		&pVS
	);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pVSCode);			//임시 개체 제거.
		return hr;
	}

	//--------------------------
	// 생성된 객체 외부 리턴
	//--------------------------
	*ppVS = pVS;
	if (ppCode) *ppCode = pVSCode;
	return hr;

	return hr;
}

HRESULT C3DShader::_PixelShaderLoad(TCHAR* fxname, char* entry, char* target, ID3D11PixelShader** ppPS)
{
	HRESULT hr = S_OK;

	// 픽셀 셰이더 컴파일 Compile a PixelShader
	ID3D11PixelShader* pPS = nullptr;
	ID3DBlob* pPSCode = nullptr;
	hr = _ShaderCompile(fxname, entry, target, &pPSCode);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"[실패] ShaderLoad :: Pixel Shader 컴파일 실패", L"Error", MB_OK | MB_ICONERROR);
		return hr;
	}
	// 픽셀 셰이더 객체 생성 Create a PS Object 
	hr = m_pDXDevice->CreatePixelShader(pPSCode->GetBufferPointer(),
		pPSCode->GetBufferSize(),
		nullptr,
		&pPS
	);

	SAFE_RELEASE(pPSCode);				//임시 개체 제거.	
	if (FAILED(hr))	return hr;


	//--------------------------
	// 생성된 객체 외부 리턴
	//--------------------------
	*ppPS = pPS;

	return hr;
}

HRESULT C3DShader::_ShaderCompile(WCHAR* FileName, /*소스파일이름. */ char* EntryPoint, /*메인함수 진입점. */ char* ShaderModel, /*셰이더 모델. */ ID3DBlob** ppCode /*[출력] 컴파일된 셰이더 코드. */)
{
	HRESULT hr = S_OK;
	ID3DBlob* pError = nullptr;

	//컴파일 옵션1.
	UINT Flags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;		//열우선 행렬 처리. 구형 DX9 이전까지의 전통적인 방식. 속도가 요구된다면, "행우선" 으로 처리할 것.
	//UINT Flags = D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;	//행우선 행렬 처리. 속도의 향상이 있지만, 행렬을 전치 후 GPU 에 공급해야 합니다.
	//UINT Flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#ifdef _DEBUG
	Flags |= D3DCOMPILE_DEBUG;							//디버깅 모드시 옵션 추가.
#endif

	//셰이더 소스 컴파일.
	hr = D3DCompileFromFile(FileName,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //외부 파일 Include 허용. 
		EntryPoint,
		ShaderModel,
		Flags,				//컴파일 옵션1
		0,					//컴파일 옵션2,  Effect 파일 컴파일시 적용됨. 이외에는 무시됨.
		ppCode,				//[출력] 컴파일된 셰이더 코드.
		&pError				//[출력] 컴파일 에러 코드.
	);
	if (FAILED(hr))
	{
		//컴파일 에러확인을 위해 Error 메세지를 출력합니다.
		//ynErrorW(TRUE, L"셰이더 컴파일 실패", hr, pError, FileName, EntryPoint, ShaderModel);
		MessageBox(NULL, L"셰이더 컴파일 실패", L"Error", MB_OK);
	}

	SAFE_RELEASE(pError);
	return hr;
}
void C3DShader::ShaderRelease()
{
	m_pDefaultCam = nullptr;
	m_pDefaultLit = nullptr;
	m_pDefaultMtrl = nullptr;

	SAFE_RELEASE(m_pVS);			//정점 셰이더 제거.
	SAFE_RELEASE(m_pPS);			//픽셀 셰이더 제거.
	SAFE_RELEASE(m_pVSCode);		//정점 셰이더 컴파일 코드 (버퍼) 제거.

	SAFE_RELEASE(m_pCBDef);			//기본 상수버퍼 제거.

	SAFE_RELEASE(m_pVBLayout);		//정점 입력레아아웃 제거.	
}

//일단을 쉐이더 종류를 미리 지정해서 쉐이더에 맞는 VBLayout을 구성
//쉐이더가 추가 될 때마다 번거로운 방법이기 때문에 더 나은 방법을 찾아#수정 하는 것이 좋을 것 같다.
int C3DShader::CreateVBLayout(SHADERTYPE shaderType)
{
	HRESULT hr = S_OK;

	switch (shaderType)
	{
	case SHADERTYPE::SHADER_DEFALT:
		hr = _CreateVBLayout();
		break;
	case SHADERTYPE::SHADER_LINE:
		hr = _CreateLineVBLayout();
		break;
	default:
		break;
	}

	return hr;
}

int C3DShader::_CreateVBLayout()
{
	HRESULT hr = S_OK;

	// 정점 입력구조 Input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//  Sementic          format                       offset         classification             
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// 정접 입력구조 객체 생성 Create the input layout
	hr = m_pDXDevice->CreateInputLayout(layout,
		numElements,
		m_pVSCode->GetBufferPointer(),
		m_pVSCode->GetBufferSize(),
		&m_pVBLayout
	);
	if (FAILED(hr))	return hr;

	return hr;
}

int C3DShader::_CreateLineVBLayout()
{
	HRESULT hr = S_OK;

	// 정점 입력구조 Input layout
	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//  Sementic          format                       offset         classification             
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// 정접 입력구조 객체 생성 Create the input layout
	hr = m_pDXDevice->CreateInputLayout(layout,
		numElements,
		m_pVSCode->GetBufferPointer(),
		m_pVSCode->GetBufferSize(),
		&m_pVBLayout
	);
	if (FAILED(hr))	return hr;

	return hr;
}



//int C3DShader::CreateVBLayoutCVtx()
//{
//	HRESULT hr = S_OK;
//
//	// 정점 입력구조 Input layout
//	D3D11_INPUT_ELEMENT_DESC layout[] =
//	{
//		//  Sementic          format                       offset         classification             
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,     0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	};
//	UINT numElements = ARRAYSIZE(layout);
//
//	// 정접 입력구조 객체 생성 Create the input layout
//	hr = g_pDevice->CreateInputLayout(layout,
//		numElements,
//		g_pVSColorCode->GetBufferPointer(),
//		g_pVSColorCode->GetBufferSize(),
//		&g_pVBLayoutColorVtx
//	);
//	if (FAILED(hr))	return hr;
//
//	return hr;
//}

void C3DShader::SetVBtoIASimple(ID3D11Buffer* pVB, D3D_PRIMITIVE_TOPOLOGY topology, UINT stride)
{
	//기하 버퍼 설정.
	//UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pDXDC->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);

	//인덱스 버퍼 설정
	//g_pDXDC->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);

	// 기하 위상 구조 설정 Set primitive topology
	m_pDXDC->IASetPrimitiveTopology(topology); 
}

void C3DShader::DrawFullVtx(int vtxCnt)
{
	//입력 레이아웃 설정. Set the input layout
	m_pDXDC->IASetInputLayout(m_pVBLayout);

	m_pDXDC->Draw(vtxCnt, 0);
}

HRESULT C3DShader::CreateConstantBuffer(UINT size, ID3D11Buffer** ppCB)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pCB = nullptr;

	//상수 버퍼 생성전, 크기 테스트.
	//상수버퍼는 16바이트 정렬된 크기로 생성해야 합니다.
	if (size % 16)
	{
		// 16바이트 정렬실패... 메세지 출력..
		/*ynErrorW(TRUE, L"[경고] 상수버퍼 : 16바이트 미정렬. 버퍼생성이 실패합니다. \n"
			L"CB.Size = %d bytes",
			size);*/
		MessageBox(NULL, L"[경고] 상수버퍼 : 16바이트 미정렬. 버퍼생성이 실패합니다.", L"Error", MB_OK);
	}
	else
	{
		/* 16바이트 정렬 OK. 메세지 생략...
		ynErrorW(L"[알림] 상수버퍼 : 16바이트 정렬됨. \n"
				 L"CB.Size = %d bytes",
				 size);*/
	}

	//상수 버퍼 정보 설정.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	//상수 버퍼 생성.
	hr = m_pDXDevice->CreateBuffer(&bd, nullptr, &pCB);
	if (FAILED(hr))
	{
		//ynError(hr, L"상수버퍼 생성 실패");
		MessageBox(NULL, L"상수버퍼 생성 실패", L"Error", MB_OK);
		return hr;
	}

	//외부로 전달.
	*ppCB = pCB;

	return hr;
}

HRESULT C3DShader::CreateDynamicConstantBuffer(UINT size, LPVOID pData, ID3D11Buffer** ppCB)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pCB = nullptr;


	//상수 버퍼 생성전, 크기 테스트.
	//상수버퍼는 16바이트 정렬된 크기로 생성해야 합니다.
	if (size % 16)
	{
		// 16바이트 정렬실패... 메세지 출력..
		/*ynErrorW(TRUE, L"[경고] 상수버퍼 : 16바이트 미정렬. 버퍼생성이 실패합니다. \n"
			L"CB = 0x%X \n"
			L"CB.Size = %d bytes",
			pData, size);*/
		MessageBox(NULL, L"[경고] 상수버퍼 : 16바이트 미정렬. 버퍼생성이 실패합니다.", L"Error", MB_OK);
	}
	else
	{
		/* 16바이트 정렬 OK. 메세지 생략...
		ynErrorW(L"[알림] 상수버퍼 : 16바이트 정렬됨. \n"
				 L"CB = 0x%X \n"
				 L"CB.Size = %d bytes",
				 pData, size);*/
	}


	//상수 버퍼 정보 설정.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;				//동적 정점버퍼 설정.
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//CPU 접근 설정.

	//서브리소스 설정.
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = pData;				//(외부) 상수 데이터 설정.
	//sd.SysMemPitch = 0;
	//sd.SysMemSlicePitch = 0;

	//상수 버퍼 생성.
	hr = m_pDXDevice->CreateBuffer(&bd, &sd, &pCB);
	if (FAILED(hr))
	{
		//ynError(hr, L"동적 상수버퍼 생성 실패");
		MessageBox(NULL, L"상수버퍼 생성 실패", L"Error", MB_OK);
		return hr;
	}

	//외부로 전달.
	*ppCB = pCB;

	return hr;
}

HRESULT C3DShader::UpdateDynamicConstantBuffer(ID3D11DeviceContext* pDXDC, ID3D11Resource* pBuff, LPVOID pData, UINT size)
{
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE mr;
	ZeroMemory(&mr, sizeof(mr));

	//상수버퍼 접근
	hr = pDXDC->Map(pBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	if (FAILED(hr))
	{
		//ynError(hr, L"UpdateDynamicConstantBuffer : Map 실패");
		MessageBox(NULL, L"UpdateDynamicConstantBuffer : Map 실패", L"Error", MB_OK);
		return hr;
	}

	//상수 버퍼 갱신.
	memcpy(mr.pData, pData, size);

	//상수버퍼 닫기.
	pDXDC->Unmap(pBuff, 0);


	return hr;
}

