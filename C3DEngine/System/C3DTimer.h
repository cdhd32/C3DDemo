#pragma once

class C3DTimer
{
private:
	static C3DTimer* m_pInstance;

	__int64 m_StartTime;
	__int64 m_StopTime;
	__int64 m_LastCountTime;
	double m_ElapsedTime;
	double m_DeltaTime;
	double m_FrequencyCountPerSec;
private:
	C3DTimer();
	~C3DTimer();

public:
	static C3DTimer* GetInstanse()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new C3DTimer();
		}

		return m_pInstance;
	}

	unsigned int Start();
	unsigned int Stop();
	double GetElapsedTime();
	double GetDeltaTime();

};
