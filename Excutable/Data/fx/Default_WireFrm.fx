// �⺻ �������
cbuffer cbDEFAULT : register(b0)
{
    matrix mTM;     //���� ���. 
    matrix mView;   //�� ��ȯ ���. 
    matrix mProj;   //���� ��ȯ ���. 
    matrix mWV;     //����-�� ��ȯ ���. 
 	matrix mWVP;
};


//VS ��� ����ü.
struct VSOutput
{
    float4 pos  : SV_POSITION;
    float4 diff : COLOR0;
};


VSOutput VS_Main(
				  float4 pos : POSITION     //[�Է�] ������ǥ. Vertex Position (Model Space, 3D)
				  //float4 nrm : NORMAL      //[�Է�] ��� normal 
				)
{
    
	//���� ��ȯ.(World Transform) 
    pos.w = 1;
    pos = mul(pos, mWVP);
 
    
    //���� ����.
    //...   
    

    //���� ���.
    VSOutput o = (VSOutput) 0;
    o.pos = pos; 
    o.diff = float4(1, 1, 1, 1);        //��� ���.

    return o;
}

float4 PS_Main(
				float4 pos : SV_POSITION,   //[�Է�] (������) �ȼ��� ��ǥ. (Screen, 2D)
				float4 diff : COLOR0        //[�Է�] (������) �ȼ��� ����. (Pixel Color : "Diffuse")
				) : SV_TARGET               //[���] ����.(�ʼ�), "����Ÿ��" ���� ����մϴ�.
{
	//Ư���� ��ȯ ���� �Էµ� ������ �״�� ����մϴ�.
    //...
     
    return diff;
}
