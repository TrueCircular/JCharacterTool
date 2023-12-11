#pragma once

class ImGuiManager
{
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
private:
	bool show_demo_window = true;
	bool show_another_window = false;
private:
	void DefaultTest();
public:
	void Init();
	void Update();
	void Render();
};

#define MANAGER_IMGUI() ImGuiManager::GetInstance()
