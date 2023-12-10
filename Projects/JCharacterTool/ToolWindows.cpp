#include "pch.h"
#include "ToolWindows.h"
#include "engine/IExecute.h"

ToolWindows::ToolWindows()
{
}

ToolWindows::~ToolWindows()
{
}

void ToolWindows::Update()
{
	MANAGER_TIME()->Update();
	MANAGER_INPUT()->Update();

	GRAPHICS()->RenderBegin();
	_desc.App->Update();
	_desc.App->Render();
	GRAPHICS()->RenderEnd();
}

LRESULT ToolWindows::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProc(handle, message, wParam, lParam);
	}
}

WPARAM ToolWindows::Run(CGameDesc& desc)
{
	_desc = desc;
	assert(_desc.App != nullptr);

	// 1) 윈도우 창 정보 등록
	MyRegisterClass();

	// 2) 윈도우 창 생성
	if (!InitInstance(SW_SHOWNORMAL))
		return FALSE;
	//Global
	g_hWnd = _desc.hWnd;
	g_gameDesc = _desc;
	//Manager Init
	GRAPHICS()->Init();
	MANAGER_TIME()->Init();
	MANAGER_INPUT()->Init();
	//Scene Init
	_desc.App->Init();

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			Update();
		}
	}

	return msg.wParam;
}
