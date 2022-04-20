// main.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

//나중에 사용하는 방법 알아보기
//#if defined(_DEBUG)
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#endif
#include "framework.h"

#include "MainWindow.h"


#include "main.h"

//콘솔 출력
#ifdef _DEBUG
    #pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif // DEBUG



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    if (MainWindow::GetInstance()->Initialize(hInstance))
    {
        MainWindow::GetInstance()->Run();
    }
    MainWindow::GetInstance()->Release();
}
