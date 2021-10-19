#pragma once
#include <Windows.h>
#include <cassert>
#include <string>

#include "Resource.h"

#include "DemoScene.h"

#include "MainWindow.h"

MainWindow* MainWindow::m_pInst = nullptr;

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �뵵: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���ø����̼� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
		assert(-1); //assert ���� ������
		return false;
	}

	//���� �� �ʱ�ȭ�� ���⼭
	//DXInitialize
	//m_pDevice = new C3DDevice();
	//m_pDevice->Initialize(m_hWnd);

	//m_pRenderer = new C3DRenderer();
	//m_pRenderer->Initialize(m_hWnd); //���� �ڵ� ����
	//m_pRenderer->DataLoading();

	m_pScene = new DemoScene();
	m_pScene->Initialize(L"TestStage", m_hWnd);

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

///////////////////////////////////////////////////////////////////////////////
//// 
//// ���� �Լ�.
////
//int APIENTRY WinMain(HINSTANCE hInstance,
//	HINSTANCE hPrevInstance,
//	LPSTR     lpCmdLine,
//	int       nCmdShow)
//{
//
//	//------------------------------------------------------------------//
//	// �ܼ� �¾�.
//	//------------------------------------------------------------------//
//	if (B3Console::_bUse)
//	{
//		B3Console::Alloc();
//		B3Console::_x = 450;	B3Console::_y = 400;
//		B3Console::Move();
//	}
//
//	//------------------------------------------------------------------//
//	// �⺻���� ������ ������ ����..                                    //
//	//------------------------------------------------------------------//
//	if (!InitWindow(960, 600)) return 0;
//
//
//	//------------------------------------------------------------------//
//	// D3D ����															//
//	//------------------------------------------------------------------//
//	if (FAILED(InitialD3D(g_hWnd)))
//		return 0;
//
//	//------------------------------------------------------------------//
//	// ���� ������ �ε�                                                //
//	//------------------------------------------------------------------//
//	if (FAILED(LoadData()))
//	{
//		g_bLoop = FALSE;
//	}
//
//
//	//------------------------------------------------------------------//
//	// ����  ����														//
//	//------------------------------------------------------------------//
//	while (g_bLoop)
//	{
//		if (!MessagePump())		// �޼��� ����.
//			break;
//
//		//DInput ������Ʈ.��
//		B3Input_CheckInput();
//		if (IsKeyDown(DIK_ESCAPE)) break;
//
//
//		/* ���� ���� ���� */
//		Render();
//
//	}
//
//	//------------------------------------------------------------------//
//	// ���ø����̼� �����, ���� �޸𸮸� �����Ѵ�.						//	
//	//------------------------------------------------------------------//
//	ReleaseData();
//	ReleaseD3D();
//	B3Error(NO_, "[info] ���ø����̼� ����..");
//
//	//�ܼ� ����.
//	if (B3Console::_bUse) B3Console::Free();
//
//
//	return 0;
//}

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

			m_pScene->FixedUpdate();
			m_pScene->Update();
			m_pScene->Render();

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