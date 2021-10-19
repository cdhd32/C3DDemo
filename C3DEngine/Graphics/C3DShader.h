#pragma once

//��� ���� ���Ż��.
enum {
	TS_WORLD,		//D3DTS_WORLD ����.
	TS_VIEW,		//D3DTS_VIEW ����.
	TS_PROJ,		//D3DTS_PROJ ����.
	TS_WV,
	TS_WVP,			//D3DTS_xxx ����. 
	TS_MAX
};

//���̴� ��� ����, ��� �������� ��ȣ�� ����
enum class DefinedConstantBuffer
{
	CB_DEFAULT,		//�⺻ ��ȯ(Transform) ��� ����
	CB_LIGHT,		//����Ʈ ���� ����
	CB_MATERIAL,	//���׸��� ���� ����
	CB_MAX 
};

enum class SHADERTYPE
{
	SHADER_DEFALT,
	SHADER_LINE,
	
	_SHADER_MAX
};

// ��� ���ۿ� ����ü : ���̴� ���� ���꿡 ���� �����͵�. 
// GPU �� �����͸� ������ ����� �����͸� �����մϴ�.  
struct cbDEFAULT
{
	union
	{
		struct
		{
			XMMATRIX mTM;		//"World" ��ȯ ��� : DirectXMath, 16����Ʈ ���� ����. 
			XMMATRIX mView;		//"View" ��ȯ ���
			XMMATRIX mProj;		//"Projection" ��ȯ ���
			XMMATRIX mWV;		// World+view ȥ�����. 
			XMMATRIX mWVP;		// World+view+Proj ȥ�����. 
		};

		//��ȯ���.
		XMMATRIX tm[TS_MAX];	//DirectXMath, 16����Ʈ ���� ����. 
	};
};

//���� ���� ����ü
struct LIGHT
{
	XMFLOAT3A Direction;   //���� ����.
	XMFLOAT4A Diffuse;     //�� ���� : Ȯ�걤 Diffuse Light.
	XMFLOAT4A Ambient;     //���� ���� : �ֺ��� Ambient Light.
	XMFLOAT4A Specular;    //���ݻ� ���� : Specular Light.
	FLOAT  Range;       //�� ���� �Ÿ�.
	BOOL   bDiffOn;     //���� ���뿩��.
	BOOL   bSpecOn;     //���ݻ� ���뿩��.
};

//���� ���� ����ü
struct MATERIAL
{
	XMFLOAT4A Diffuse;     //Ȯ�걤 �ݻ���(%) 
	XMFLOAT4A Ambient;     //�ֺ��� �ݻ���(%) 
	XMFLOAT4A Specular;    //���ݻ籤 �ݻ���(%)
	FLOAT  Power;       //���ݻ� ������ : "��ĥ��" 
};

struct VERTEX
{
	float x, y, z; 			//��ǥ(Position)
	float nx, ny, nz;		//��� : Normal
	float u, v;				//�ؽ�ó ��ǥ: Texture-Coordinates 
};
class C3DDevice;

class C3DTransform;
class C3DCamera;
class C3DLight;
class C3DMaterial;

class C3DShader
{
public:

protected:
	C3DTransform* m_pWorldTranfrom;	//worldTM ������ ������ �ִ� �⺻ ����Transform #����
	C3DCamera* m_pDefaultCam;	//view, proj ������ ������ �ִ� �⺻ ī�޶�, �׽�Ʈ #����
	C3DLight* m_pDefaultLit;	//Light ������ ������ �ִ� �⺻ directional light, �׽�Ʈ #���� 
	C3DMaterial* m_pDefaultMtrl;	//Material ������ ������ �ִ� �⺻ Material	#����

	//���̴� �� ��ü ����.
	TCHAR m_ShaderFileName[256];
	ID3DBlob* m_pVSCode;			//���� ���̴� ������ �ڵ� ��ü.(�ӽ�)
	ID3D11VertexShader* m_pVS;	//���� ���̴� Vertex Shader �������̽�.
	ID3D11PixelShader* m_pPS;		//�ȼ� ���̴� Pixel Shader �������̽�.
	

	//���ſ� �������
	cbDEFAULT		m_DefalutCBuffer;
	LIGHT			m_LightCBuffer;
	MATERIAL		m_MtrlCBuffer;

