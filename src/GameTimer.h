#pragma once
#include <windows.h>

class GameTimer
{
public:
	GameTimer();
	~GameTimer();

	float GameTime() const;
	float DeltaTime() const;
	float TotalTime() const;

	void Reset();
	void Start();
	void Stop();
	void Tick();


private:

	double m_SecondsPerCount;
	double m_DeltaTime;

	__int64 m_BaseTime;
	__int64 m_PausedTime;
	__int64 m_StopTime;
	__int64 m_PrevTime;
	__int64 m_CurrentTime;

	bool m_Stopped;

};

