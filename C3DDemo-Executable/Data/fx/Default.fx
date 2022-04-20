// Default.fx : 기본 셰이더 소스.


// 상수 버퍼
cbuffer cbDEFAULT : register(b0)
{
    matrix mTM;     //월드 행렬. 
    matrix mView;   //뷰 변환 행렬. 
    matrix mProj;   //투영 변환 행렬. 
    matrix mWV;     //월드-뷰 변환 행렬. 
    matrix mWVP;		//월드-뷰-투영 변환 행렬.
};

//조명 정보 구조체
struct LIGHT
{
    float3 Direction;   //빛의 방향.
    float4 Diffuse;     //주 광량 : 확산광 Diffuse Light.
    float4 Ambient;     //보조 광량 : 주변광 Ambient Light.
    float4 Specular;    //정반사 광량 : Specular Light.
    float  Range;       //빛 도달 거리.
    bool   bDiffOn;     //조명 적용여부.
    bool   bSpecOn;     //정반사 적용여부.
};

//재질 정보 구조체
struct MATERIAL
{
    float4 Diffuse;     //확산광 반사율(%) 
    float4 Ambient;     //주변광 반사율(%) 
    float4 Specular;    //정반사광 반사율(%)
    float  Power;       //정반사 보정값 : "거칠기" 
};

//조명 정보용 상수버퍼
cbuffer cbLIGHT : register(b1)
{
    LIGHT g_Lit;
};


//재질 정보용 상수버퍼
cbuffer cbMATERIAL : register(b2)
{
    MATERIAL g_Mtrl;
};

//VS 출력 구조체.
struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 diff : COLOR0;
    float4 spec : COLOR1;
    
};

float4 DiffLight(float4 nrm);
float4 SpecLight(float4 pos, float4 nrm);

VSOutput VS_Main(
    float4 pos : POSITION,    //[입력] 정점좌표. Vertex Position (Model Space, 3D)
    float4 nrm : NORMAL,       //[입력] 노멀 normal 
    float2 uv : TEXCOORD0    //[입력] 텍스처 좌표 Texture Coordiates.
)
{
    //* 아래의 테스트를 수행하기 전에  
    //* VS 에 상수 버퍼가 설정되어 있어야 합니다.    
    pos.w = 1.f;

    //월드 변환.(World Transform) 
    pos = mul(pos, mTM);        //pos = pos * mTM

    //시야-뷰 변환 (View Transform)
    pos = mul(pos, mView);
    
    //조명 계산 1 (Diffuse Lighting)
    float4 diff = 1;
    diff = DiffLight(nrm);

    //조명 계산 2 (Specular Lighting) 
    float4 spec = 1;
    spec = SpecLight(pos, nrm);


    //알파값은 diff 성분유지
    diff.a = g_Mtrl.Diffuse.a;

    //원근 투영 변환 (Projection Transform)
    pos = mul(pos, mProj);

    //WVP변환.
    //pos = mul(pos, mWVP);

    //정보 출력.
    VSOutput o = (VSOutput)0;
    o.pos = pos;
    o.diff = diff;
    o.spec = spec;

    return o;
}

float4 PS_Main(
    float4 pos  : SV_POSITION,
    float4 diff : COLOR0,
    float4 spec : COLOR1
) : SV_TARGET
{
    //출력색 결정.
    //float4 col = diff + spec;
    float4 col = diff;

    //알파값은 diff 성분유지.
    col.a = diff.a;

    return col;
}

float4 DiffLight(float4 nrm)
{
    float4 N = nrm;
    N.w = 0;
    float4 L = float4(g_Lit.Direction, 0);

    //뷰공간으로 정보를 변환.
    N = mul(N, mWV);
    L = mul(L, mView);

    //각 벡터 노멀라이즈.
    N = normalize(N);
    L = normalize(L);


    //조명 계산 
    float4 diff = max(dot(N, L), 0) * g_Lit.Diffuse * g_Mtrl.Diffuse;
    float4 amb = g_Lit.Ambient * g_Mtrl.Ambient;


    //포화도 처리 : 광량의 합을 1로 제한합니다.
    //return saturate(diff + amb);

    return diff + amb;
}


float4 SpecLight(float4 pos, float4 nrm)
{
    float4 N = nrm;
    N.w = 0;
    float4 L = float4(g_Lit.Direction, 0);

    //뷰공간으로 정보를 변환.
    N = mul(N, mWV);
    L = mul(L, mView);

    //시선백터 계산.
    float4 E = normalize(-pos);

    //하프벡터 계산.
    float4 H = normalize(L + E);

    //조명 계산 
    float4 spec = pow(max(dot(H, N), 0), g_Mtrl.Power) * g_Lit.Specular * g_Mtrl.Specular;
    spec.w = 1;

    return saturate(spec);
}