#pragma once
#include "GUIInterface.h"

class ImGuiManager
{
	using GuiList = vector<shared_ptr<GUIInterface>>;
private:
	static ImGuiManager* _instance;
public:
	static ImGuiManager* GetInstance()
	{
		if (_instance == nullptr)
			_instance = new ImGuiManager();

		return _instance;
	}
private:
	ImGuiManager();
	~ImGuiManager();
public:
	void Init();
	void Update();
	void Render();
};

#define MANAGER_IMGUI() ImGuiManager::GetInstance()

