#define WIN32_LEAN_AND_MEAN		// Always #define this before #including <windows.h>
#include <windows.h>			// #include this (massive, platform-specific) header in very few places
#include "Engine/Window/Window.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "../Core/EventSystem.hpp"
#include <commdlg.h>
#include "../Math/MathUtils.hpp"
#include "ThirdParty/imgui/imgui_impl_win32.h"
#include "../Renderer/DebugRender.hpp"


Window* Window::s_mainWindow = nullptr;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowsMessageHandlingProcedure(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam)
{
	Window* windowContext = Window::GetWindowContext();
	GUARANTEE_OR_DIE(windowContext != nullptr, "WindowContext was null!");
	InputSystem* input = windowContext->GetConfig().m_inputSystem;
	GUARANTEE_OR_DIE(input != nullptr, "No Input System!");

	//IMGUI
	ImGui_ImplWin32_WndProcHandler(windowHandle, wmMessageCode, wParam, lParam);

	switch (wmMessageCode)
	{
		// App close requested via "X" button, or right-click "Close Window" on task bar, or "Close" from system menu, or Alt-F4
		case WM_CLOSE:
		{
			//ERROR_AND_DIE("WM_CLOSE not currently handled!");
			//return 0; // "Consumes" this message (tells Windows "okay, we handled it")
			FireEvent("quit");
		}

		// Raw physical keyboard "key-was-just-depressed" event (case-insensitive, not translated)
		case WM_LBUTTONDOWN:
		{
			unsigned char asKey = (unsigned char) KEYCODE_LMB;
			bool wasConsumed = false;
			if (input)
			{
				wasConsumed = input->HandleKeyPressed(asKey);
				if (wasConsumed)
				{
					return 0;
				}
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			unsigned char asKey = (unsigned char)KEYCODE_LMB;
			bool wasConsumed = false;
			if (input)
			{
				wasConsumed = input->HandleKeyReleased(asKey);
				if (wasConsumed)
				{
					return 0;
				}
			}
			break;
		}
		case WM_RBUTTONDOWN:
		{
			unsigned char asKey = (unsigned char)KEYCODE_RMB;
			bool wasConsumed = false;
			if (input)
			{
				wasConsumed = input->HandleKeyPressed(asKey);
				if (wasConsumed)
				{
					return 0;
				}
			}
			break;
		}
		case WM_RBUTTONUP:
		{
			unsigned char asKey = (unsigned char)KEYCODE_RMB;
			bool wasConsumed = false;
			if (input)
			{
				wasConsumed = input->HandleKeyReleased(asKey);
				if (wasConsumed)
				{
					return 0;
				}
			}
			break;
		}
		case WM_KEYDOWN:
		{
			unsigned char asKey = (unsigned char)wParam;
			EventArgs args;
			args.SetValue("KeyCode", Stringf("%d", asKey));
			FireEvent("KeyPressed", args);
			break;
		}
		// Raw physical keyboard "key-was-just-released" event (case-insensitive, not translated)
		case WM_KEYUP:
		{
			unsigned char asKey = (unsigned char)wParam;
			EventArgs args;
			args.SetValue("KeyCode", Stringf("%d", asKey));
			FireEvent("KeyReleased", args);
			break;
		}
		case WM_CHAR:
		{
			unsigned char asKey = (unsigned char)wParam;
			EventArgs args;
			args.SetValue("char", Stringf("%d", asKey));
			FireEvent("CharInput", args);
			break;
		}
	}

	// Send back to Windows any unhandled/unconsumed messages we want other apps to see (e.g. play/pause in music apps, etc.)
	return DefWindowProc(windowHandle, wmMessageCode, wParam, lParam);
}

Window::Window(WindowConfig const& config)
	:m_config(config)
{
	s_mainWindow = this;
}

Window::~Window()
{
}

void Window::Startup()
{
	CreateOSWindow();
}

void Window::BeginFrame()
{
	RunMessagePump();
}

void Window::EndFrame()
{
}

void Window::Shutdown()
{
	
}

Vec2 Window::GetCursorNormalizeUVs() const
{
	POINT cursorPos;
	::GetCursorPos( &cursorPos );

	HWND windleHandle = m_hwnd;
	::ScreenToClient(windleHandle, &cursorPos);

	//std::string str = Stringf("%d %d", cursorPos.x, cursorPos.y);
	//DebugAddScreenText(str, Vec2(100.f, 300.f), 30.f, Vec2(0.5f, 0.5f), 0.f, Rgba8::GREEN);

	RECT clientRectangle;
	::GetClientRect(windleHandle,&clientRectangle);

	float clientWidth = float (clientRectangle.right - clientRectangle.left);
	float clientLength = float (clientRectangle.bottom - clientRectangle.top);
	float mouseClientX = float(cursorPos.x);
	float mouseClientY = float(cursorPos.y);
	float mouseNormalizedU = mouseClientX / clientWidth;
	float mouseNormalizedV = mouseClientY / clientLength;

	return Vec2(mouseNormalizedU, 1.f - mouseNormalizedV);
}

void* Window::GetHwnd() const
{
	return m_hwnd;
}

IntVec2 Window::GetClientDimensions() const
{
	RECT clientRectangle;
	HWND winHandle = static_cast<HWND>(GetHwnd());
	::GetClientRect(winHandle, &clientRectangle);
	return IntVec2(clientRectangle.right - clientRectangle.left, clientRectangle.bottom - clientRectangle.top);
}

bool Window::IsFocusingWindow() const
{
	HWND currentWin = ::GetActiveWindow();
	if (currentWin == m_hwnd)
	{
		return true;
	}
	return false;
}

Vec2 Window::GetMouseOffset() const
{
	POINT cursorPos;
	::GetCursorPos(&cursorPos);

	//HWND windleHandle = m_hwnd;
	HWND winHandle = static_cast<HWND>(GetHwnd());
 
	IntVec2 const& clientDimension = GetClientDimensions();
	::ScreenToClient(winHandle, &cursorPos);
	Vec2 posCursor = Vec2(static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y));

	float cursorX = posCursor.x / clientDimension.x;
	float cursorY = posCursor.y / clientDimension.y;
	return Vec2(cursorX, 1.f - cursorY);

}

