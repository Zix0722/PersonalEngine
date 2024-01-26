#pragma once
#include "Engine/Input/KeyButtonState.hpp"
#include "Engine/Input/XboxController.hpp"
#include "../Core/EventSystem.hpp"

class Window;


struct MouseState
{
	IntVec2 m_cursorClientPosition;
	IntVec2 m_cursorClientDelta;

	bool m_currentHidden = false;
	bool m_desiredHidden = false;

	bool m_currentRelative = false;
	bool m_desiredRelative = false;
};

struct InputSystemConfig
{
	Window* m_window = nullptr;
};

extern const unsigned char F1_KEY;
extern const unsigned char F2_KEY;
extern const unsigned char F3_KEY;
extern const unsigned char F4_KEY;
extern const unsigned char F5_KEY;
extern const unsigned char F6_KEY;
extern const unsigned char F7_KEY;
extern const unsigned char F8_KEY;
extern const unsigned char F9_KEY;
extern const unsigned char F10_KEY;
extern const unsigned char F11_KEY;
extern const unsigned char ENTER_KEY;
extern const unsigned char ESC_KEY;
extern const unsigned char SPACE_KEY;
extern const unsigned char KEYCODE_LMB;
extern const unsigned char KEYCODE_RMB;
extern const unsigned char KEYCODE_TILDE;
extern const unsigned char KEYCODE_UPARROW;
extern const unsigned char KEYCODE_DOWNARROW;
extern const unsigned char KEYCODE_LEFTARROW;
extern const unsigned char KEYCODE_RIGHTARROW;
extern const unsigned char KEYCODE_ENTER;
extern const unsigned char KEYCODE_BACKSPACE;
extern const unsigned char KEYCODE_INSERT;
extern const unsigned char KEYCODE_DELETE;
extern const unsigned char KEYCODE_HOME;
extern const unsigned char KEYCODE_END;
extern const unsigned char KEYCODE_LEFTBRACKET;
extern const unsigned char KEYCODE_RIGHTBRACKET;
extern const unsigned char KEYCODE_LEFTSHIFT;
extern const unsigned char KEYCODE_COMMA;
extern const unsigned char KEYCODE_PERIOD;
extern const unsigned char KEYCODE_SEMICOLON;
extern const unsigned char KEYCODE_SINGLEQUOTE;


constexpr int NUM_KEYCODES = 256;
constexpr int NUM_XBOX_CONTROLLERS = 4;

class InputSystem
{
public:
	InputSystem(InputSystemConfig const& config);
	~InputSystem();
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();
	bool WasKeyJustPressed(unsigned char keyCode);
	bool WasKeyJustReleased(unsigned char keyCode);
	bool IsKeyDown(unsigned char keyCode);
	bool HandleKeyPressed(unsigned char keyCode);
	bool HandleKeyReleased(unsigned char keyCode);
	XboxController const& GetController(int controllerID);
	InputSystemConfig const& GetConfig() const { return m_config; }

	static bool Event_KeyPressed(EventArgs& args);
	static bool Event_KeyReleased(EventArgs& args);

	void SetCursorMode(bool hidden, bool relative);
	Vec2 GetCursorClientDelta() const;
	Vec2 GetCursorClientPosition() const;
	Vec2 GetCursorNormalizedClientPosition() const;
	Vec2 GetCursorNormalizedScreenCameraPosition() const;

protected:
	InputSystemConfig   m_config;
	KeyButtonState		m_keyStates[NUM_KEYCODES];
	XboxController		m_controllers[NUM_XBOX_CONTROLLERS] =
	{
	XboxController(0),
	XboxController(1),
	XboxController(2),
	XboxController(3)
	};
	MouseState			m_mouseState;
};