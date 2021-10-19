#include "ExampleGame.h"
#include "ConsoleWindow.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HEngine_DX12_3D* p3DEngine =
		reinterpret_cast<HEngine_DX12_3D*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if (p3DEngine != nullptr)
		p3DEngine->ProcessWndMessage(hWnd, message, wParam, lParam);

	switch (message)
	{
	case WM_CREATE:
	case WM_CLOSE:
	case WM_DEVICECHANGE:
	case WM_PAINT:
	case WM_SIZE:
	case WM_ENTERSIZEMOVE:
	case WM_EXITSIZEMOVE:
	case WM_GETMINMAXINFO:
	case WM_ACTIVATEAPP:
	case WM_POWERBROADCAST:
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	case WM_SYSKEYDOWN:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

HWND ExampleGame::InitWindow(HINSTANCE hInstance, int clientWidth, int clientHeight)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIconW(hInstance, L"IDI_ICON");
	wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"random name";
	wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON");

	RegisterClassExW(&wcex);

	RECT R = { 0, 0, clientWidth, clientHeight };

	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, FALSE);
	std::wstring caption = L"initialCaption";

	HWND hwnd = CreateWindowExW(0, L"random name", caption.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, R.right - R.left, R.bottom - R.top, nullptr, nullptr, hInstance,
		nullptr);

	//아래 함수로 인해 WinProc함수에 들어가더라도 엔진 초기화 전에는 엔진의 기능을 사용하지 않는다.
	ShowWindow(hwnd, SW_SHOW);

	//엔진의 포인터를 윈도우에 세팅해준다. WndProc함수에서 이를 다시 불러와서 사용할 것이다.
	SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&m_p3DgraphicEngine));

	return hwnd;
}

void ExampleGame::AddGrid()
{
	for (int i = 0; i < 101; i++)
	{
		HProceduralGeometry_line* line = 
		m_p3DgraphicEngine->CreateLine(eNoOption_Primitive);

		
		if (i == 0)
		{
			line->color = XMFLOAT4(0, 0, 0, 1);
		}
		else
		{
			line->color = XMFLOAT4(0.0, 0.0, 1.0, 1);
		}
		line->dots[0] = XMFLOAT3(10 * i - 500, 0, -500);
		line->dots[1] = XMFLOAT3(10 * i - 500, 0, +500);

	}

	for (int i = 0; i < 101; i++)
	{

		HProceduralGeometry_line* line =
			m_p3DgraphicEngine->CreateLine(eNoOption_Primitive);

		if (i == 0)
		{
			line->color = XMFLOAT4(0, 0, 0, 1);
		}
		else
		{
			line->color = XMFLOAT4(0.3, 0.3, 0.3, 0.5);
		}
		line->dots[0] = XMFLOAT3(500, 0, 10 * i - 500);
		line->dots[1] = XMFLOAT3(-500, 0, 10 * i - 500);

	}

}

void ExampleGame::Initialize(HINSTANCE hInstance, int clientWidth, int clientHeight)
{
	HWND hWnd = InitWindow(hInstance, clientWidth, clientHeight);
	
	//graphic initialize
	m_p3DgraphicEngine = HEngine_DX12_3D::GetInstance();
	m_p3DgraphicEngine->InitEngine(hWnd, clientWidth, clientHeight);
	m_keyboard = m_p3DgraphicEngine->GetKeyboard();

	//physics initialize
	m_p3DPhysicsEngine = &PhysicsEngine::PhysicsEngineMain::GetInstance();
	m_p3DPhysicsEngine->Initialize();

	m_pObjectManager = ObjectManager::GetInstance();
	m_pObjectManager->Initialize(m_p3DgraphicEngine, m_p3DPhysicsEngine);

	m_cameraControl.Initialize(dynamic_cast<HeroObject*>(m_pObjectManager->FindObjectByName("Hero")));

	//디버깅용 콘솔 창 켜기
	ConsoleWindow::Allocate();

	AddGrid();
}

void ExampleGame::Loop()
{
	float dTime = m_p3DgraphicEngine->GetElapsedTime();

	static float accumTime = 0;
	accumTime += dTime;

	if (accumTime > 1 / 60.f)
	{
		//캐릭터 컨트롤러 콜백 함수(OnControllerHit) 실행 시점을 동기화하기 위해 추가,
		//FixedUpdate 앞에서 동일한 간격(accumTime) 으로 동작하는 것이 좋음
		//m_p3DPhysicsEngine->ComputeInteractionsCCT(accumTime);

		m_pObjectManager->FixedUpdate(accumTime);
		accumTime = 0;
	}

	m_p3DPhysicsEngine->Update();
	m_pObjectManager->Update(dTime);

	m_cameraControl.Update(dTime);
	m_p3DgraphicEngine->Loop();
}

void ExampleGame::Finalize()
{
	m_pObjectManager->Finalize();
	m_p3DPhysicsEngine->Finalize();
	
	m_p3DgraphicEngine->EndEngine();

}