	//���̴� ��� ����
	ID3D11Buffer* m_pCBDef;
	ID3D11Buffer* m_pCBLight;
	ID3D11Buffer* m_pCBMtrl;

	// ���� �Է·��̾ƿ� (����) : ���̴��� ������� �ʿ�. ���� ���𿡼��� ���� ��.
	ID3D11InputLayout* m_pVBLayout;

	//���ؽ� ����
	ID3D11Buffer* m_pVB;
	void* m_pVBuff;

	//�ε��� ����
	ID3D11Buffer* m_pIB;
	void* m_pIBuff;


	//���̵��� ���ִ� C3D����̽�
	C3DDevice* m_pDevice;

	// D3D ���� ��ü �������̽� ������
	ID3D11Device* m_pDXDevice;				//DX Device
	ID3D11DeviceContext* m_pDXDC;			//DX DC
public:
	//C3DShader();
	C3DShader(C3DDevice* pDevice);
	~C3DShader();

	void CreateVB(void* pBuff, UINT size, ID3D11Buffer** ppVB);
	void CreateIB(void* pBuff, UINT size, ID3D11Buffer** ppIB);

	//�ӽ� ��� ���� �޼ҵ�. (world matrix ���� ������Ʈ �� ���)
	void SetTransform(DWORD type, XMMATRIX& rTM);
	//�ӽ� ��� ���� �޼ҵ�. (view, proj matrix ���� ������Ʈ �� ���)
	void SetViewProjMatrix();
	
	void UpdateTempCB();

	void SetWorldTM(C3DTransform* pWorldTransform);
	//�ӽ� ī�޶� �׽�Ʈ�� �Լ� #����
	void SetCam(C3DCamera* pCam);
	//�ӽ� ����Ʈ �׽�Ʈ�� �Լ� #����
	void SetLight(C3DLight* pCam);
	//�ӽ� ���׸��� �׽�Ʈ�� �Լ� #����
	void SetMaterial(C3DMaterial* pMtrl);

	//void SetConstBufferObj(con);

	//void ShaderSetup(TCHAR* pShaderFileName);

	void ShaderSetup(TCHAR* pShaderFileName, SHADERTYPE shaderTpye);

	//Shader ��ü�� ������۸� ����� ������Ʈ
	void ShaderUpdate();
	void UpdateConstantBuffer();

	void ShaderRelease();
	
	//int CreateVBLayout();

	int C3DShader::CreateVBLayout(SHADERTYPE shaderType);
	//int CreateVBLayoutCVtx();
	
	//IA�� ���� ���� set
	//void SetVBtoIASimple(ID3D11Buffer* pVB, D3D_PRIMITIVE_TOPOLOGY topology);

	void SetVBtoIASimple(ID3D11Buffer* pVB, D3D_PRIMITIVE_TOPOLOGY topology, UINT stride);
	//�ε��� ���� �׸���
	void DrawFullVtx(int vtxCnt);

	//��� ���� ������Ʈ�� ���� ���̴������� protected �������� ���� #����
	//(����) ��� ���� ����.
	HRESULT CreateConstantBuffer(UINT size, ID3D11Buffer** ppCB);
	//���� ������� ����.
	HRESULT CreateDynamicConstantBuffer(UINT size, LPVOID pData, ID3D11Buffer** ppCB);
	//���� ������� ����.
	HRESULT UpdateDynamicConstantBuffer(ID3D11DeviceContext* pDXDC, ID3D11Resource* pBuff, LPVOID pData, UINT size);

protected:
	int _CreateVBLayout();
	int _CreateLineVBLayout();

	HRESULT _ShaderLoad(TCHAR* fxname, ID3D11VertexShader** ppVS, ID3D11PixelShader** ppPS, ID3DBlob** ppCode);
	HRESULT _VertexShaderLoad(TCHAR* fxname, char* entry, char* target, ID3D11VertexShader** ppVS, ID3DBlob** ppCode /*= NULL*/);
	HRESULT _PixelShaderLoad(TCHAR* fxname, char* entry, char* target, ID3D11PixelShader** ppPS);
	HRESULT _ShaderCompile(
		WCHAR* FileName,		//�ҽ������̸�.
		char* EntryPoint,		//�����Լ� ������.
		char* ShaderModel,		//���̴� ��.
		ID3DBlob** ppCode		//[���] �����ϵ� ���̴� �ڵ�.
	);
};

