#pragma once

//winapi�� �ۼ��� Thread ��ƿ(�۾� ��)
//2021.01.28 �ֵ�ȣ
namespace ThreadUtil
{
	static bool isLooping = true;

	HANDLE CreateThreadLoop(void* callbackFunc);

	void FreeThreadLoop(HANDLE threadHandle);
};

