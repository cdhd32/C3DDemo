#pragma once

//winapi로 작성된 Thread 유틸(작업 중)
//2021.01.28 최동호
namespace ThreadUtil
{
	static bool isLooping = true;

	HANDLE CreateThreadLoop(void* callbackFunc);

	void FreeThreadLoop(HANDLE threadHandle);
};

