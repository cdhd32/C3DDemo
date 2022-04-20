// Default.fx : �⺻ ���̴� �ҽ�.


// ��� ����
cbuffer cbDEFAULT : register(b0)
{
    matrix mTM;     //���� ���. 
    matrix mView;   //�� ��ȯ ���. 
    matrix mProj;   //���� ��ȯ ���. 
    matrix mWV;     //����-�� ��ȯ ���. 
    matrix mWVP;		//����-��-���� ��ȯ ���.
};

//���� ���� ����ü
struct LIGHT
{
    float3 Direction;   //���� ����.
    float4 Diffuse;     //�� ���� : Ȯ�걤 Diffuse Light.
    float4 Ambient;     //���� ���� : �ֺ��� Ambient Light.
    float4 Specular;    //���ݻ� ���� : Specular Light.
    float  Range;       //�� ���� �Ÿ�.
    bool   bDiffOn;     //���� ���뿩��.
    bool   bSpecOn;     //���ݻ� ���뿩��.
};

//���� ���� ����ü
struct MATERIAL
{
    float4 Diffuse;     //Ȯ�걤 �ݻ���(%) 
    float4 Ambient;     //�ֺ��� �ݻ���(%) 
    float4 Specular;    //���ݻ籤 �ݻ���(%)
    float  Power;       //���ݻ� ������ : "��ĥ��" 
};

//���� ������ �������
cbuffer cbLIGHT : register(b1)
{
    LIGHT g_Lit;
};


//���� ������ �������
cbuffer cbMATERIAL : register(b2)
{
    MATERIAL g_Mtrl;
};

//VS ��� ����ü.
struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 diff : COLOR0;
    float4 spec : COLOR1;
    
};

float4 DiffLight(float4 nrm);
float4 SpecLight(float4 pos, float4 nrm);

VSOutput VS_Main(
    float4 pos : POSITION,    //[�Է�] ������ǥ. Vertex Position (Model Space, 3D)
    float4 nrm : NORMAL,       //[�Է�] ��� normal 
    float2 uv : TEXCOORD0    //[�Է�] �ؽ�ó ��ǥ Texture Coordiates.
)
{
    //* �Ʒ��� �׽�Ʈ�� �����ϱ� ����  
    //* VS �� ��� ���۰� �����Ǿ� �־�� �մϴ�.    
    pos.w = 1.f;

    //���� ��ȯ.(World Transform) 
    pos = mul(pos, mTM);        //pos = pos * mTM

    //�þ�-�� ��ȯ (View Transform)
    pos = mul(pos, mView);
    
    //���� ��� 1 (Diffuse Lighting)
    float4 diff = 1;
    diff = DiffLight(nrm);

    //���� ��� 2 (Specular Lighting) 
    float4 spec = 1;
    spec = SpecLight(pos, nrm);


    //���İ��� diff ��������
    diff.a = g_Mtrl.Diffuse.a;

    //���� ���� ��ȯ (Projection Transform)
    pos = mul(pos, mProj);

    //WVP��ȯ.
    //pos = mul(pos, mWVP);

    //���� ���.
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
    //��»� ����.
    //float4 col = diff + spec;
    float4 col = diff;

    //���İ��� diff ��������.
    col.a = diff.a;

    return col;
}

float4 DiffLight(float4 nrm)
{
    float4 N = nrm;
    N.w = 0;
    float4 L = float4(g_Lit.Direction, 0);

    //��������� ������ ��ȯ.
    N = mul(N, mWV);
    L = mul(L, mView);

    //�� ���� ��ֶ�����.
    N = normalize(N);
    L = normalize(L);


    //���� ��� 
    float4 diff = max(dot(N, L), 0) * g_Lit.Diffuse * g_Mtrl.Diffuse;
    float4 amb = g_Lit.Ambient * g_Mtrl.Ambient;


    //��ȭ�� ó�� : ������ ���� 1�� �����մϴ�.
    //return saturate(diff + amb);

    return diff + amb;
}


float4 SpecLight(float4 pos, float4 nrm)
{
    float4 N = nrm;
    N.w = 0;
    float4 L = float4(g_Lit.Direction, 0);

    //��������� ������ ��ȯ.
    N = mul(N, mWV);
    L = mul(L, mView);

    //�ü����� ���.
    float4 E = normalize(-pos);

    //�������� ���.
    float4 H = normalize(L + E);

    //���� ��� 
    float4 spec = pow(max(dot(H, N), 0), g_Mtrl.Power) * g_Lit.Specular * g_Mtrl.Specular;
    spec.w = 1;

    return saturate(spec);
}