#include "KeyButtonState.hpp"

void KeyButtonState::UpdateStatus(bool isPressedRightNow)
{
	m_wasPressedLastFrame = m_isPressed;
	m_isPressed = isPressedRightNow;
}

bool KeyButtonState::WasJustPressed() const
{
	if (!m_wasPressedLastFrame && m_isPressed)
	{
		return true;
	}
	return false;
}

bool KeyButtonState::WasJustReleased() const
{
	if (m_wasPressedLastFrame &&
		m_isPressed == false)
	{
		return true;
	}
	return false;
}

void KeyButtonState::Reset()
{
	m_isPressed = false;
	m_wasPressedLastFrame = false;
}
