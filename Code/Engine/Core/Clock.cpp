#include "Engine/Core/Clock.hpp"
#include "Engine/Core/Time.hpp"

static Clock s_SystemClock;


Clock::Clock(Clock& parent)
	:m_parent(&parent)
{
	parent.AddChild(this);
}

Clock::Clock()
{
	if (this == &s_SystemClock)
	{
		return;
	}
	m_parent = &s_SystemClock;
	s_SystemClock.AddChild(this);
}

void Clock::Reset()
{
	m_totalSeconds = 0.f;
}

bool Clock::IsPaused() const
{
	return m_isPaused;
}

void Clock::Pause()
{
	m_isPaused = true;
}

void Clock::Unpause()
{
	m_isPaused = false;
}

void Clock::TogglePause()
{
	m_isPaused = !m_isPaused;
}

void Clock::StepSingleFrame()
{
	m_stepSingleFrame = true;
}

void Clock::SetTimeScale(float timeScale)
{
	m_timeScale = timeScale;
}

float Clock::GetTimeScale() const
{
	return m_timeScale;
}

float Clock::GetDeltaSeconds() const
{
	return m_deltaSeconds;
}

float Clock::GetTotalSeconds() const
{
	return m_totalSeconds;
}

size_t Clock::GetFrameCount() const
{
	return m_frameCount;
}

Clock& Clock::GetSystemClock()
{
	return s_SystemClock;
}

void Clock::TickSystemClock()
{
	s_SystemClock.Tick();
}

void Clock::Tick()
{
	float currentTimeInSeconds = (float)GetCurrentTimeSeconds();
	if (m_stepSingleFrame)
	{
		m_deltaSeconds = m_maxDeltaSeconds * m_timeScale;
		m_stepSingleFrame = false;
	}
	else
	{
		if (m_isPaused)
		{
			m_deltaSeconds = 0.f;
		}
		else
		{
			m_deltaSeconds = (currentTimeInSeconds - m_lastUpdateTimeInSeconds) * m_timeScale;
		}
	}

	m_lastUpdateTimeInSeconds = currentTimeInSeconds;

	Advance(m_deltaSeconds);

}

void Clock::Advance(float deltaTimeSeconds)
{
	m_totalSeconds += deltaTimeSeconds;
	m_frameCount++;

	for (int childIndex = 0; childIndex < m_children.size(); childIndex++)
	{
		if (m_children[childIndex] != nullptr)
		{
			m_children[childIndex]->Advance(deltaTimeSeconds);
		}
	}
}

void Clock::AddChild(Clock* childClock)
{
	for (int childIndex = 0; childIndex < m_children.size(); childIndex++) 
	{
		Clock* child = m_children[childIndex];
		if (child == nullptr) 
		{
			child = childClock;
			childClock->m_parent = this;
			return;
		}
	}
	m_children.push_back(childClock);
	childClock->m_parent = this;
}

void Clock::RemoveChild(Clock* childClock)
{
	for (int childIndex = 0; childIndex < m_children.size(); childIndex++) 
	{
		Clock* child = m_children[childIndex];
		if (child != nullptr)
		{
			if (child == childClock)
			{
				m_children[childIndex] = nullptr;
				childClock->m_parent = nullptr;
				return;
			}
		}
	}
}

Clock::~Clock()
{
 	if (this != &s_SystemClock)
	{
		m_parent->RemoveChild(this);
	}
	for (int childIndex = 0; childIndex < m_children.size(); childIndex++)
	{
		RemoveChild(m_children[childIndex]);
	}
}

