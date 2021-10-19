#include <Windows.h>
#include "C3DTimer.h"

C3DTimer* C3DTimer::m_pInstance = nullptr;

C3DTimer::C3DTimer()
	: m_StartTime(0), m_StopTime(0), m_LastCountTime(0),
	m_ElapsedTime(0.f), m_DeltaTime(0.f)
{
	LARGE_INTEGER count;
	QueryPerformanceFrequency(&count);

	m_FrequencyCountPerSec = 1.0 / (double)count.QuadPart;
}

C3DTimer::~C3DTimer()
{

}

unsigned int C3DTimer::Start()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	m_LastCountTime = count.QuadPart;
	m_StartTime = m_LastCountTime;

	return m_StartTime;
}

unsigned int C3DTimer::Stop()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	m_LastCountTime = count.QuadPart;
	m_StopTime = m_LastCountTime;

	return m_StopTime;
}

double C3DTimer::GetElapsedTime()
{
	m_ElapsedTime = m_StopTime - m_StartTime;

	return (double)m_ElapsedTime * m_FrequencyCountPerSec;
}


double C3DTimer::GetDeltaTime()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	m_DeltaTime = (double)(count.QuadPart - m_LastCountTime) * m_FrequencyCountPerSec;

	m_LastCountTime = count.QuadPart;

	return m_DeltaTime;
}

//UINT  CGameProcess::countPerFrame()
//{
//	static unsigned int enlapsedTime = 0;
//	static unsigned int LastTime = 0;
//	LARGE_INTEGER count, timer;
//
//	QueryPerformanceCounter(&count);
//	QueryPerformanceFrequency(&timer);
//
//	enlapsedTime = count.QuadPart - LastTime;
//
//	LastTime = count.QuadPart;
//
//	return (float)enlapsedTime / (float)timer.QuadPart * 1000.f;
//}