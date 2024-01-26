#include "DevConsole.hpp"
#include "Stopwatch.hpp"
#include "VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "../Math/AABB2.hpp"
#include <string>
#include <vector>
#include "../Renderer/SimpleTriangleFont.hpp"
#include "../Renderer/BitmapFont.hpp"
#include "Engine/Input/InputSystem.hpp"

extern Renderer* g_theRenderer;

BitmapFont* g_devFont = nullptr;

const Rgba8 DevConsole::ERRORMSG	 = Rgba8(255, 0, 0, 255);
const Rgba8 DevConsole::ECHOMSG		 = Rgba8(125, 0, 125, 255);
const Rgba8 DevConsole::WARNING		 = Rgba8(255, 255, 0, 255);
const Rgba8 DevConsole::INFO_MAJOR   = Rgba8(0, 255,255,255);
const Rgba8 DevConsole::INFO_MINOR   = Rgba8(125, 205, 205, 255);
const Rgba8 DevConsole::COMMAND_ECHO = Rgba8(255, 0, 255, 255);
const Rgba8 DevConsole::INPUT_TEXT	 = Rgba8(255, 255, 255, 255);
const Rgba8 DevConsole::INPUT_CARET  = Rgba8(255, 255, 255, 255);

constexpr float FONT_HEIGHT = 20.f;
constexpr float CARET_WIDTH = 2.f;

DevConsole::DevConsole(DevConsoleConfig const& config)
	:m_config(config)
{
	m_caretStopwatch = new Stopwatch(1.f);
}

DevConsole::~DevConsole()
{
	delete m_caretStopwatch;
	m_caretStopwatch = nullptr;
	g_devFont = nullptr;
}

void DevConsole::Startup()
{
	m_commandHistory.reserve(m_config.m_maxCommandHistory);
	std::string FontAddress = "Data/Fonts/";
	FontAddress.append(m_config.m_fontName);
 	g_devFont = g_theRenderer->CreateOrGetBitmapFont(FontAddress.c_str());
 	SubscribeEventCallbackFunction("CharInput", Event_CharInput);
	SubscribeEventCallbackFunction("KeyPressed", Event_KeyPressed);
	SubscribeEventCallbackFunction("errorMessage", Event_ErrorMsg);
	SubscribeEventCallbackFunction("clear", Command_Clear);
	SubscribeEventCallbackFunction("help", Command_Help);
	SubscribeEventCallbackFunction("Echo", Event_Echo);
	
	AddLine(DevConsole::INFO_MINOR, "Type help for a list of commands:");
}

void DevConsole::Shutdown()
{

}

void DevConsole::BeginFrame()
{
	if (m_caretStopwatch->DecrementDurationIfElapsed())
	{
		m_caretStopwatch->Restart();
	}
	if (m_caretStopwatch->GetElapsedFration() < 0.5f)
	{
		m_caretVisible = true;
	}
	else if(m_caretStopwatch->GetElapsedFration() >= 0.5f)
	{
		m_caretVisible = false;
	}
}

void DevConsole::EndFrame()
{

}

void DevConsole::Execute(std::string const& consoleCommandText, bool isFiredRomotely)
{
	m_commandHistory.push_back(consoleCommandText);
	m_caretPosition = 0;
	m_historyIndex = -1;

	EventArgs args;
	Strings nameAndArgs;
	nameAndArgs = SplitStringWithQuotes(consoleCommandText, ' ');
	if (nameAndArgs.size() == 2)
	{
		Strings argsNameAndValue;
		argsNameAndValue = SplitStringWithQuotes(nameAndArgs[1], '=');
		if (argsNameAndValue.size() == 2)
		{
			args.SetValue(argsNameAndValue[0], argsNameAndValue[1]);
		}
	}

	bool isEventFired = FireEvent(nameAndArgs[0], args);
	if (isEventFired && !isFiredRomotely)
	{
		g_theDevConsole->AddLine(DevConsole::COMMAND_ECHO, consoleCommandText);
	}
	m_inputText.clear();
}

void DevConsole::ExecuteWithoutRender(std::string const& consoleCommandText, bool isFiredRomotely /*= false*/)
{
	m_commandHistory.push_back(consoleCommandText);
	m_caretPosition = 0;
	m_historyIndex = -1;

	EventArgs args;
	Strings nameAndArgs;
	nameAndArgs = SplitStringWithQuotes(consoleCommandText, ' ');
	if (nameAndArgs.size() == 2)
	{
		Strings argsNameAndValue;
		argsNameAndValue = SplitStringWithQuotes(nameAndArgs[1], '=');
		if (argsNameAndValue.size() == 2)
		{
			args.SetValue(argsNameAndValue[0], argsNameAndValue[1]);
		}
	}

	bool isEventFired = FireEvent(nameAndArgs[0], args);
	if (isEventFired && !isFiredRomotely)
	{
		//g_theDevConsole->AddLine(DevConsole::COMMAND_ECHO, consoleCommandText);
	}
	m_inputText.clear();
}

