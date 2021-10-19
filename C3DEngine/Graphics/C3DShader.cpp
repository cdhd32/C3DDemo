#include "../pch.h"
#include "C3DDevice.h"

//���߿� ���̴��� ���? �޴� Ŭ�������� ����ϴ� ���� ���� �� #����
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

//���̴��� ����� ����̽����� VB�� ����
void C3DShader::CreateVB(void* pBuff, UINT size, ID3D11Buffer** ppVB)
{
	m_pDevice->CreateVB(pBuff, size, ppVB);
}

//���̴��� ����� ����̽����� IB�� ����
void C3DShader::CreateIB(void* pBuff, UINT size, ID3D11Buffer** ppIB)
{
	m_pDevice->CreateIB(pBuff, size, ppIB);
}

//��ȯ ��� ������Ʈ worldTM, type�� ���� �ٸ� ������ ������Ʈ ������(������ worldTM ������)
void C3DShader::SetTransform(DWORD type, XMMATRIX& rTM)
{
	m_DefalutCBuffer.tm[type] = rTM;

	//���� ��� ����.
	m_DefalutCBuffer.mWV = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView;
	m_DefalutCBuffer.mWVP = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView * m_DefalutCBuffer.mProj;

	//���̴� �� ������� ���� 
	//�� ��� �����ø��� �����ϴ� ���� ��ȿ����������
	//������ �������� ���� �̰����� ó���ϰڽ��ϴ�.
	//ShaderUpdate();
	//CBUpdate();
}

//��ȯ ��� ������Ʈ viewTM, projTM
void C3DShader::SetViewProjMatrix()
{
	//g_cbDef.tm[type] = refTM;
	m_DefalutCBuffer.mView = m_pDefaultCam->GetViewMatrix();
	m_DefalutCBuffer.mProj = m_pDefaultCam->GetProjMatrix();

	//���� ��� ����.
	m_DefalutCBuffer.mWV = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView;
	m_DefalutCBuffer.mWVP = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView * m_DefalutCBuffer.mProj;

	//���̴� ��� ���� ����.

	//�⺻ ���̴� ����.
	//g_pDXDC->VSSetShader(g_pVS, nullptr, 0);
	//g_pDXDC->PSSetShader(g_pPS, nullptr, 0);

	//�⺻ ���̴� ������� ����.
	//g_pDXDC->VSSetConstantBuffers(0, 1, &g_pCBDef);
}

