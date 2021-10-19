#pragma once

class C3DRenderer;
class C3DScene;
class DemoScene;

//WinMain ��Ʈ�� ����Ʈ���� ����Ǵ� ���� ������ ��ü

class MainWindow
{
//member variable
public:

private:
	static MainWindow* m_pInst;

	HINSTANCE m_hInstance;
	HWND m_hWnd;

	//���� ���� ��� ����
	C3DRenderer* m_pRenderer;
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

