#include "pch.h"
#include "GUIAssetReadWrite.h"

GUIAssetReadWrite::GUIAssetReadWrite() : Super(GUIType::AssetReadWrite)
{
	_maxDialogSize = ImVec2(g_gameDesc.width, g_gameDesc.height);
	_minDialogSize = _maxDialogSize;
	_minDialogSize.x *= 0.55f;
	_minDialogSize.y *= 0.4f;

	_readsaveDialogPos.x = 0.f;
	_readsaveDialogPos.y = 18.f;
}

GUIAssetReadWrite::~GUIAssetReadWrite()
{
}

void GUIAssetReadWrite::Update()
{
	//Model
	ImGui::MenuItem("(Model)", NULL, false, false);
	//AssetFile Read
	if (ImGui::MenuItem("Read Asset File"))
	{
		_dialog.OpenDialog("ReadModelAssets", "File", ".fbx,.obj",
			"../../Resources/Assets/", 1, nullptr, ImGuiFileDialogFlags_Modal);
	}
	//Save Mesh File
	if (ImGui::MenuItem("Save Mesh File"))
	{

	}
	//Animation

}

void GUIAssetReadWrite::Render()
{
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
