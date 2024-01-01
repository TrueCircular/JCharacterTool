#include "pch.h"
#include "main.h"
#include "ToolWindows.h"
#include "Demo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	CGameDesc desc;
	desc.AppName = L"JCharacterTool";
	desc.hInstance = hInstance;
	desc.vsync = false;
	desc.hWnd = NULL;
	desc.width = 1600.f;
	desc.height = 900.f;
	desc.clearColor = Color(0.f, 0.5f, 0.f, 0.1f);
	desc.App = make_shared<Demo>();

	ToolWindows* tool = new ToolWindows();

	tool->Run(desc);
	 
	delete tool;
	tool = nullptr;

	return 0;
}