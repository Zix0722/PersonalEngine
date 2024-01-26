#include <Windows.h>
#include <Xinput.h>
#pragma comment( lib, "xinput9_1_0")
#include "Engine/Math/MathUtils.hpp"
#include "XboxController.hpp"

XboxController::XboxController(int id)
	:m_id(id)
{
	m_leftStick.SetDeadZoneThresholds(0.3f, 0.95f);
	m_rightStick.SetDeadZoneThresholds(0.3f, 0.95f);
}

XboxController::~XboxController()
{
}

bool XboxController::IsConnected() const
{
	return m_isConnected;
}

int XboxController::GetControllerID() const
{
	return m_id;
}

AnalogJoystick const& XboxController::GetLeftStick() const
{
	return m_leftStick;
}

AnalogJoystick const& XboxController::GetRightStick() const
{
	return m_rightStick;
}

float XboxController::GetLeftTrigger() const
{
	return m_leftTrigger;
}

float XboxController::GetRightTrigger() const
{
	return  m_rightTrigger;
}

KeyButtonState const& XboxController::GetButton(XboxButtonID buttonID) const
{
	return m_buttons[buttonID];
}

bool XboxController::IsButtonDown(XboxButtonID buttonID) const
{
	return m_buttons[buttonID].IsPressed();
}

bool XboxController::WasButtonJustPressed(XboxButtonID buttonID) const
{
	return m_buttons[buttonID].WasJustPressed();
}

bool XboxController::WasButtonJustReleased(XboxButtonID buttonID) const
{
	return m_buttons[buttonID].WasJustReleased();
}

void XboxController::Update()
{
	XINPUT_STATE xboxControllerState;
	memset(&xboxControllerState, 0, sizeof(xboxControllerState));
	DWORD errorStatus = XInputGetState(m_id, &xboxControllerState);
	if (errorStatus == ERROR_SUCCESS)
	{
		m_isConnected = true;
		UpdateJoystick(m_leftStick, xboxControllerState.Gamepad.sThumbLX, xboxControllerState.Gamepad.sThumbLY);
		UpdateJoystick(m_rightStick, xboxControllerState.Gamepad.sThumbRX, xboxControllerState.Gamepad.sThumbRY);
		UpdateButton(XBOX_BUTTON_A, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_A);
		UpdateButton(XBOX_BUTTON_B, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_B);
		UpdateButton(XBOX_BUTTON_X, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_X);
		UpdateButton(XBOX_BUTTON_Y, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_Y);
		UpdateButton(XBOX_BUTTON_BACK, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_BACK);
		UpdateButton(XBOX_BUTTON_START, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_START);
		UpdateButton(XBOX_BUTTON_L, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER);
		UpdateButton(XBOX_BUTTON_R, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER);
		UpdateButton(XBOX_BUTTON_LTHUMB, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_LEFT_THUMB);
		UpdateButton(XBOX_BUTTON_RTHUMB, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_RIGHT_THUMB);
		UpdateButton(XBOX_BUTTON_RIGHT, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_RIGHT);
		UpdateButton(XBOX_BUTTON_LEFT, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_LEFT);
		UpdateButton(XBOX_BUTTON_UP, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_UP);
		UpdateButton(XBOX_BUTTON_DOWN, xboxControllerState.Gamepad.wButtons, XINPUT_GAMEPAD_DPAD_DOWN);
		UpdateTrigger(m_leftTrigger, xboxControllerState.Gamepad.bLeftTrigger);
		UpdateTrigger(m_rightTrigger, xboxControllerState.Gamepad.bRightTrigger);
	}
	else
	{
		m_isConnected = false;
	}
}

void XboxController::Reset()
{
	m_id               = -1;
	m_isConnected      = false;
	m_leftTrigger      = 0.f;
	m_rightTrigger     = 0.f;
	for (int buttonIndex = 0; buttonIndex < XBOX_BUTTON_NUM; ++buttonIndex)
	{
		m_buttons[buttonIndex].Reset();
	}
	
	m_leftStick.Reset();
	m_rightStick.Reset();
}

void XboxController::UpdateJoystick(AnalogJoystick& out_joystick, short rawX, short rawY)
{
	float normalizedX = RangeMap(rawX ,-32768.f, 32767.f, -1.f, 1.f);
	float normalizedY = RangeMap(rawY, -32768.f, 32767.f, -1.f, 1.f);
	out_joystick.UpdatePosition(normalizedX, normalizedY);
}

void XboxController::UpdateTrigger(float& out_triggerValue, unsigned char rawValue)
{
	out_triggerValue = static_cast<float>(rawValue * 1.f / 255.f);
}

void XboxController::UpdateButton(XboxButtonID buttonID, unsigned short buttonFlags, unsigned short buttonFlag)
{
	bool isPressed = buttonFlags & buttonFlag;
	m_buttons[buttonID].UpdateStatus(isPressed);
}
