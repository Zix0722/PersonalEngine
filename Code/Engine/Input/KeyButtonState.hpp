#pragma once
class InputSystem;

struct KeyButtonState
{
	friend class InputSystem;
public:
	void UpdateStatus(bool isPressedRightNow);
	bool IsPressed() const { return m_isPressed; }
	bool WasJustPressed() const;
	bool WasJustReleased() const;
	void Reset();
private:
	bool m_isPressed = false;
	bool m_wasPressedLastFrame = false;
};