#pragma once
#include <Windows.h>
#include <cassert>
#include <string>

#include "Resource.h"

#include "DemoScene.h"
#include "PhysicsEngine.h"
#include "System/C3DTimer.h"

#include "MainWindow.h"

MainWindow* MainWindow::m_pInst = nullptr;

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
	m_hWnd(nullptr)
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
	m_pDevice = std::make_unique<C3DDevice>();
	m_pDevice->Initialize(m_hWnd);

	m_pScene = new DemoScene();
	PhysicsEngine::PhysicsEngineMain::GetInstance().Initialize();
	m_pScene->Initialize(L"TestStage", m_hWnd, m_pDevice.get());

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

			//매 업데이트 시 dTime을 가져와서 누적시킨다
			float dTime = C3DTimer::GetInstanse()->GetDeltaTime();

			static float accumTime = 0;
			accumTime += dTime;

			if (accumTime > (1.f / 60.f))
			{
				m_pScene->FixedUpdate(accumTime);
				accumTime = accumTime - (1.f / 60.f);
			}

			PhysicsEngine::PhysicsEngineMain::GetInstance().Update();
			m_pScene->Update();
			//랜더 상태 업테이트
			m_pDevice->RenderStateUpdate();
			//백버퍼 클리어
			m_pDevice->RTClear(C3DDevice::CLEAR_COLOR_DBLUE);
			m_pScene->Render();
			//디바이스 화면 출력
			m_pDevice->Flip();
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