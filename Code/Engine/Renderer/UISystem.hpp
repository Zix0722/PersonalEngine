#pragma once

class Renderer;
class Window;

class UISystem
{
public:
	void Startup(Renderer* render, Window* window);
	void BeginFrame();
	void Render();
	void Shutdown();

};