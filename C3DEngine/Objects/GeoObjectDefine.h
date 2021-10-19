#pragma once
#include "../pch.h"

//PrimitiveGeoObject
//�⺻ 3���� ���ϵ��� �⺻ ����, ���ؽ� ���� ���� �� �����ϴ� ��
//Line(��, ����׿� �븻 ǥ�ó� grid �׸� �� ���),

//D3D11_PRIMITIVE_TOPOLOGY_LINELIST�� �׸� �� ���

struct VB_POS
{
	float x, y, z; 			//��ǥ(Position)
};

enum
{
	LINE_START,
	LINE_END,

	_LINE_MAX
};
//3D Line ���۰� �� ���� ����
struct LINE
{
	VB_POS vtx[2];
};

//Ǯ ���ؽ� ����� ���� ���� position�� ��ĥ ���� ����
struct VB_DEFAULT
{
	float x, y, z; 			//��ǥ(Position)
	float nx, ny, nz;		//��� : Normal
	float u, v;				//�ؽ�ó ��ǥ: Texture-Coordinates 
};

//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST�� drawindexed �� �� �ε��� ���۷� ���
enum
{
	FACE_A,
	FACE_B,
	FACE_C,

	_FACE_MAX
};
struct IB_FACE {
	Vector3 vtx[3];
};

//Cube fullvtx model(��� ť�� �׽�Ʈ ��)
//�⺻ ���ؽ� ���� ����(3d ��ǥ, ��� ����, �ؽ�ó ��ǥ UV)
//�ε��� ���� ���� Draw�� �׸��� �˴ϴ�.
static constexpr VB_DEFAULT CUBE_VTX_FULL[] = {
	//( Face#0)
	//pos				normal			uv
	{ -1.0, 1.0,-1.0 ,  0.0, 0.0,-1.0,  0.0, 0.0 },		//v0	
	{  1.0, 1.0,-1.0 ,  0.0, 0.0,-1.0,  1.0, 0.0 },		//v1
	{ -1.0,-1.0,-1.0 ,  0.0, 0.0,-1.0,  0.0, 1.0 },		//v2
	//( Face#1)
	{ -1.0,-1.0,-1.0 ,  0.0, 0.0,-1.0 ,  0.0, 1.0 },
	{  1.0, 1.0,-1.0 ,  0.0, 0.0,-1.0 ,  1.0, 0.0 },
	{  1.0,-1.0,-1.0 ,  0.0, 0.0,-1.0 ,  1.0, 1.0 },

	//�޸�.  (Face#3) :  
	{  1.0, 1.0, 1.0 ,  0.0, 0.0, 1.0,  0.0, 0.0 },
	{ -1.0, 1.0, 1.0 ,  0.0, 0.0, 1.0,  1.0, 0.0 },
	{ -1.0,-1.0, 1.0 ,  0.0, 0.0, 1.0,  1.0, 1.0 },
	//		  (Face#4)
	{  1.0, 1.0, 1.0 ,  0.0, 0.0, 1.0,  0.0, 0.0 },
	{ -1.0,-1.0, 1.0 ,  0.0, 0.0, 1.0,  1.0, 1.0 },
	{  1.0,-1.0, 1.0 ,  0.0, 0.0, 1.0,  0.0, 1.0 },


	// ������. (Face#5)
	{  1.0, 1.0,-1.0 ,  1.0, 0.0, 0.0,  0.0, 0.0 },
	{  1.0, 1.0, 1.0 ,  1.0, 0.0, 0.0,  1.0, 0.0 },
	{  1.0,-1.0,-1.0 ,  1.0, 0.0, 0.0,  0.0, 1.0 },
	//			(Face#6)
	{  1.0,-1.0,-1.0 ,  1.0, 0.0, 0.0,  0.0, 1.0 },
	{  1.0, 1.0, 1.0 ,  1.0, 0.0, 0.0,  1.0, 0.0 },
	{  1.0,-1.0, 1.0 ,  1.0, 0.0, 0.0,  1.0, 1.0 },


	// ������. (Face#7)
	{ -1.0, 1.0, 1.0 ,  -1.0, 0.0, 0.0,  0.0, 0.0 },
	{ -1.0, 1.0,-1.0 ,  -1.0, 0.0, 0.0,  1.0, 0.0 },
	{ -1.0,-1.0,-1.0 ,  -1.0, 0.0, 0.0,  1.0, 1.0 },
	//			(Face#8)
	{ -1.0, 1.0, 1.0 ,  -1.0, 0.0, 0.0,  0.0, 0.0 },
	{ -1.0,-1.0,-1.0 ,  -1.0, 0.0, 0.0,  1.0, 1.0 },
	{ -1.0,-1.0, 1.0 ,  -1.0, 0.0, 0.0,  0.0, 1.0 },


	//���.  ( Face#9)
	{ -1.0, 1.0, 1.0 ,  0.0, 1.0, 0.0,  0.0, 0.0 },
	{  1.0, 1.0, 1.0 ,  0.0, 1.0, 0.0,  1.0, 0.0 },
	{ -1.0, 1.0,-1.0 ,  0.0, 1.0, 0.0,  0.0, 1.0 },
	//		 ( Face#10)
	{ -1.0, 1.0,-1.0 ,  0.0, 1.0, 0.0,  0.0, 1.0 },
	{  1.0, 1.0, 1.0 ,  0.0, 1.0, 0.0,  1.0, 0.0 },
	{  1.0, 1.0,-1.0 ,  0.0, 1.0, 0.0,  1.0, 1.0 },


	//�Ϻ�.  ( Face#11)
	{  1.0,-1.0, 1.0 , 0.0,-1.0, 0.0,  0.0, 0.0 },
	{ -1.0,-1.0, 1.0 , 0.0,-1.0, 0.0,  1.0, 0.0 },
	{ -1.0,-1.0,-1.0 , 0.0,-1.0, 0.0,  1.0, 1.0 },
	//		 ( Face#12)
	{  1.0,-1.0, 1.0 , 0.0,-1.0, 0.0,  0.0, 0.0 },
	{ -1.0,-1.0,-1.0 , 0.0,-1.0, 0.0,  1.0, 1.0 },
	{  1.0,-1.0,-1.0 , 0.0,-1.0, 0.0,  0.0, 1.0 },
};


//QuadPlane(������ �Ǵ� UI �׸���), �簢�� �� �׸���
VB_DEFAULT quadPlane[] =
{
	//pos		uv
	{ 0, 0, 0,  0, 0 },
	{ 1, 0, 0,  1, 0 },
	{ 0, 1, 0,  0, 1 },
	{ 1, 1, 0,  1, 1 },
};