#include "pch.h"
#include "ImGuiManager.h"
#include "GUIAssetReadWrite.h"
#include "GUIAnimationReadWrite.h"

ImGuiManager* ImGuiManager::_instance = nullptr;

ImGuiManager::ImGuiManager()
{

}

ImGuiManager::~ImGuiManager()
{
}

void ImGuiManager::GuiCreate()
{
	int guiSize = static_cast<int>(GUIType::End) - 1;

	_guiList.resize(guiSize);
	//File
	_guiList.push_back(make_shared<GUIAssetReadWrite>());
	_guiList.push_back(make_shared<GUIAnimationReadWrite>());
	//View

	//SceneTest
}

void ImGuiManager::GuiUpdate()
{
	if (ImGui::BeginMainMenuBar())
	{
		//File Section
		if (ImGui::BeginMenu("File"))
		{
			for (int i = 0; i < _guiList.size(); i++)
			{
				if (_guiList[i] != nullptr)
					_guiList[i]->Update();
			}

			ImGui::EndMenu();
		}
		//View Section
		if (ImGui::BeginMenu("View"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("SceneTest"))
		{
			ImGui::EndMenu();
		}
		//MainMenuBar End
		ImGui::EndMainMenuBar();
	}
}

void ImGuiManager::GuiRender()
{
	for (int i = 0; i < _guiList.size(); i++)
	{
		if (_guiList[i] != nullptr)
			_guiList[i]->Render();
	}
}

void ImGuiManager::Init()
{
	//ImGui Main
	{
		// Show the window
		::ShowWindow(g_gameDesc.hWnd, SW_SHOWDEFAULT);
		::UpdateWindow(g_gameDesc.hWnd);
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(g_gameDesc.hWnd);
		ImGui_ImplDX11_Init(DEVICE().Get(), DC().Get());
	}
	//Gui Create
	{
		GuiCreate();
	}
}

void ImGuiManager::Update()
{
	//ImGui Main
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	//Gui Update
	{
		GuiUpdate();
	}
}

void ImGuiManager::Render()
{
	//Gui Render
	{
		GuiRender();
	}
	//ImGui Main
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}
