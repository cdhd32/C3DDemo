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
		assert(-1); //assert ���� ������
		return false;
	}

	//���� �� �ʱ�ȭ�� ���⼭
	//DXInitialize
	m_pDevice = std::make_unique<C3DDevice>();
	m_pDevice->Initialize(m_hWnd);

	m_pScene = new DemoScene();
	PhysicsEngine::PhysicsEngineMain::GetInstance().Initialize();
	m_pScene->Initialize(L"TestStage", m_hWnd, m_pDevice.get());

	//Model data load

	return true;
}

//������ �޼���, ������, ��Ʈ��ũ ���� ��� ó���� �ϴ� ������ ȣ��
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
	//�޸� ������ ���⼭
	//m_pRenderer->DataRelease();

	MainWindow::~MainWindow();
	return true;
}

//���� �Լ��� �����Ͽ� ������� �� �Լ����� �ʿ��� ���/�Լ����� �߰� ����, dx �ʱ�ȭ����
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
			//������ �޽����� ���� ��
			//������, �ΰ��� ó���� ���⼭.. #RUN

			//RENDERING -> ���ν����忡�� ������ó��
			//m_pRenderer->SceneUpdate();
			//m_pRenderer->SceneRender();

			//�� ������Ʈ �� dTime�� �����ͼ� ������Ų��
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
			//���� ���� ������Ʈ
			m_pDevice->RenderStateUpdate();
			//����� Ŭ����
			m_pDevice->RTClear(C3DDevice::CLEAR_COLOR_DBLUE);
			m_pScene->Render();
			//����̽� ȭ�� ���
			m_pDevice->Flip();
			//ETC

			
		}
	}

	return (int)msg.wParam;
}

//
//  �Լ�: MyRegisterClass()
//
//  �뵵: â Ŭ������ ����մϴ�.
//
ATOM MainWindow::CreateMainWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	//������ Ŭ���� �̸�
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

	m_hInstance = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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