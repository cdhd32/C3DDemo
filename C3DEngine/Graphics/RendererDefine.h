#pragma once

// 깊이/스텐실 테스트 상태들.
//
enum {
	DS_DEPTH_ON,		//깊이버퍼 ON! (기본값), 스텐실버퍼 OFF.
	DS_DEPTH_OFF,		//깊이버퍼 OFF!, 스텐실 OFF.
	DS_DEPTH_WRITE_OFF,	//깊이버퍼 쓰기 끄기, 스텐실 OFF. 

	//스텐실 버퍼 연산용.
	DS_DEPTH_ON_STENCIL_ON,				//깊이버퍼 On (Write On), 스텐실버퍼 ON (참조값 쓰기) : "깊이/스텐실 기록".
	DS_DEPTH_ON_STENCIL_EQUAL_KEEP,		//깊이버퍼 On (Write On), 스텐실버퍼 ON (동일비교, 성공시 유지) : "지정 위치에만 그리기".
	DS_DEPTH_ON_STENCIL_NOTEQUAL_KEEP,	//깊이버퍼 On (Write On), 스텐실버퍼 ON (다름비교, 성공시 유지) : "지정 위치 이외에 그리기".

	//DS_DEPTH_OFF_STENCIL_ON,			//깊이버퍼 Off, 스텐실버퍼 ON (참조값 쓰기) : "스텐실만 기록"
	//DS_DEPTH_ON_STENCIL_EQUAL_INCR,	//깊이버퍼 On (Write On), 스텐실버퍼 ON (동일비교, 성공시 증가) : "이중그리기 방지".
	DS_DEPTH_WRITE_OFF_STENCIL_ON,		//깊이버퍼 On (Write Off), 스텐실버퍼 ON (항상기록, 성공시 증가) :" 스텐실만 기록".
	DS_DEPTH_OFF_STENCIL_EQUAL_INCR,	//깊이버퍼 Off (Write Off), 스텐실버퍼 ON (동일비교, 성공시 증가) : "이중그리기 방지.


	//열거 상수 최대값.
	DS_MAX_,

	//상수 재정의. MAX 값 뒷쪽에 정의해야 함. 주의.
	DS_DEPTH_ON_STENCIL_OFF = DS_DEPTH_ON,			//깊이버퍼 ON, 스텐실버퍼 OFF
	DS_DEPTH_ONLY = DS_DEPTH_ON_STENCIL_OFF,		//깊이버퍼 ON, 스텐실버퍼 OFF
	//DS_STENCIL_ONLY = DS_DEPTH_OFF_STENCIL_ON,	//깊이버퍼 OFF, 스텐실버퍼 ON
};

// 레스터라이져 상태 객체 Rasterizer State Objects
// 
enum {
	RS_SOLID,				//삼각형 채우기 : Fill Mode - Soild.
	RS_WIREFRM,				//삼각형 채우기 : Fill Mode - Wireframe.
	RS_CULLBACK,			//뒷면 컬링 (ON) : Back-Face Culling - "CCW" 
	RS_CULLFRONT,			//정면 컬링 (ON) : Front-Face Culling - "CW" 	
	RS_WIRECULLBACK,		//와이어 프레임 + 뒷면 컬링 (ON) 
	RS_WIRECULLFRONT,		//와이어 프레임 + 앞면 컬링 (ON) 	

	//열거 상수 최대값.
	RS_MAX_,

	//상수 재정의. MAX 값 뒷쪽에 정의해야 함. 주의.
	RS_CULL_CCW = RS_CULLBACK,
	RS_CULL_CW = RS_CULLFRONT,
	RS_WIRE_CULL_CCW = RS_WIRECULLBACK,
	RS_WIRE_CULL_CW = RS_WIRECULLFRONT,

};

// 렌더링 모드 : 다수의 렌더링 상태 조합 및 운용을 위한 정의.
//
enum {
	RM_SOLID = 0x0000,		// 삼각형채우기 : ON, Solid
	RM_WIREFRAME = 0x0001,		// 삼각형채우기 : OFF, Wire-frame
	RM_CULLBACK = 0x0002,		// 뒷면 컬링 : ON, "CCW"

	//렌더링 기본모드 : Solid + Cull-On.
	RM_DEFAULT = RM_SOLID | RM_CULLBACK,

};

// 색상/알파 혼합 상태 객체 Blending State Objects 
//
enum {
	BS_DEFAULT,				//기본혼합 객체.
	BS_ALPHA_BLEND,			//알파 블랜딩. (Src.a + (1-Src.a))
	BS_COLOR_BLEND,			//색상 블랜딩. ( Src.Color + (1-Src.Color))
	BS_COLOR_BLEND_ONE,		//1:1 블랜딩. ( Src.Color + Dest.Color) 

	BS_MAX_,

	BS_AB_OFF = BS_DEFAULT,
	BS_AB_ON = BS_ALPHA_BLEND,
	//BS_AB_ALPHA = BS_ALPHA_BLEND,
	//BS_AB_COLOR = BS_COLOR_BLEND,
};

//색상/알파 혼합 관리 객체 

// 텍스처 셈플러 상태 객체 : Texture Sampler State Objects.
//
enum {
	SS_CLAMP,			//텍스처 어드레스 모드 : 클램핑 (DX 기본값) 
	SS_WRAP,			//텍스처 어드레스 모드 : 렙핑 
	SS_BORDER,
	//SS_MIRROR,
	//SS_MIRRROONCE,

	SS_MAX,
	SS_DEFAULT = SS_WRAP,	//기본 셈플러

};