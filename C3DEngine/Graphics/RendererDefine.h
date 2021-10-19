#pragma once

// ����/���ٽ� �׽�Ʈ ���µ�.
//
enum {
	DS_DEPTH_ON,		//���̹��� ON! (�⺻��), ���ٽǹ��� OFF.
	DS_DEPTH_OFF,		//���̹��� OFF!, ���ٽ� OFF.
	DS_DEPTH_WRITE_OFF,	//���̹��� ���� ����, ���ٽ� OFF. 

	//���ٽ� ���� �����.
	DS_DEPTH_ON_STENCIL_ON,				//���̹��� On (Write On), ���ٽǹ��� ON (������ ����) : "����/���ٽ� ���".
	DS_DEPTH_ON_STENCIL_EQUAL_KEEP,		//���̹��� On (Write On), ���ٽǹ��� ON (���Ϻ�, ������ ����) : "���� ��ġ���� �׸���".
	DS_DEPTH_ON_STENCIL_NOTEQUAL_KEEP,	//���̹��� On (Write On), ���ٽǹ��� ON (�ٸ���, ������ ����) : "���� ��ġ �̿ܿ� �׸���".

	//DS_DEPTH_OFF_STENCIL_ON,			//���̹��� Off, ���ٽǹ��� ON (������ ����) : "���ٽǸ� ���"
	//DS_DEPTH_ON_STENCIL_EQUAL_INCR,	//���̹��� On (Write On), ���ٽǹ��� ON (���Ϻ�, ������ ����) : "���߱׸��� ����".
	DS_DEPTH_WRITE_OFF_STENCIL_ON,		//���̹��� On (Write Off), ���ٽǹ��� ON (�׻���, ������ ����) :" ���ٽǸ� ���".
	DS_DEPTH_OFF_STENCIL_EQUAL_INCR,	//���̹��� Off (Write Off), ���ٽǹ��� ON (���Ϻ�, ������ ����) : "���߱׸��� ����.


	//���� ��� �ִ밪.
	DS_MAX_,

	//��� ������. MAX �� ���ʿ� �����ؾ� ��. ����.
	DS_DEPTH_ON_STENCIL_OFF = DS_DEPTH_ON,			//���̹��� ON, ���ٽǹ��� OFF
	DS_DEPTH_ONLY = DS_DEPTH_ON_STENCIL_OFF,		//���̹��� ON, ���ٽǹ��� OFF
	//DS_STENCIL_ONLY = DS_DEPTH_OFF_STENCIL_ON,	//���̹��� OFF, ���ٽǹ��� ON
};

// �����Ͷ����� ���� ��ü Rasterizer State Objects
// 
enum {
	RS_SOLID,				//�ﰢ�� ä��� : Fill Mode - Soild.
	RS_WIREFRM,				//�ﰢ�� ä��� : Fill Mode - Wireframe.
	RS_CULLBACK,			//�޸� �ø� (ON) : Back-Face Culling - "CCW" 
	RS_CULLFRONT,			//���� �ø� (ON) : Front-Face Culling - "CW" 	
	RS_WIRECULLBACK,		//���̾� ������ + �޸� �ø� (ON) 
	RS_WIRECULLFRONT,		//���̾� ������ + �ո� �ø� (ON) 	

	//���� ��� �ִ밪.
	RS_MAX_,

	//��� ������. MAX �� ���ʿ� �����ؾ� ��. ����.
	RS_CULL_CCW = RS_CULLBACK,
	RS_CULL_CW = RS_CULLFRONT,
	RS_WIRE_CULL_CCW = RS_WIRECULLBACK,
	RS_WIRE_CULL_CW = RS_WIRECULLFRONT,

};

// ������ ��� : �ټ��� ������ ���� ���� �� ����� ���� ����.
//
enum {
	RM_SOLID = 0x0000,		// �ﰢ��ä��� : ON, Solid
	RM_WIREFRAME = 0x0001,		// �ﰢ��ä��� : OFF, Wire-frame
	RM_CULLBACK = 0x0002,		// �޸� �ø� : ON, "CCW"

	//������ �⺻��� : Solid + Cull-On.
	RM_DEFAULT = RM_SOLID | RM_CULLBACK,

};

// ����/���� ȥ�� ���� ��ü Blending State Objects 
//
enum {
	BS_DEFAULT,				//�⺻ȥ�� ��ü.
	BS_ALPHA_BLEND,			//���� ����. (Src.a + (1-Src.a))
	BS_COLOR_BLEND,			//���� ����. ( Src.Color + (1-Src.Color))
	BS_COLOR_BLEND_ONE,		//1:1 ����. ( Src.Color + Dest.Color) 

	BS_MAX_,

	BS_AB_OFF = BS_DEFAULT,
	BS_AB_ON = BS_ALPHA_BLEND,
	//BS_AB_ALPHA = BS_ALPHA_BLEND,
	//BS_AB_COLOR = BS_COLOR_BLEND,
};

//����/���� ȥ�� ���� ��ü 

// �ؽ�ó ���÷� ���� ��ü : Texture Sampler State Objects.
//
enum {
	SS_CLAMP,			//�ؽ�ó ��巹�� ��� : Ŭ���� (DX �⺻��) 
	SS_WRAP,			//�ؽ�ó ��巹�� ��� : ���� 
	SS_BORDER,
	//SS_MIRROR,
	//SS_MIRRROONCE,

	SS_MAX,
	SS_DEFAULT = SS_WRAP,	//�⺻ ���÷�

};