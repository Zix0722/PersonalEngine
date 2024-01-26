#include "Stopwatch.hpp"
#include "Clock.hpp"

Stopwatch::Stopwatch(float duration)
	:m_duration(duration)
	,m_clock(&Clock::GetSystemClock())
{
	
}

Stopwatch::Stopwatch(Clock const* clock, float duration)
	:m_duration(duration)
	,m_clock(clock)
{

}

Stopwatch::~Stopwatch()
{

}

void Stopwatch::Start()
{
	m_startTime = m_clock->GetTotalSeconds();
}

void Stopwatch::Restart()
{
	if (IsStopped())
	{
		m_startTime = m_clock->GetTotalSeconds();
	}
}

void Stopwatch::Stop()
{
	m_startTime = 0.f;
}

float Stopwatch::GetElapsedTime() const
{
	if (IsStopped())
	{
		return 0.f;
	}
	return m_clock->GetTotalSeconds() - m_startTime;
}

float Stopwatch::GetElapsedFration() const
{
	if (IsStopped())
	{
		return 0.f;
	}
	return GetElapsedTime()/ m_duration;
}

bool Stopwatch::IsStopped() const
{
	if (m_startTime == 0.f)
	{
		return true;
	}
	return false;
}

bool Stopwatch::HasDurationElapsed() const
{
	if (GetElapsedTime() > m_duration)
	{
		return true;
	}
	return false;
}

bool Stopwatch::DecrementDurationIfElapsed()
{
	if (HasDurationElapsed() && !IsStopped())
	{
		m_startTime += m_duration;
		return true;
	}
	return false;
}

