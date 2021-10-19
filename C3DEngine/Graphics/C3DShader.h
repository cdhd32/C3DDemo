#pragma once

//행렬 접근 열거상수.
enum {
	TS_WORLD,		//D3DTS_WORLD 대응.
	TS_VIEW,		//D3DTS_VIEW 대응.
	TS_PROJ,		//D3DTS_PROJ 대응.
	TS_WV,
	TS_WVP,			//D3DTS_xxx 없음. 
	TS_MAX
};

//쉐이더 상수 버퍼, 상수 레지스터 번호에 대응
enum class DefinedConstantBuffer
{
	CB_DEFAULT,		//기본 변환(Transform) 행렬 버퍼
	CB_LIGHT,		//라이트 정보 버퍼
	CB_MATERIAL,	//마테리얼 정보 버퍼
	CB_MAX 
};

enum class SHADERTYPE
{
	SHADER_DEFALT,
	SHADER_LINE,
	
	_SHADER_MAX
};

// 상수 버퍼용 구조체 : 셰이더 내부 연산에 사용될 데이터들. 
// GPU 로 데이터를 전송할 사용자 데이터를 정의합니다.  
struct cbDEFAULT
{
	union
	{
		struct
		{
			XMMATRIX mTM;		//"World" 변환 행렬 : DirectXMath, 16바이트 정렬 버전. 
			XMMATRIX mView;		//"View" 변환 행렬
			XMMATRIX mProj;		//"Projection" 변환 행렬
			XMMATRIX mWV;		// World+view 혼합행렬. 
			XMMATRIX mWVP;		// World+view+Proj 혼합행렬. 
		};

		//변환행렬.
		XMMATRIX tm[TS_MAX];	//DirectXMath, 16바이트 정렬 버전. 
	};
};

//조명 정보 구조체
struct LIGHT
{
	XMFLOAT3A Direction;   //빛의 방향.
	XMFLOAT4A Diffuse;     //주 광량 : 확산광 Diffuse Light.
	XMFLOAT4A Ambient;     //보조 광량 : 주변광 Ambient Light.
	XMFLOAT4A Specular;    //정반사 광량 : Specular Light.
	FLOAT  Range;       //빛 도달 거리.
	BOOL   bDiffOn;     //조명 적용여부.
	BOOL   bSpecOn;     //정반사 적용여부.
};

//재질 정보 구조체
struct MATERIAL
{
	XMFLOAT4A Diffuse;     //확산광 반사율(%) 
	XMFLOAT4A Ambient;     //주변광 반사율(%) 
	XMFLOAT4A Specular;    //정반사광 반사율(%)
	FLOAT  Power;       //정반사 보정값 : "거칠기" 
};

struct VERTEX
{
	float x, y, z; 			//좌표(Position)
	float nx, ny, nz;		//노멀 : Normal
	float u, v;				//텍스처 좌표: Texture-Coordinates 
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
	C3DTransform* m_pWorldTranfrom;	//worldTM 정보를 가지고 있는 기본 월드Transform #수정
	C3DCamera* m_pDefaultCam;	//view, proj 정보를 가지고 있는 기본 카메라, 테스트 #수정
	C3DLight* m_pDefaultLit;	//Light 정보를 가지고 있는 기본 directional light, 테스트 #수정 
	C3DMaterial* m_pDefaultMtrl;	//Material 정보를 가지고 있는 기본 Material	#수정

	//셰이더 별 객체 선언.
	TCHAR m_ShaderFileName[256];
	ID3DBlob* m_pVSCode;			//정점 셰이더 컴파일 코드 개체.(임시)
	ID3D11VertexShader* m_pVS;	//정점 셰이더 Vertex Shader 인터페이스.
	ID3D11PixelShader* m_pPS;		//픽셀 셰이더 Pixel Shader 인터페이스.
	

	//갱신용 상수버퍼
	cbDEFAULT		m_DefalutCBuffer;
	LIGHT			m_LightCBuffer;
	MATERIAL		m_MtrlCBuffer;

	//셰이더 상수 버퍼
	ID3D11Buffer* m_pCBDef;
	ID3D11Buffer* m_pCBLight;
	ID3D11Buffer* m_pCBMtrl;

