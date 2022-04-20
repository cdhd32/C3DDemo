#pragma once
#include <iostream>

class C3DRenderer;
class C3DScene;
class DemoScene;
class C3DDevice;

//WinMain 엔트리 포인트에서 실행되는 메인 윈도우 객체

class MainWindow
{
//member variable
public:

private:
	static MainWindow* m_pInst;

	HINSTANCE m_hInstance;
	HWND m_hWnd;

	//엔진 관련 멤버 변수
	std::unique_ptr<C3DDevice> m_pDevice;
	DemoScene* m_pScene = nullptr;


//member function
public:
	static MainWindow* GetInstance()
					{
		if (m_pInst == nullptr)
		{
			m_pInst = new MainWindow();
		}
		return m_pInst;
	};

	bool Initialize(HINSTANCE hInstance);
	bool Run();
	bool Release();
	
	HINSTANCE GetInstanceHandle() const { return m_hInstance; }
	HWND GetWndHandle() const { return m_hWnd; }
private:
	MainWindow();
	~MainWindow();

	HRESULT MainWindowLoop();
	ATOM CreateMainWindowClass(HINSTANCE hInstance);

	
};