bool Window::GetXMLPathByOpenFileNameA(std::string& filePath, bool is3D)
{
	if (is3D)
	{
		m_config.m_inputSystem->SetCursorMode(true, false);
	}

	OPENFILENAMEA ofn;
	char defaultDirectory[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, defaultDirectory);
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = (HWND)GetHwnd();
	ofn.lpstrFilter = "XML Files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a Model XML File";
	ofn.lpstrInitialDir = defaultDirectory;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		if (strcmp(szFileName, "") != 0)
		{
			filePath = szFileName;
			SetCurrentDirectoryA(defaultDirectory);
			if (is3D)
			{
				m_config.m_inputSystem->SetCursorMode(false, true);
			}
			EventArgs args;
			args.SetValue("KeyCode", Stringf("%d", 'L'));
			FireEvent("KeyReleased", args);
			return true;
		}
		else
		{
			SetCurrentDirectoryA(defaultDirectory);
			if (is3D)
			{
				m_config.m_inputSystem->SetCursorMode(false, true);
			}
			EventArgs args;
			args.SetValue("KeyCode", Stringf("%d", 'L'));
			FireEvent("KeyReleased", args);
			return false;
		}
	}
	else
	{
		SetCurrentDirectoryA(defaultDirectory);
		if (is3D)
		{
			m_config.m_inputSystem->SetCursorMode(false, true);
		}
		EventArgs args;
		args.SetValue("KeyCode", Stringf("%d", 'L'));
		FireEvent("KeyReleased", args);
		return false;
	}
}

Vec2 Window::GetCursorClientPos()
{
	POINT cursorPos;
	::GetCursorPos(&cursorPos);

	HWND windleHandle = m_hwnd;
	::ScreenToClient(windleHandle, &cursorPos);

	return Vec2((float)cursorPos.x, m_fullScreenDimensions.y - (float)cursorPos.y);
}

