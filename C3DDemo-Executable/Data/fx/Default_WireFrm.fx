// 기본 상수버퍼
cbuffer cbDEFAULT : register(b0)
{
    matrix mTM;     //월드 행렬. 
    matrix mView;   //뷰 변환 행렬. 
    matrix mProj;   //투영 변환 행렬. 
    matrix mWV;     //월드-뷰 변환 행렬. 
 	matrix mWVP;
};


//VS 출력 구조체.
struct VSOutput
{
    float4 pos  : SV_POSITION;
    float4 diff : COLOR0;
};


VSOutput VS_Main(
				  float4 pos : POSITION     //[입력] 정점좌표. Vertex Position (Model Space, 3D)
				  //float4 nrm : NORMAL      //[입력] 노멀 normal 
				)
{
    
	//정점 변환.(World Transform) 
    pos.w = 1;
    pos = mul(pos, mWVP);
 
    
    //조명 없음.
    //...   
    

    //정보 출력.
    VSOutput o = (VSOutput) 0;
    o.pos = pos; 
    o.diff = float4(1, 1, 1, 1);        //흰색 출력.

    return o;
}

float4 PS_Main(
				float4 pos : SV_POSITION,   //[입력] (보간된) 픽셀별 좌표. (Screen, 2D)
				float4 diff : COLOR0        //[입력] (보간된) 픽셀별 색상. (Pixel Color : "Diffuse")
				) : SV_TARGET               //[출력] 색상.(필수), "렌더타겟" 으로 출력합니다.
{
	//특별한 변환 없이 입력된 정보를 그대로 출력합니다.
    //...
     
    return diff;
}
