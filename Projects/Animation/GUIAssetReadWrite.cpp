#include "pch.h"
#include "GUIAssetReadWrite.h"

extern ImGuiID ImHashStr(const char* data_p, size_t data_size, ImGuiID seed);


GUIAssetReadWrite::GUIAssetReadWrite() : Super(GUIType::AssetReadWrite)
{
	_maxDialogSize = ImVec2(g_gameDesc.width, g_gameDesc.height);
	_minDialogSize = _maxDialogSize;
	_minDialogSize.x *= 0.55f;
	_minDialogSize.y *= 0.4f;

	_readsaveDialogPos.x = 0.f;
	_readsaveDialogPos.y = 18.f;

	_popupId = ImHashStr("POPUP", sizeof("POPUP"), _popupId);
}

GUIAssetReadWrite::~GUIAssetReadWrite()
{
}

void GUIAssetReadWrite::Update()
{
	//Model
	ImGui::MenuItem("(Model)", NULL, false, false);
	//AssetFile Read
	if (ImGui::BeginMenu("Read Model Asset File"))
	{
		if (ImGui::MenuItem("Skeletal"))
		{
			_dialog.OpenDialog("ReadModelAssets", "File", ".fbx,.obj",
				"../../Resources/Assets/Skeletal/", 1, nullptr, ImGuiFileDialogFlags_Modal);
		}
		if (ImGui::MenuItem("Static"))
		{
			_dialog.OpenDialog("ReadModelAssets", "File", ".fbx,.obj",
				"../../Resources/Assets/Static/", 1, nullptr, ImGuiFileDialogFlags_Modal);
		}
		ImGui::EndMenu();
	}

	ImGui::Separator();

	//Save Mesh File

	if (ImGui::MenuItem("Save Asset File as Mesh File"))
	{
		_button = true;
	}

	ImGui::Separator();

	if (ImGui::MenuItem("Read Mesh File and Show Model"))
	{

	}
	ImGui::Separator();


}

void GUIAssetReadWrite::Render()
{
	if (_button)
	{
		ImGui::OpenPopup("Are you going to save that Mesh?");
		if (ImGui::BeginPopupModal("Are you going to save that Mesh?", 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::Button("Save", ImVec2(120.f, 20.f)))
			{
				_button = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Close", ImVec2(120.f, 20.f)))
			{
				_button = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	ImGui::SetNextWindowPos(_readsaveDialogPos);
	if (_dialog.Display("ReadModelAssets", ImGuiWindowFlags_NoCollapse, _minDialogSize, _maxDialogSize))
	{
		if (_dialog.IsOk())
		{
			string filePathName = _dialog.GetFilePathName();
			string filePath = _dialog.GetCurrentPath();
		}
		_dialog.Close();
	}
}
