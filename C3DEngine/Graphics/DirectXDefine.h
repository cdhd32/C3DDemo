#pragma once
//Windows 헤더
//#include <Windows.h>

// DX 11 표준 헤더 
#include <D3D11.h>
#pragma comment(lib, "D3D11.lib")
//#include "DXGI.h"					//DXGI 헤더. (d3d11.h 에 포함됨)
#pragma comment(lib, "dxgi")
#include "d3dcompiler.h"				//DX 셰이더 컴파일러 헤더.
#pragma comment(lib, "d3dcompiler")		//DX 셰이더 컴파일러 라이브러리.  D3DCompiler.dll 필요.

// DX 수학 라이브러리
#include <DirectXMath.h>

// 모든 함수가 같은 DirectX namespace로 묶여있음, using 문 
// http://blog.naver.com/PostView.nhn?blogId=sorkelf&logNo=220846504837

using namespace DirectX;

//미리 빌드한 DirectXTK 정적라이브러리 파일(.lib) 사용을 위한 추가 정의 헤더파일
//DXTK 소스 단위로 빌드 및 디버깅을 하려면 밑의 전처리문을 주석 처리 하시면 됩니다.
//그리고 DXTK 프로젝트에서 빌드되는 lib파일 안쓰게 설정해야함
//#include <DirectXTK.h>


//DirectXTK 의 텍스처 API 사용.
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

//DirectXTK SimpleMath API
#include "SimpleMath.h"

// 주의 : Ray처럼 단순한 이름으로 선언 되어있는 식별자가 있으므로
// 식별자가 중복이 될 수도 있음
using namespace DirectX::SimpleMath;

//나중에 input 클래스에서 사용 #수정
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