//�ӽ� ������� ������Ʈ
void C3DShader::UpdateTempCB()
{
	//��� ���� ��ü ���� ������ ������Ʈ
	//worldTM ������Ʈ
	if(m_pWorldTranfrom != nullptr)
	{
		m_DefalutCBuffer.mTM = m_pWorldTranfrom->GetTransformMatrix();

		//���� ��� ����.
		m_DefalutCBuffer.mWV = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView;
		m_DefalutCBuffer.mWVP = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView * m_DefalutCBuffer.mProj;
	}
	else
	{
		//������Ʈ�� worldTM �����
		XMMATRIX mTM = XMMatrixIdentity();
		XMMATRIX _mScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		XMMATRIX _mRot = XMMatrixRotationRollPitchYaw(0, 0, 0);
		XMMATRIX _mTrans = XMMatrixTranslation(0, 0, 0);
		mTM = _mScale * _mRot * _mTrans;

		m_DefalutCBuffer.mTM = mTM;

		//���� ��� ����.
		m_DefalutCBuffer.mWV = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView;
		m_DefalutCBuffer.mWVP = m_DefalutCBuffer.mTM * m_DefalutCBuffer.mView * m_DefalutCBuffer.mProj;
	}

	if (m_pDefaultCam != nullptr)
	{
		m_DefalutCBuffer.mView = m_pDefaultCam->GetViewMatrix();
		m_DefalutCBuffer.mProj = m_pDefaultCam->GetProjMatrix();

		//���� ��� ����.
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

	// �⺻���̴� ����.(����,����)
	//_ShaderLoad(L"../Data/fx/Default.fx", &g_pVS, &g_pPS, &g_pVSCode);
	//_ShaderLoad(L"../Data/fx/Default(perVtx).fx", &m_pVS, &m_pPS, &m_pVSCode); //���ȼ� �������� ���߿� �ٽ� �غ��� #����
	_ShaderLoad(pShaderFileName, &m_pVS, &m_pPS, &m_pVSCode);

	// �⺻ ���� �Է±��� ����.(����,����)
	// ���̴��� ������� �ʿ�. ���� ���𿡼��� ���� ��. #����
	CreateVBLayout(shaderTpye);

	//CreateVBLayoutCVtx();

	// �⺻ ������� ����.(����,����)
	ZeroMemory(&m_DefalutCBuffer, sizeof(cbDEFAULT));
	CreateDynamicConstantBuffer(sizeof(cbDEFAULT), &m_DefalutCBuffer, &m_pCBDef);
	
	// ���� ������� ���� (�Ϲ�+������)
	ZeroMemory(&m_LightCBuffer, sizeof(LIGHT));
	CreateDynamicConstantBuffer(sizeof(LIGHT), &m_LightCBuffer, &m_pCBLight);

	ZeroMemory(&m_MtrlCBuffer, sizeof(MATERIAL));
	CreateDynamicConstantBuffer(sizeof(MATERIAL), &m_MtrlCBuffer, &m_pCBMtrl);

	//���̴� ������� ����.(��������)
	//UpdateDynamicConstantBuffer(g_pDXDC, g_pCB, &g_CBuffer, sizeof(ConstBuffer));


	//�⺻ ���̴� ����.
	m_pDXDC->VSSetShader(m_pVS, nullptr, 0);
	m_pDXDC->PSSetShader(m_pPS, nullptr, 0);

	//�⺻ ���̴� ������� ����.
	//g_pDXDC->VSSetConstantBuffers(0, 1, &g_pCBDef);
	//g_pDXDC->VSSetConstantBuffers(1, 1, &g_pCBLight);
	//g_pDXDC->VSSetConstantBuffers(2, 1, &g_pCBMtrl);
}

void C3DShader::ShaderUpdate()
{
	//�⺻ ���̴� ����.
	m_pDXDC->VSSetShader(m_pVS, nullptr, 0);
	m_pDXDC->PSSetShader(m_pPS, nullptr, 0);

	
}

void C3DShader::UpdateConstantBuffer()
{
	//�⺻ ���̴� ������� ����.
	m_pDXDC->VSSetConstantBuffers(0, 1, &m_pCBDef);
	m_pDXDC->VSSetConstantBuffers(1, 1, &m_pCBLight);
	m_pDXDC->VSSetConstantBuffers(2, 1, &m_pCBMtrl);

	m_pDXDC->PSSetConstantBuffers(1, 1, &m_pCBDef);
	m_pDXDC->PSSetConstantBuffers(1, 1, &m_pCBLight);
	m_pDXDC->PSSetConstantBuffers(2, 1, &m_pCBMtrl);

	//���̴� ��� ���� ����.
	UpdateDynamicConstantBuffer(m_pDXDC, m_pCBDef, &m_DefalutCBuffer, sizeof(cbDEFAULT));
	UpdateDynamicConstantBuffer(m_pDXDC, m_pCBLight, &m_LightCBuffer, sizeof(LIGHT));
	UpdateDynamicConstantBuffer(m_pDXDC, m_pCBMtrl, &m_MtrlCBuffer, sizeof(MATERIAL));
}

HRESULT C3DShader::_ShaderLoad(TCHAR* fxname, ID3D11VertexShader** ppVS, ID3D11PixelShader** ppPS, ID3DBlob** ppCode)
{
	HRESULT hr = S_OK;

	// ���� ���̴� ����.
	hr = _VertexShaderLoad(fxname, "VS_Main", "vs_5_0", ppVS, ppCode);

	// �ȼ� ���̴� ����.
	hr = _PixelShaderLoad(fxname, "PS_Main", "ps_5_0", ppPS);

	return hr;
}

HRESULT C3DShader::_VertexShaderLoad(TCHAR* fxname, char* entry, char* target, ID3D11VertexShader** ppVS, ID3DBlob** ppCode /*= NULL*/)
{
	HRESULT hr = S_OK;

	// ���� ���̴� ������ Compile a VertexShader
	ID3D11VertexShader* pVS = nullptr;
	ID3DBlob* pVSCode = nullptr;
	hr = _ShaderCompile(fxname, entry, target, &pVSCode);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"[����] ShaderLoad :: Vertex Shader ������ ����", L"Error", MB_OK | MB_ICONERROR);
		return hr;
	}
	// ���� ���̴� ��ü ���� Create a VS Object 
	hr = m_pDXDevice->CreateVertexShader(pVSCode->GetBufferPointer(),
		pVSCode->GetBufferSize(),
		nullptr,
		&pVS
	);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pVSCode);			//�ӽ� ��ü ����.
		return hr;
	}

	//--------------------------
	// ������ ��ü �ܺ� ����
	//--------------------------
	*ppVS = pVS;
	if (ppCode) *ppCode = pVSCode;
	return hr;

	return hr;
}

