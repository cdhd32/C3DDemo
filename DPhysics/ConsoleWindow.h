#pragma once

//WinAPI 콘솔 출력 클래스
//스레드 따로 생성해서 사용
//https://docs.microsoft.com/en-us/windows/console/creation-of-a-console

class ConsoleWindow
{

private:
	static HANDLE m_ConsoleHandle;
	static TCHAR  m_Msg[8192];

	static bool m_IsAlloc;
	
public:
	//콘솔 생성할 스크린 좌표
	static INT X, Y;

	static CONSOLE_SCREEN_BUFFER_INFO m_CsbiInfo;
	static HANDLE m_hOutput;

	//콘솔 세로크기
	static INT COLSIZE;

public:
	ConsoleWindow() = default;
	~ConsoleWindow() = default;

	//콘솔창이 생성되었는지 확인하고 사용 할 것
	static bool IsAlloc() { return m_IsAlloc; };

	static bool Allocate();
	static bool Free();

	static void Write(const TCHAR* msg);
	//콘솔 스크롤 (작업중)
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
