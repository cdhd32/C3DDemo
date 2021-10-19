#include "pch.h"
#include "ThreadUtil.h"

void WINAPI LoopFunc(void* callbackFunc)
{
	while (ThreadUtil::isLooping)
	{
		//void*�� �Է� ������ ȣ���� �Լ� ���� void (*)()�� ������ ����
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
	//������ �� ������ ������ �� �ְ� ������ #����
	if (threadHandle)
		TerminateThread(threadHandle, 0);
}