HRESULT C3DShader::_PixelShaderLoad(TCHAR* fxname, char* entry, char* target, ID3D11PixelShader** ppPS)
{
	HRESULT hr = S_OK;

	// �ȼ� ���̴� ������ Compile a PixelShader
	ID3D11PixelShader* pPS = nullptr;
	ID3DBlob* pPSCode = nullptr;
	hr = _ShaderCompile(fxname, entry, target, &pPSCode);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"[����] ShaderLoad :: Pixel Shader ������ ����", L"Error", MB_OK | MB_ICONERROR);
		return hr;
	}
	// �ȼ� ���̴� ��ü ���� Create a PS Object 
	hr = m_pDXDevice->CreatePixelShader(pPSCode->GetBufferPointer(),
		pPSCode->GetBufferSize(),
		nullptr,
		&pPS
	);

	SAFE_RELEASE(pPSCode);				//�ӽ� ��ü ����.	
	if (FAILED(hr))	return hr;


	//--------------------------
	// ������ ��ü �ܺ� ����
	//--------------------------
	*ppPS = pPS;

	return hr;
}

HRESULT C3DShader::_ShaderCompile(WCHAR* FileName, /*�ҽ������̸�. */ char* EntryPoint, /*�����Լ� ������. */ char* ShaderModel, /*���̴� ��. */ ID3DBlob** ppCode /*[���] �����ϵ� ���̴� �ڵ�. */)
{
	HRESULT hr = S_OK;
	ID3DBlob* pError = nullptr;

	//������ �ɼ�1.
	UINT Flags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;		//���켱 ��� ó��. ���� DX9 ���������� �������� ���. �ӵ��� �䱸�ȴٸ�, "��켱" ���� ó���� ��.
	//UINT Flags = D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;	//��켱 ��� ó��. �ӵ��� ����� ������, ����� ��ġ �� GPU �� �����ؾ� �մϴ�.
	//UINT Flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#ifdef _DEBUG
	Flags |= D3DCOMPILE_DEBUG;							//����� ���� �ɼ� �߰�.
#endif

	//���̴� �ҽ� ������.
	hr = D3DCompileFromFile(FileName,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�ܺ� ���� Include ���. 
		EntryPoint,
		ShaderModel,
		Flags,				//������ �ɼ�1
		0,					//������ �ɼ�2,  Effect ���� �����Ͻ� �����. �̿ܿ��� ���õ�.
		ppCode,				//[���] �����ϵ� ���̴� �ڵ�.
		&pError				//[���] ������ ���� �ڵ�.
	);
	if (FAILED(hr))
	{
		//������ ����Ȯ���� ���� Error �޼����� ����մϴ�.
		//ynErrorW(TRUE, L"���̴� ������ ����", hr, pError, FileName, EntryPoint, ShaderModel);
		MessageBox(NULL, L"���̴� ������ ����", L"Error", MB_OK);
	}

	SAFE_RELEASE(pError);
	return hr;
}
void C3DShader::ShaderRelease()
{
	m_pDefaultCam = nullptr;
	m_pDefaultLit = nullptr;
	m_pDefaultMtrl = nullptr;

	SAFE_RELEASE(m_pVS);			//���� ���̴� ����.
	SAFE_RELEASE(m_pPS);			//�ȼ� ���̴� ����.
	SAFE_RELEASE(m_pVSCode);		//���� ���̴� ������ �ڵ� (����) ����.

	SAFE_RELEASE(m_pCBDef);			//�⺻ ������� ����.

	SAFE_RELEASE(m_pVBLayout);		//���� �Է·��ƾƿ� ����.	
}

//�ϴ��� ���̴� ������ �̸� �����ؼ� ���̴��� �´� VBLayout�� ����
//���̴��� �߰� �� ������ ���ŷο� ����̱� ������ �� ���� ����� ã��#���� �ϴ� ���� ���� �� ����.
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

	// ���� �Է±��� Input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//  Sementic          format                       offset         classification             
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// ���� �Է±��� ��ü ���� Create the input layout
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

	// ���� �Է±��� Input layout
	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//  Sementic          format                       offset         classification             
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// ���� �Է±��� ��ü ���� Create the input layout
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
//	// ���� �Է±��� Input layout
//	D3D11_INPUT_ELEMENT_DESC layout[] =
//	{
//		//  Sementic          format                       offset         classification             
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,     0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	};
//	UINT numElements = ARRAYSIZE(layout);
//
//	// ���� �Է±��� ��ü ���� Create the input layout
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
	//���� ���� ����.
	//UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pDXDC->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);

	//�ε��� ���� ����
	//g_pDXDC->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);

	// ���� ���� ���� ���� Set primitive topology
	m_pDXDC->IASetPrimitiveTopology(topology); 
}

