#include <Windows.h>
#include "Engine/Input/KeyButtonState.hpp"
#include "InputSystem.hpp"
#include "Engine/Window/Window.hpp"

extern InputSystem* g_theInput;


const unsigned char ENTER_KEY = VK_RETURN;
const unsigned char F1_KEY = VK_F1;
const unsigned char F2_KEY = VK_F2;
const unsigned char F3_KEY = VK_F3;
const unsigned char F4_KEY = VK_F4;
const unsigned char F5_KEY = VK_F5;
const unsigned char F6_KEY = VK_F6;
const unsigned char F7_KEY = VK_F7;
const unsigned char F8_KEY = VK_F8;
const unsigned char F9_KEY = VK_F9;
const unsigned char F10_KEY = VK_F10;
const unsigned char F11_KEY = VK_F11;
const unsigned char ESC_KEY = VK_ESCAPE;
const unsigned char SPACE_KEY = VK_SPACE;
const unsigned char KEYCODE_LMB = VK_LBUTTON;
const unsigned char KEYCODE_RMB = VK_RBUTTON;
const unsigned char KEYCODE_TILDE = 0xC0;	//`/~
const unsigned char KEYCODE_UPARROW = VK_UP;
const unsigned char KEYCODE_DOWNARROW = VK_DOWN;
const unsigned char KEYCODE_LEFTARROW = VK_LEFT;
const unsigned char KEYCODE_RIGHTARROW = VK_RIGHT;
const unsigned char KEYCODE_ENTER = VK_RETURN;
const unsigned char KEYCODE_BACKSPACE = VK_BACK;
const unsigned char KEYCODE_INSERT = VK_INSERT;
const unsigned char KEYCODE_DELETE = VK_DELETE;
const unsigned char KEYCODE_HOME = VK_HOME;
const unsigned char KEYCODE_END = VK_END;
const unsigned char KEYCODE_LEFTBRACKET = 0xDB;	//[/{
const unsigned char KEYCODE_RIGHTBRACKET = 0xDD;   //]/}
const unsigned char KEYCODE_LEFTSHIFT = VK_SHIFT;
const unsigned char KEYCODE_COMMA = VK_OEM_COMMA;
const unsigned char KEYCODE_PERIOD = VK_OEM_PERIOD;
const unsigned char KEYCODE_SEMICOLON = VK_OEM_1;
const unsigned char KEYCODE_SINGLEQUOTE = VK_OEM_7;

InputSystem::InputSystem(InputSystemConfig const& config)
	:m_config(config)
{
	
}

InputSystem::~InputSystem()
{
}

void InputSystem::Startup()
{
	SubscribeEventCallbackFunction("KeyPressed", InputSystem::Event_KeyPressed);
	SubscribeEventCallbackFunction("KeyReleased", InputSystem::Event_KeyReleased);
}

void InputSystem::Shutdown()
{
}

void InputSystem::BeginFrame()
{
	for (int controllerIndex = 0; controllerIndex < MAX_XBOX_CONTROLLERS; controllerIndex++)
	{
		m_controllers[controllerIndex].Update();
	}


	if (m_mouseState.m_currentHidden != m_mouseState.m_desiredHidden)
	{
		m_mouseState.m_currentHidden = m_mouseState.m_desiredHidden;
	}
	if (m_mouseState.m_currentHidden == false)
	{
		while (ShowCursor(m_mouseState.m_currentHidden) >= 0);
	}
	if (m_mouseState.m_currentHidden == true)
	{
		while (ShowCursor(m_mouseState.m_currentHidden) < 0);
	}

	POINT cursorPos;
	::GetCursorPos(&cursorPos);

	HWND windowHandle = ::GetActiveWindow();
	::ScreenToClient(windowHandle, &cursorPos);

	m_mouseState.m_cursorClientPosition.x = cursorPos.x;
	m_mouseState.m_cursorClientPosition.y = cursorPos.y;


	if (m_mouseState.m_currentRelative != m_mouseState.m_desiredRelative)
	{
		m_mouseState.m_currentRelative = m_mouseState.m_desiredRelative;
		m_mouseState.m_cursorClientPosition = IntVec2(0, 0);
		m_mouseState.m_cursorClientDelta = IntVec2(0, 0);
		if (m_mouseState.m_currentRelative == true)
		{
			IntVec2 midOfScreen = IntVec2((int)((float)Window::GetWindowContext()->GetClientDimensions().x * 0.5f), (int)((float)Window::GetWindowContext()->GetClientDimensions().y * 0.5f));
			m_mouseState.m_cursorClientPosition.x = midOfScreen.x;
			m_mouseState.m_cursorClientPosition.y = midOfScreen.y;
		}

	}

	if (m_mouseState.m_currentRelative == true)
	{
		IntVec2 midOfScreen = IntVec2((int)((float)Window::GetWindowContext()->GetClientDimensions().x * 0.5f), (int)((float)Window::GetWindowContext()->GetClientDimensions().y * 0.5f));
		m_mouseState.m_cursorClientDelta = m_mouseState.m_cursorClientPosition - midOfScreen;
		windowHandle = ::GetActiveWindow();
		POINT midScreenPoint;
		midScreenPoint.x = midOfScreen.x;
		midScreenPoint.y = midOfScreen.y;
		::ClientToScreen(windowHandle, &midScreenPoint);
		::SetCursorPos(midScreenPoint.x, midScreenPoint.y);
	}

	::GetCursorPos(&cursorPos);

	windowHandle = ::GetActiveWindow();
	::ScreenToClient(windowHandle, &cursorPos);

	m_mouseState.m_cursorClientPosition.x = cursorPos.x;
	m_mouseState.m_cursorClientPosition.y = cursorPos.y;


}