	// 정점 입력레이아웃 (공유) : 셰이더별 개별운용 필요. 현재 데모에서는 공유 중.
	ID3D11InputLayout* m_pVBLayout;

	//버텍스 버퍼
	ID3D11Buffer* m_pVB;
	void* m_pVBuff;

	//인덱스 버퍼
	ID3D11Buffer* m_pIB;
	void* m_pIBuff;


	//쉐이딩을 해주는 C3D디바이스
	C3DDevice* m_pDevice;

	// D3D 관련 개체 인터페이스 포인터
	ID3D11Device* m_pDXDevice;				//DX Device
	ID3D11DeviceContext* m_pDXDC;			//DX DC
public:
	//C3DShader();
	C3DShader(C3DDevice* pDevice);
	~C3DShader();

	void CreateVB(void* pBuff, UINT size, ID3D11Buffer** ppVB);
	void CreateIB(void* pBuff, UINT size, ID3D11Buffer** ppIB);

	//임시 행렬 설정 메소드. (world matrix 정보 업데이트 때 사용)
	void SetTransform(DWORD type, XMMATRIX& rTM);
	//임시 행렬 설정 메소드. (view, proj matrix 정보 업데이트 때 사용)
	void SetViewProjMatrix();
	
	void UpdateTempCB();

	void SetWorldTM(C3DTransform* pWorldTransform);
	//임시 카메라 테스트용 함수 #수정
	void SetCam(C3DCamera* pCam);
	//임시 라이트 테스트용 함수 #수정
	void SetLight(C3DLight* pCam);
	//임시 머테리얼 테스트용 함수 #수정
	void SetMaterial(C3DMaterial* pMtrl);

	//void SetConstBufferObj(con);

	//void ShaderSetup(TCHAR* pShaderFileName);

	void ShaderSetup(TCHAR* pShaderFileName, SHADERTYPE shaderTpye);

	//Shader 객체와 상수버퍼를 나누어서 업데이트
	void ShaderUpdate();
	void UpdateConstantBuffer();

	void ShaderRelease();
	
	//int CreateVBLayout();

	int C3DShader::CreateVBLayout(SHADERTYPE shaderType);
	//int CreateVBLayoutCVtx();
	
	//IA에 정점 버퍼 set
	//void SetVBtoIASimple(ID3D11Buffer* pVB, D3D_PRIMITIVE_TOPOLOGY topology);

	void SetVBtoIASimple(ID3D11Buffer* pVB, D3D_PRIMITIVE_TOPOLOGY topology, UINT stride);
	//인덱스 없이 그리기
	void DrawFullVtx(int vtxCnt);

	//상수 버퍼 업데이트는 이제 쉐이더에서만 protected 접근으로 변경 #수정
	//(정적) 상수 버퍼 생성.
	HRESULT CreateConstantBuffer(UINT size, ID3D11Buffer** ppCB);
	//동적 상수버퍼 생성.
	HRESULT CreateDynamicConstantBuffer(UINT size, LPVOID pData, ID3D11Buffer** ppCB);
	//동적 상수버퍼 갱신.
	HRESULT UpdateDynamicConstantBuffer(ID3D11DeviceContext* pDXDC, ID3D11Resource* pBuff, LPVOID pData, UINT size);

protected:
	int _CreateVBLayout();
	int _CreateLineVBLayout();

	HRESULT _ShaderLoad(TCHAR* fxname, ID3D11VertexShader** ppVS, ID3D11PixelShader** ppPS, ID3DBlob** ppCode);
	HRESULT _VertexShaderLoad(TCHAR* fxname, char* entry, char* target, ID3D11VertexShader** ppVS, ID3DBlob** ppCode /*= NULL*/);
	HRESULT _PixelShaderLoad(TCHAR* fxname, char* entry, char* target, ID3D11PixelShader** ppPS);
	HRESULT _ShaderCompile(
		WCHAR* FileName,		//소스파일이름.
		char* EntryPoint,		//메인함수 진입점.
		char* ShaderModel,		//셰이더 모델.
		ID3DBlob** ppCode		//[출력] 컴파일된 셰이더 코드.
	);
};

