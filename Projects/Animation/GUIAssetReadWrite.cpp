#include "pch.h"
#include "GUIAssetReadWrite.h"
#include "engine/Utils.h"

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

void GUIAssetReadWrite::SavePoPUP()
{
	if (_isSaveMesh)
	{
		ImGui::OpenPopup("Are you going to save that Mesh?");
		if (ImGui::BeginPopupModal("Are you going to save that Mesh?", 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::Button("Save", ImVec2(120.f, 20.f)))
			{
				_isSaveMesh = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Close", ImVec2(120.f, 20.f)))
			{
				_isSaveMesh = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
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
			string adr = Utils::ToString(RESOURCES_ADDR_ASSET_SKELETAL);
			_dialog.OpenDialog("ReadModelAssets", "File", ".fbx,.obj",
				adr, 1, nullptr, ImGuiFileDialogFlags_Modal);
		}
		if (ImGui::MenuItem("Static"))
		{
			string adr = Utils::ToString(RESOURCES_ADDR_ASSET_STATIC);
			_dialog.OpenDialog("ReadModelAssets", "File", ".fbx,.obj",
				adr, 1, nullptr, ImGuiFileDialogFlags_Modal);
		}
		ImGui::EndMenu();
	}

	ImGui::Separator();

	//Save Mesh File

	if (ImGui::MenuItem("Save Asset File as Mesh File"))
	{
		_isSaveMesh = true;
	}

	ImGui::Separator();

	if (ImGui::BeginMenu("Read Mesh File and Show Model"))
	{
		if (ImGui::MenuItem("Skeletal"))
		{
			string adr = Utils::ToString(RESOURCES_ADDR_MESH_SKELETAL);
			_dialog.OpenDialog("ReadMesh", "File", ".mesh",
				adr, 1, nullptr, ImGuiFileDialogFlags_Modal);
		}
		if (ImGui::MenuItem("Static"))
		{
			string adr = Utils::ToString(RESOURCES_ADDR_MESH_STATIC);
			_dialog.OpenDialog("ReadMesh", "File", ".mesh",
				adr, 1, nullptr, ImGuiFileDialogFlags_Modal);
		}

		ImGui::EndMenu();
	}

	ImGui::Separator();


}

void GUIAssetReadWrite::Render()
{
	SavePoPUP();


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
	if (_dialog.Display("ReadMesh", ImGuiWindowFlags_NoCollapse, _minDialogSize, _maxDialogSize))
	{
		if (_dialog.IsOk())
		{
			string fileName = _dialog.GetCurrentFileName();
		}
		_dialog.Close();
	}
}
