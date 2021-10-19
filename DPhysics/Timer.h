#pragma once

class Timer
{
private:
	static Timer* m_pInstance;

	__int64 m_StartTime;
	__int64 m_StopTime;
	__int64 m_LastCountTime;
	double m_ElapsedTime;
	double m_DeltaTime;
	double m_FrequencyCountPerSec;
private:
	Timer();
	~Timer();

public:
	static Timer* GetInstanse()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new Timer();
		}

		return m_pInstance;
	}

	unsigned int Start();
	unsigned int Stop();
	double GetElapsedTime();
	double GetDeltaTime();

};