void Window::CreateOSWindow()
{
	
	WNDCLASSEX windowClassDescription;
	memset(&windowClassDescription, 0, sizeof(windowClassDescription));
	windowClassDescription.cbSize = sizeof(windowClassDescription);
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast<WNDPROC>(WindowsMessageHandlingProcedure); // Register our Windows message-handling function
	windowClassDescription.hInstance = GetModuleHandle(NULL);
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT("Simple Window Class");
	RegisterClassEx(&windowClassDescription);

	// #SD1ToDo: Add support for full screen mode (requires different window style flags than windowed mode)
	DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	if (m_config.m_isFullscreen)
	{
		windowStyleFlags = WS_POPUP;
	}
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	// Get desktop rect, dimensions, aspect
	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect(desktopWindowHandle, &desktopRect);
	float desktopWidth = (float)(desktopRect.right - desktopRect.left);
	float desktopHeight = (float)(desktopRect.bottom - desktopRect.top);
	float desktopAspect = desktopWidth / desktopHeight;

	if (m_config.m_Size != IntVec2(0, 0))
	{
		desktopWidth = (float)m_config.m_Size.x;
		desktopHeight = (float)m_config.m_Size.y;
		desktopAspect = desktopWidth / desktopHeight;
	}
	

	// Calculate maximum client size (as some % of desktop size)
	float maxClientFractionOfDesktop = m_config.m_isFullscreen ? 1.f : 0.90f;
	float clientWidth = desktopWidth * maxClientFractionOfDesktop;
	float clientHeight = desktopHeight * maxClientFractionOfDesktop;



	if (!m_config.m_isFullscreen)
	{
		if (m_config.m_clientAspect > desktopAspect)
		{
			// Client window has a wider aspect than desktop; shrink client height to match its width
			clientHeight = clientWidth / m_config.m_clientAspect;

		}
		else
		{
			// Client window has a taller aspect than desktop; shrink client width to match its height
			clientWidth = clientHeight * m_config.m_clientAspect;
		}
	}
	m_fullScreenDimensions = Vec2(clientWidth, clientHeight);

	// Calculate client rect bounds by centering the client area
	float clientMarginX = 0.5f * (desktopWidth - clientWidth);
	float clientMarginY = 0.5f * (desktopHeight - clientHeight);
	RECT clientRect;
	clientRect.left = (int)clientMarginX;
	clientRect.right = clientRect.left + (int)clientWidth;
	clientRect.top = (int)clientMarginY;
	clientRect.bottom = clientRect.top + (int)clientHeight;

	// Calculate the outer dimensions of the physical window, including frame et. al.
	RECT windowRect = clientRect;

	AdjustWindowRectEx(&windowRect, windowStyleFlags, FALSE, windowStyleExFlags);

	if (m_config.m_Pos != IntVec2(0, 0))
	{
		windowRect.left += m_config.m_Pos.x;
		windowRect.top += m_config.m_Pos.y;
		windowRect.right += m_config.m_Pos.x;
		windowRect.bottom += m_config.m_Pos.y;
	}
	WCHAR windowTitle[1024];
	MultiByteToWideChar(GetACP(), 0, m_config.m_windowTitle.c_str(), -1, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
	m_hwnd = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		(HINSTANCE)NULL,
		NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//g_displayDeviceContext = GetDC(windowHandle);


	HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
	SetCursor(cursor);

}

void Window::RunMessagePump()
{
	MSG queuedMessage;
	for (;; )
	{
		const BOOL wasMessagePresent = PeekMessage(&queuedMessage, NULL, 0, 0, PM_REMOVE);
		if (!wasMessagePresent)
		{
			break;
		}

		TranslateMessage(&queuedMessage);
		DispatchMessage(&queuedMessage); // This tells Windows to call our "WindowsMessageHandlingProcedure" (a.k.a. "WinProc") function
	}
}