void InputSystem::EndFrame()
{
	for (int keyboardIndex = 0; keyboardIndex < NUM_KEYCODES; keyboardIndex++)
	{
		if (m_keyStates[keyboardIndex].m_wasPressedLastFrame != m_keyStates[keyboardIndex].m_isPressed)
		{
			m_keyStates[keyboardIndex].m_wasPressedLastFrame = m_keyStates[keyboardIndex].m_isPressed;
		}
	}
}

bool InputSystem::WasKeyJustPressed(unsigned char keyCode)
{
	return  m_keyStates[keyCode].WasJustPressed();
}

bool InputSystem::WasKeyJustReleased(unsigned char keyCode)
{
	return m_keyStates[keyCode].WasJustReleased();
}

bool InputSystem::IsKeyDown(unsigned char keyCode)
{
	return m_keyStates[keyCode].IsPressed();
}

bool InputSystem::HandleKeyPressed(unsigned char keyCode)
{
	m_keyStates[keyCode].m_isPressed = true;
	return true;
}

bool InputSystem::HandleKeyReleased(unsigned char keyCode)
{
	m_keyStates[keyCode].m_isPressed = false;
	return true;
}

XboxController const& InputSystem::GetController(int controllerID)
{
	return m_controllers[controllerID];
}

bool InputSystem::Event_KeyPressed(EventArgs& args)
{
	if (!g_theInput)
	{
		return false;
	}
	unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);
	g_theInput->HandleKeyPressed(keyCode);
	return true;
}

bool InputSystem::Event_KeyReleased(EventArgs& args)
{
	if (!g_theInput)
	{
		return false;
	}
	unsigned char keyCode = (unsigned char)args.GetValue("KeyCode", -1);
	g_theInput->HandleKeyReleased(keyCode);
	return true;
}

void InputSystem::SetCursorMode(bool hidden, bool relative)
{
	m_mouseState.m_desiredHidden = hidden;
	m_mouseState.m_desiredRelative = relative;

	if (m_mouseState.m_currentHidden != m_mouseState.m_desiredHidden)
	{
		m_mouseState.m_currentHidden = m_mouseState.m_desiredHidden;
	}
	if (m_mouseState.m_currentHidden == false)
	{
		while (ShowCursor(m_mouseState.m_currentHidden) >= 0);
	}
	if (m_mouseState.m_currentHidden == true)
	{
		while (ShowCursor(m_mouseState.m_currentHidden) < 0);
	}
}

Vec2 InputSystem::GetCursorClientDelta() const
{
	return Vec2((float)m_mouseState.m_cursorClientDelta.x, (float)m_mouseState.m_cursorClientDelta.y);
}

Vec2 InputSystem::GetCursorClientPosition() const
{
	return Vec2((float)m_mouseState.m_cursorClientPosition.x, (float)m_mouseState.m_cursorClientPosition.y);
}

Vec2 InputSystem::GetCursorNormalizedClientPosition() const
{
	POINT cursorPos;
	::GetCursorPos(&cursorPos);

	HWND windleHandle = ::GetActiveWindow();
	::ScreenToClient(windleHandle, &cursorPos);

	RECT clientRectangle;
	::GetClientRect(windleHandle, &clientRectangle);

	float clientWidth = float(clientRectangle.right - clientRectangle.left);
	float clientLength = float(clientRectangle.bottom - clientRectangle.top);
	float mouseClientX = float(cursorPos.x);
	float mouseClientY = float(cursorPos.y);
	float mouseNormalizedU = mouseClientX / clientWidth;
	float mouseNormalizedV = mouseClientY / clientLength;

	return Vec2(mouseNormalizedU, 1.f - mouseNormalizedV);
}

Vec2 InputSystem::GetCursorNormalizedScreenCameraPosition() const
{
	return Vec2(0.f, 0.f);
}