void C3DShader::DrawFullVtx(int vtxCnt)
{
	//�Է� ���̾ƿ� ����. Set the input layout
	m_pDXDC->IASetInputLayout(m_pVBLayout);

	m_pDXDC->Draw(vtxCnt, 0);
}

HRESULT C3DShader::CreateConstantBuffer(UINT size, ID3D11Buffer** ppCB)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pCB = nullptr;

	//��� ���� ������, ũ�� �׽�Ʈ.
	//������۴� 16����Ʈ ���ĵ� ũ��� �����ؾ� �մϴ�.
	if (size % 16)
	{
		// 16����Ʈ ���Ľ���... �޼��� ���..
		/*ynErrorW(TRUE, L"[���] ������� : 16����Ʈ ������. ���ۻ����� �����մϴ�. \n"
			L"CB.Size = %d bytes",
			size);*/
		MessageBox(NULL, L"[���] ������� : 16����Ʈ ������. ���ۻ����� �����մϴ�.", L"Error", MB_OK);
	}
	else
	{
		/* 16����Ʈ ���� OK. �޼��� ����...
		ynErrorW(L"[�˸�] ������� : 16����Ʈ ���ĵ�. \n"
				 L"CB.Size = %d bytes",
				 size);*/
	}

	//��� ���� ���� ����.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	//��� ���� ����.
	hr = m_pDXDevice->CreateBuffer(&bd, nullptr, &pCB);
	if (FAILED(hr))
	{
		//ynError(hr, L"������� ���� ����");
		MessageBox(NULL, L"������� ���� ����", L"Error", MB_OK);
		return hr;
	}

	//�ܺη� ����.
	*ppCB = pCB;

	return hr;
}

HRESULT C3DShader::CreateDynamicConstantBuffer(UINT size, LPVOID pData, ID3D11Buffer** ppCB)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pCB = nullptr;


	//��� ���� ������, ũ�� �׽�Ʈ.
	//������۴� 16����Ʈ ���ĵ� ũ��� �����ؾ� �մϴ�.
	if (size % 16)
	{
		// 16����Ʈ ���Ľ���... �޼��� ���..
		/*ynErrorW(TRUE, L"[���] ������� : 16����Ʈ ������. ���ۻ����� �����մϴ�. \n"
			L"CB = 0x%X \n"
			L"CB.Size = %d bytes",
			pData, size);*/
		MessageBox(NULL, L"[���] ������� : 16����Ʈ ������. ���ۻ����� �����մϴ�.", L"Error", MB_OK);
	}
	else
	{
		/* 16����Ʈ ���� OK. �޼��� ����...
		ynErrorW(L"[�˸�] ������� : 16����Ʈ ���ĵ�. \n"
				 L"CB = 0x%X \n"
				 L"CB.Size = %d bytes",
				 pData, size);*/
	}


	//��� ���� ���� ����.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;				//���� �������� ����.
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//CPU ���� ����.

	//���긮�ҽ� ����.
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = pData;				//(�ܺ�) ��� ������ ����.
	//sd.SysMemPitch = 0;
	//sd.SysMemSlicePitch = 0;

	//��� ���� ����.
	hr = m_pDXDevice->CreateBuffer(&bd, &sd, &pCB);
	if (FAILED(hr))
	{
		//ynError(hr, L"���� ������� ���� ����");
		MessageBox(NULL, L"������� ���� ����", L"Error", MB_OK);
		return hr;
	}

	//�ܺη� ����.
	*ppCB = pCB;

	return hr;
}

HRESULT C3DShader::UpdateDynamicConstantBuffer(ID3D11DeviceContext* pDXDC, ID3D11Resource* pBuff, LPVOID pData, UINT size)
{
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE mr;
	ZeroMemory(&mr, sizeof(mr));

	//������� ����
	hr = pDXDC->Map(pBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	if (FAILED(hr))
	{
		//ynError(hr, L"UpdateDynamicConstantBuffer : Map ����");
		MessageBox(NULL, L"UpdateDynamicConstantBuffer : Map ����", L"Error", MB_OK);
		return hr;
	}

	//��� ���� ����.
	memcpy(mr.pData, pData, size);

	//������� �ݱ�.
	pDXDC->Unmap(pBuff, 0);


	return hr;
}