void DevConsole::AddLine(Rgba8 const& color, std::string const& text)
{
	DevConsoleLine newLine;
	newLine.m_color = color;
	newLine.m_text = text;
	m_lines.push_back(newLine);
}

void DevConsole::Render(AABB2 const& bounds)
{
	if (m_isOpen)
	{
		m_config.m_renderer->SetBlendMode(BlendMode::ALPHA);
		m_config.m_renderer->BindShader(nullptr);
		m_config.m_renderer->BindTexture(nullptr);
		std::vector<Vertex_PCU> vertsList;
		AddVertsForAABB2(vertsList, bounds, Rgba8(0,0,0,200));
		if (m_caretVisible)
		{
			AddVertsForAABB2(vertsList, AABB2(m_config.m_camera->GetOrthoBottomLeft() + Vec2(FONT_HEIGHT * m_caretPosition * m_config.m_fontAspect, 0.f), 
									          m_config.m_camera->GetOrthoBottomLeft() + Vec2(FONT_HEIGHT * m_caretPosition * m_config.m_fontAspect + CARET_WIDTH, FONT_HEIGHT)),
											  INPUT_CARET);
		}
		m_config.m_renderer->DrawVertexArray((int)vertsList.size(), vertsList.data());

		std::vector<Vertex_PCU> fontList;
		m_config.m_renderer->SetBlendMode(BlendMode::ADDITIVE);
		m_config.m_renderer->BindShader(nullptr);
		m_config.m_renderer->BindTexture(&(g_devFont->GetTexture()));
		for (int lineIndex = 0; lineIndex < m_lines.size(); ++lineIndex)
		{
			float bottomLeftOfLine = m_config.m_camera->GetOrthoBottomLeft().y + (FONT_HEIGHT) * (((int)m_lines.size() - 1 - lineIndex) + 1);
			if (bottomLeftOfLine < m_config.m_linesOnScreen * FONT_HEIGHT)
			{
				g_devFont->AddVertsForTextInBox2D(fontList
					, AABB2(m_config.m_camera->GetOrthoBottomLeft() + Vec2(0.f, (FONT_HEIGHT) * (((int)m_lines.size() - 1 - lineIndex) + 1))
					, Vec2(m_config.m_camera->GetOrthoTopRight().x, FONT_HEIGHT) + Vec2(0.f, (FONT_HEIGHT) * (((int)m_lines.size() - 1 - lineIndex) + 1)))
					, FONT_HEIGHT, m_lines[lineIndex].m_text, m_lines[lineIndex].m_color, m_config.m_fontAspect, Vec2(0.f, 0.5f));
			}
		}
		g_devFont->AddVertsForTextInBox2D(fontList, AABB2(m_config.m_camera->GetOrthoBottomLeft(), Vec2(m_config.m_camera->GetOrthoTopRight().x, FONT_HEIGHT))
									    , FONT_HEIGHT, m_inputText, INPUT_TEXT,m_config.m_fontAspect, Vec2(0.f,0.5f));
		m_config.m_renderer->DrawVertexArray((int)fontList.size(), fontList.data());
	}
}

void DevConsole::ToggleOpen()
{
	m_isOpen = !m_isOpen;
	m_caretStopwatch->Start();
}

bool DevConsole::IsOpen() const
{
	return m_isOpen;
}

