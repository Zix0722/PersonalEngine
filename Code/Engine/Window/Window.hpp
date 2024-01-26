#pragma once
#include "Engine/Core/EngineCommon.hpp"
class InputSystem;
struct Vec2;


struct WindowConfig
{
	InputSystem* m_inputSystem = nullptr;
	std::string	 m_windowTitle = "Untitled App";
	float		 m_clientAspect = 2.0f;
	bool		 m_isFullscreen = false;
	IntVec2		 m_Size = IntVec2(0, 0);
	IntVec2		 m_Pos = IntVec2(0, 0);

};
//----------------------------------------------
class Window
{
public:
	Window(WindowConfig const& config);
	~Window();

	void Startup();
	void BeginFrame();
	void EndFrame();
	void Shutdown();

	WindowConfig& GetConfig() { return m_config; }
	Vec2				GetCursorNormalizeUVs() const;
	static Window* GetWindowContext() { return s_mainWindow; }
	void* GetHwnd() const;
	IntVec2 GetClientDimensions() const;
	bool IsFocusingWindow() const;
	Vec2 GetMouseOffset() const;
	bool GetXMLPathByOpenFileNameA(std::string& filePath, bool is3D = true);
	Vec2 GetCursorClientPos();

	Vec2 m_fullScreenDimensions;
protected:
	void CreateOSWindow();
	void RunMessagePump();

protected:
	WindowConfig	m_config;
	static Window*	s_mainWindow;
	HWND m_hwnd = nullptr;

};