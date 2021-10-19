#include "pch.h"
#include "ThreadUtil.h"

void WINAPI LoopFunc(void* callbackFunc)
{
	while (ThreadUtil::isLooping)
	{
		//void*로 입력 받으나 호출할 함수 형는 void (*)()로 정해져 있음
		(*(void (*)())callbackFunc)();
	}
}

HANDLE ThreadUtil::CreateThreadLoop(void *callbackFunc)
{
	DWORD id;

	void* pfunc = callbackFunc;

	HANDLE threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LoopFunc, callbackFunc, 0, &id);
	if (threadHandle)
	{
		return threadHandle;
	}
	return threadHandle;
}


void ThreadUtil::FreeThreadLoop(HANDLE threadHandle)
{
	//루프가 다 끝나고 종료할 수 있게 만들어보자 #수정
	if (threadHandle)
		TerminateThread(threadHandle, 0);
}