bool DevConsole::Event_KeyPressed(EventArgs& args)
{
	if (!g_theDevConsole->m_isOpen)
	{
		return false;
	}

	g_theDevConsole->m_caretStopwatch->Start();

	if (args.GetValue("KeyCode", -1) == KEYCODE_LEFTARROW)
	{
		if (g_theDevConsole->m_caretPosition > 0)
		{
			g_theDevConsole->m_caretPosition--;
		}
	}
	else if (args.GetValue("KeyCode", -1) == KEYCODE_RIGHTARROW)
	{
		if (g_theDevConsole->m_caretPosition < g_theDevConsole->m_inputText.size())
		{
			g_theDevConsole->m_caretPosition++;
		}
	}
	else if (args.GetValue("KeyCode", -1) == KEYCODE_UPARROW)
	{
		if (g_theDevConsole->m_historyIndex >= -1)
		{
			g_theDevConsole->m_historyIndex++;
			
			if ((int)g_theDevConsole->m_commandHistory.size() - 1 - g_theDevConsole->m_historyIndex >= 0)
			{
				int commandIndex = (int)g_theDevConsole->m_commandHistory.size() - 1 - g_theDevConsole->m_historyIndex;
				g_theDevConsole->m_inputText = g_theDevConsole->m_commandHistory[commandIndex];
				g_theDevConsole->m_caretPosition = (int)g_theDevConsole->m_inputText.length();
			}
			else
			{
				g_theDevConsole->m_historyIndex--;
			}
		}
	}
	else if (args.GetValue("KeyCode", -1) == KEYCODE_DOWNARROW)
	{
		if (g_theDevConsole->m_historyIndex > -1)
		{
			g_theDevConsole->m_historyIndex--;
			if (g_theDevConsole->m_historyIndex == -1)
			{
				g_theDevConsole->m_inputText.clear();
				g_theDevConsole->m_caretPosition = (int)g_theDevConsole->m_inputText.length();
			}
			else
			{
				int commandIndex = (int)g_theDevConsole->m_commandHistory.size() - 1 - g_theDevConsole->m_historyIndex;
				g_theDevConsole->m_inputText = g_theDevConsole->m_commandHistory[commandIndex];
				g_theDevConsole->m_caretPosition = (int)g_theDevConsole->m_inputText.length();
			}
		}
	}
	else if (args.GetValue("KeyCode", -1) == KEYCODE_TILDE)
	{
		g_theDevConsole->ToggleOpen();
	}
	else if (args.GetValue("KeyCode", -1) == KEYCODE_ENTER)
	{
		if (g_theDevConsole->m_inputText.length() > 0)
		{
			g_theDevConsole->Execute(g_theDevConsole->m_inputText);
		}
		else
		{
			g_theDevConsole->ToggleOpen();
		}
	}
	else if (args.GetValue("KeyCode", -1) == KEYCODE_BACKSPACE)
	{
		if (g_theDevConsole->m_inputText.length() != 0 && g_theDevConsole->m_caretPosition != 0)
		{
			g_theDevConsole->m_inputText.erase((size_t)g_theDevConsole->m_caretPosition - 1, 1);
			g_theDevConsole->m_caretPosition--;
		}
	}
	else if (args.GetValue("KeyCode", -1) == ESC_KEY)
	{
		if (g_theDevConsole->m_inputText.length() == 0)
		{
			g_theDevConsole->ToggleOpen();
		}
		else
		{
			g_theDevConsole->m_inputText.clear();
			g_theDevConsole->m_caretPosition = 0;
		}
	}
	else if (args.GetValue("KeyCode", -1) == KEYCODE_HOME)
	{
		g_theDevConsole->m_caretPosition = 0;
	}
	else if (args.GetValue("KeyCode", -1) == KEYCODE_END)
	{
		g_theDevConsole->m_caretPosition = (int)g_theDevConsole->m_inputText.length();
	}
	else if (args.GetValue("KeyCode", -1) == KEYCODE_DELETE)
	{
		g_theDevConsole->m_inputText.erase((size_t)g_theDevConsole->m_caretPosition, 1);
	}
	return true;
}

bool DevConsole::Event_CharInput(EventArgs& args)
{
	if (g_theDevConsole->m_isOpen)
	{
		if ((unsigned char)args.GetValue("char", -1) >= 32 && (unsigned char)args.GetValue("char", -1) <= 126)
		{
			if ((unsigned char)args.GetValue("char", -1) == KEYCODE_TILDE)
			{
				return false;
			}
			g_theDevConsole->m_inputText.insert(g_theDevConsole->m_caretPosition, 1, (unsigned char)args.GetValue("char", -1));
			g_theDevConsole->m_caretPosition++;
			g_theDevConsole->m_caretStopwatch->Start();
			return true;
		}
	}
	return true;
}

bool DevConsole::Command_Clear(EventArgs& args)
{
	if (!g_theDevConsole->m_isOpen)
	{
		return false;
	}
	UNUSED(args);
	g_theDevConsole->m_lines.clear();
	return true;
}

bool DevConsole::Command_Help(EventArgs& args)
{
	if (!g_theDevConsole->m_isOpen)
	{
		return false;
	}
	UNUSED(args);
	std::vector<std::string> listName;
	GetAllRegisteredCommandsName(listName);
	g_theDevConsole->AddLine(DevConsole::INFO_MAJOR, "Registered Commands");
	for (int nameIndex = 0; nameIndex < listName.size(); ++nameIndex)
	{
		g_theDevConsole->AddLine(DevConsole::INFO_MINOR, listName[nameIndex]);
	}
	return true;
}

bool DevConsole::Event_ErrorMsg(EventArgs& args)
{
	if (!g_theDevConsole->m_isOpen)
	{
		return false;
	}
	std::string msg;
	msg.append("Unknown command: ");
	msg.append(args.GetValue("message", ""));
	g_theDevConsole->AddLine(DevConsole::ERRORMSG, msg);
	return true;
}

bool DevConsole::Event_Echo(EventArgs& args)
{
	std::string msg;
	msg.append(args.GetValue("Message", ""));
	g_theDevConsole->AddLine(DevConsole::ECHOMSG, msg);
	return true;
}

