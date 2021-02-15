#include "GameTimer.h"

GameTimer::GameTimer()
	:m_SecondsPerCount(0.0), m_DeltaTime(-1.0), m_BaseTime(0),
	m_PausedTime(0), m_StopTime(0), m_PrevTime(0), m_CurrentTime(0),
	m_Stopped(false)
{
	//Query the frequency of performance counter
	__int64 countsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	m_SecondsPerCount = 1.0 / (double)countsPerSec;
}

GameTimer::~GameTimer()
{
}

float GameTimer::GameTime() const
{
	return 0.0f;
}

float GameTimer::DeltaTime() const
{
	return (float)m_DeltaTime;
}

float GameTimer::TotalTime() const
{
	//If stopped, do not count time passed since stopped
	if (m_Stopped)
	{
		return (float)(((m_StopTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
	}
	else //CurrTime - BaseTime includes paused time, which is not wanted. Corrected by subtracting paused time from CurrTime
	{
		return (float)(((m_CurrentTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
	}
}

//Initialise the values for first frame
void GameTimer::Reset()
{
	__int64 currTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_BaseTime = currTime;
	m_PrevTime = currTime;
	m_StopTime = 0;
	m_Stopped = false;
}

void GameTimer::Start()
{
	__int64 startTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	//Accumulate the time elapsed between stop and start pairs
	//If resuming timer from stopped state
	if (m_Stopped)
	{
		//then accumulate paused time
		m_PausedTime += (startTime - m_StopTime);

		//Current previous time is not valid as timer is started again
		m_PrevTime = startTime;

		m_StopTime = 0;
		m_Stopped = false;
	}
}

void GameTimer::Stop()
{
	//If already stopped, do nothing
	if (!m_Stopped)
	{
		__int64 currTime = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		//Save the time stopped at and set stopped to true as timer has stopped
		m_StopTime = currTime;
		m_Stopped = true;
	}
}

void GameTimer::Tick()
{
	if (m_Stopped)
	{
		m_DeltaTime = 0.0;
		return;
	}

	//Get the time this frame
	__int64 currTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_CurrentTime = currTime;

	//Time difference between this frame and the previous 
	m_DeltaTime = (m_CurrentTime - m_PrevTime) * m_SecondsPerCount;

	//Prepare for next frame
	m_PrevTime = m_CurrentTime;

	//Ensure nonnegative - Deltatime can become negative if processor goes into sleep mode or changed thread
	if (m_DeltaTime < 0.0) { m_DeltaTime = 0; }

}
