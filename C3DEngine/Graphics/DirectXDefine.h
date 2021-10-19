#pragma once
//Windows ���
//#include <Windows.h>

// DX 11 ǥ�� ��� 
#include <D3D11.h>
#pragma comment(lib, "D3D11.lib")
//#include "DXGI.h"					//DXGI ���. (d3d11.h �� ���Ե�)
#pragma comment(lib, "dxgi")
#include "d3dcompiler.h"				//DX ���̴� �����Ϸ� ���.
#pragma comment(lib, "d3dcompiler")		//DX ���̴� �����Ϸ� ���̺귯��.  D3DCompiler.dll �ʿ�.

// DX ���� ���̺귯��
#include <DirectXMath.h>

// ��� �Լ��� ���� DirectX namespace�� ��������, using �� 
// http://blog.naver.com/PostView.nhn?blogId=sorkelf&logNo=220846504837

using namespace DirectX;

//�̸� ������ DirectXTK �������̺귯�� ����(.lib) ����� ���� �߰� ���� �������
//DXTK �ҽ� ������ ���� �� ������� �Ϸ��� ���� ��ó������ �ּ� ó�� �Ͻø� �˴ϴ�.
//�׸��� DXTK ������Ʈ���� ����Ǵ� lib���� �Ⱦ��� �����ؾ���
//#include <DirectXTK.h>


//DirectXTK �� �ؽ�ó API ���.
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

//DirectXTK SimpleMath API
#include "SimpleMath.h"

// ���� : Rayó�� �ܼ��� �̸����� ���� �Ǿ��ִ� �ĺ��ڰ� �����Ƿ�
// �ĺ��ڰ� �ߺ��� �� ���� ����
using namespace DirectX::SimpleMath;

//���߿� input Ŭ�������� ��� #����
#include <dinput.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "../Objects/C3DAlignedAllocationPolicy.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(pBuff) if((pBuff)){ (pBuff)->Release(); (pBuff) = NULL; }
#define SAFE_DELETE(pBuff)	if((pBuff)){ delete (pBuff); (pBuff) = NULL; }
#define SAFE_DELARRY(pBuff) if((pBuff)){ delete [] (pBuff); (pBuff) = NULL; }
#endif

#ifndef IsKeyDown
#define IsKeyDown(key)	((GetAsyncKeyState(key)&0x8000) == 0x8000)
#define IsKeyUp(key)	((GetAsyncKeyState(key)&0x8001) == 0x8001)
#endif 

#ifndef YES_
#define YES_ TRUE
#define NO_  FALSE
#endif