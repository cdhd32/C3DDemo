#include "pch.h"
#include "ConsoleWindow.h" 

//thread read buffer, loop
void __stdcall ConsoleThread(void* fp)
{
	HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
	TCHAR buffer[1024];
	DWORD length;
	while (1)
	{
		ReadConsole(input, buffer, 1024, &length, 0);

		// 입력에 따른 처리
		//...
	}
}

HANDLE ConsoleWindow::m_ConsoleHandle = NULL;

bool ConsoleWindow::m_IsAlloc = false;

INT ConsoleWindow::X = 0;

INT ConsoleWindow::Y = 0;

CONSOLE_SCREEN_BUFFER_INFO ConsoleWindow::m_CsbiInfo;

HANDLE ConsoleWindow::m_hOutput = NULL;

INT ConsoleWindow::COLSIZE = 28;

bool ConsoleWindow::Allocate()
{
	if (AllocConsole() != 0)
	{
		DWORD id;
		m_ConsoleHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ConsoleThread, 0, 0, &id);
		if (m_ConsoleHandle)
		{
			m_IsAlloc = true;
			SetConsoleCP(CP_UTF8); //코드페이지 UTF-8 로 설정
			m_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
			GetConsoleScreenBufferInfo(m_hOutput, &m_CsbiInfo);
			COLSIZE = m_CsbiInfo.dwSize.Y;
			return true;
		}
	}
	return false;
}

bool ConsoleWindow::Free()
{
	if (m_ConsoleHandle)
		TerminateThread(m_ConsoleHandle, 0);

	m_ConsoleHandle = NULL;
	m_IsAlloc = false;

	return FreeConsole() != 0;
}

void ConsoleWindow::Write(const TCHAR* msg)
{
	DWORD written = 0;
	WriteConsole(m_hOutput, msg, (DWORD)_tcslen(msg), &written, 0);
}

//콘솔 스크롤 (작업중)
void ConsoleWindow::ScrollConsole(int toScroll)
{
	SMALL_RECT srctWindow;

	// Check whether the window is too close to the screen buffer top
	if (m_CsbiInfo.srWindow.Top >= toScroll)
	{
		srctWindow.Top = -(SHORT)toScroll;     // move top up
		srctWindow.Bottom = -(SHORT)toScroll;  // move bottom up
		srctWindow.Left = 0;         // no change
		srctWindow.Right = 0;        // no change

		SetConsoleWindowInfo(
			m_hOutput,          // screen buffer handle
			FALSE,            // relative coordinates
			&srctWindow);     // specifies new location
	}
}

void ConsoleWindow::Move()
{
	HWND hConsole = GetConsoleWindow();
	if (hConsole == NULL) return;

	RECT rc;
	GetWindowRect(hConsole, &rc);
	MoveWindow(hConsole, X, Y, (rc.right - rc.left), (rc.bottom - rc.top), TRUE);
}
