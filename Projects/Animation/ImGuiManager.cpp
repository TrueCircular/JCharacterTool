#include "pch.h"
#include "ImGuiManager.h"
#include "GUIFile.h"
#include "GUIView.h"

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

	_guiList.reserve(guiSize);
	//File
	_guiList.push_back(make_shared<GUIFile>());
	//View
	_guiList.push_back(make_shared<GUIView>());
	//SceneTest
}

void ImGuiManager::GuiUpdate()
{
	if (ImGui::BeginMainMenuBar())
	{
		//File Section
		if (ImGui::BeginMenu("File"))
		{
			_guiList[0]->Update();

			ImGui::EndMenu();
		}
		//View Section
		if (ImGui::BeginMenu("View"))
		{
			_guiList[1]->Update();

			ImGui::EndMenu();
		}
		//Scene Test Section
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

const float* ImGuiManager::ConvertMatrixToFloat(Matrix& mat)
{
	float ReturnFloat[16] = { 0, };

	ReturnFloat[0] = mat._11; ReturnFloat[1] = mat._12; ReturnFloat[2] = mat._13; ReturnFloat[3] = mat._14;
	ReturnFloat[4] = mat._21; ReturnFloat[5] = mat._22; ReturnFloat[6] = mat._23; ReturnFloat[7] = mat._24;
	ReturnFloat[8] = mat._31; ReturnFloat[9] = mat._32; ReturnFloat[10] = mat._33; ReturnFloat[11] = mat._34;
	ReturnFloat[12] = mat._41; ReturnFloat[13] = mat._42; ReturnFloat[14] = mat._43; ReturnFloat[15] = mat._44;

	return ReturnFloat;
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
		ImGuizmo::BeginFrame();

		//ImGuizmo::Enable(true);
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
