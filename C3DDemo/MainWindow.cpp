#pragma once
#include <Windows.h>
#include <cassert>
#include <string>

#include "Resource.h"

#include "DemoScene.h"

#include "MainWindow.h"

MainWindow* MainWindow::m_pInst = nullptr;

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZING:
		break;
	case WM_SIZE:
		//SIZE size;
		//size.cx = LOWORD(lParam);
		//size.cy = HIWORD(lParam);
		//engine->SetLogicalSize(size);
		break;
	case WM_MOVING:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

MainWindow::MainWindow()
	: m_hInstance(nullptr),
	m_hWnd(nullptr),
	m_pRenderer(nullptr) //,m_pDevice(nullptr)	, m_pRenderer(nullptr)
{

}

MainWindow::~MainWindow()
{

}

bool MainWindow::Initialize(HINSTANCE hInstance)
{
	HRESULT hr = CreateMainWindowClass(hInstance);
	if (FAILED(hr))
	{
		assert(-1); //assert 따로 만들자
		return false;
	}

	//생성 및 초기화는 여기서
	//DXInitialize
	//m_pDevice = new C3DDevice();
	//m_pDevice->Initialize(m_hWnd);

	//m_pRenderer = new C3DRenderer();
	//m_pRenderer->Initialize(m_hWnd); //윈도 핸들 전달
	//m_pRenderer->DataLoading();

	m_pScene = new DemoScene();
	m_pScene->Initialize(L"TestStage", m_hWnd);

	//Model data load


	return true;
}

//윈도우 메세지, 랜더링, 네트워크 등의 모든 처리를 하는 루프를 호출
bool MainWindow::Run()
{
	if (FAILED(MainWindowLoop()))
	{
		return false;
	}

	return true;
}

bool MainWindow::Release()
{
	//메모리 해제는 여기서
	//m_pRenderer->DataRelease();

	MainWindow::~MainWindow();
	return true;
}

///////////////////////////////////////////////////////////////////////////////
//// 
//// 메인 함수.
////
//int APIENTRY WinMain(HINSTANCE hInstance,
//	HINSTANCE hPrevInstance,
//	LPSTR     lpCmdLine,
//	int       nCmdShow)
//{
//
//	//------------------------------------------------------------------//
//	// 콘솔 셋업.
//	//------------------------------------------------------------------//
//	if (B3Console::_bUse)
//	{
//		B3Console::Alloc();
//		B3Console::_x = 450;	B3Console::_y = 400;
//		B3Console::Move();
//	}
//
//	//------------------------------------------------------------------//
//	// 기본적인 윈도우 프레임 생성..                                    //
//	//------------------------------------------------------------------//
//	if (!InitWindow(960, 600)) return 0;
//
//
//	//------------------------------------------------------------------//
//	// D3D 설정															//
//	//------------------------------------------------------------------//
//	if (FAILED(InitialD3D(g_hWnd)))
//		return 0;
//
//	//------------------------------------------------------------------//
//	// 게임 데이터 로딩                                                //
//	//------------------------------------------------------------------//
//	if (FAILED(LoadData()))
//	{
//		g_bLoop = FALSE;
//	}
//
//
//	//------------------------------------------------------------------//
//	// 메인  루프														//
//	//------------------------------------------------------------------//
//	while (g_bLoop)
//	{
//		if (!MessagePump())		// 메세지 펌프.
//			break;
//
//		//DInput 업데이트.★
//		B3Input_CheckInput();
//		if (IsKeyDown(DIK_ESCAPE)) break;
//
//
//		/* 이하 게임 로직 */
//		Render();
//
//	}
//
//	//------------------------------------------------------------------//
//	// 어플리케이션 종료시, 관련 메모리를 제거한다.						//	
//	//------------------------------------------------------------------//
//	ReleaseData();
//	ReleaseD3D();
//	B3Error(NO_, "[info] 어플리케이션 종료..");
//
//	//콘솔 제거.
//	if (B3Console::_bUse) B3Console::Free();
//
//
//	return 0;
//}

//위에 함수를 참고하여 순서대로 각 함수별로 필요한 기능/함수들을 추가 구현, dx 초기화과정
HRESULT MainWindow::MainWindowLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//윈도우 메시지가 없을 때
			//랜더링, 인게임 처리는 여기서.. #RUN

			//RENDERING -> 메인스레드에서 랜더링처리
			//m_pRenderer->SceneUpdate();
			//m_pRenderer->SceneRender();

			m_pScene->FixedUpdate();
			m_pScene->Update();
			m_pScene->Render();

			//ETC

			
		}
	}

	return (int)msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MainWindow::CreateMainWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	//윈도우 클래스 이름
	WCHAR className[30] = L"3D Engine Test";

	wcex.cbSize = sizeof(WNDCLASSEXW);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION); //MAKEINTRESOURCE(IDI_MY3DENGINE)
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH); //(HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = className; 
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	m_hInstance = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	m_hWnd = CreateWindow(className, className, WS_OVERLAPPEDWINDOW,
		60, 30, g_Mode.Width, g_Mode.Height, NULL, NULL, hInstance, NULL);

	if (!m_hWnd)
	{
		return FALSE;
	}

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	SetFocus(m_hWnd);

	return TRUE;
}