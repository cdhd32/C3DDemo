#pragma once

//WinAPI �ܼ� ��� Ŭ����
//������ ���� �����ؼ� ���
//https://docs.microsoft.com/en-us/windows/console/creation-of-a-console

class ConsoleWindow
{

private:
	static HANDLE m_ConsoleHandle;
	static TCHAR  m_Msg[8192];

	static bool m_IsAlloc;
	
public:
	//�ܼ� ������ ��ũ�� ��ǥ
	static INT X, Y;

	static CONSOLE_SCREEN_BUFFER_INFO m_CsbiInfo;
	static HANDLE m_hOutput;

	//�ܼ� ����ũ��
	static INT COLSIZE;

public:
	ConsoleWindow() = default;
	~ConsoleWindow() = default;

	//�ܼ�â�� �����Ǿ����� Ȯ���ϰ� ��� �� ��
	static bool IsAlloc() { return m_IsAlloc; };

	static bool Allocate();
	static bool Free();

	static void Write(const TCHAR* msg);
	//�ܼ� ��ũ�� (�۾���)
	static void ScrollConsole(int toScroll = COLSIZE);
	static void Move();

	static HWND GetWindow() { return  GetConsoleWindow(); }
	static HANDLE GetThreadHandle() { return m_ConsoleHandle; }

	static TCHAR* GetMsg() { return m_Msg; };
};

//how to use
/*wchar_t wdbgStr[256];
	_stprintf_s(wdbgStr, L"displacement( x : %f, y : %f, z : %f)\n", displacement.x, displacement.y, displacement.z);

	std::wstring dbgStr = wdbgStr;
	ConsoleWindow::Write(dbgStr.c_str());*/
