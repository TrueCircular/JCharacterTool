#pragma once
#include "GUIInterface.h"

#pragma region Declaration
class GUIAssetReadWrite;
class GUIAnimationReadWrite;
#pragma endregion

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
private:
	bool _isRunning = false;
	GuiList _guiList;
private:
	void GuiCreate();
	void GuiUpdate();
	void GuiRender();
public:
	void GUIRunning(bool run) { _isRunning = run; }
public:
	void Init();
	void Update();
	void Render();
};

#define MANAGER_IMGUI() ImGuiManager::GetInstance()
