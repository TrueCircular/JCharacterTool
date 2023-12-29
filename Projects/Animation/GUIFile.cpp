#include "pch.h"
#include "GUIFile.h"
#include "engine/Utils.h"
#include "ModelManager.h"

GUIFile::GUIFile() : Super(GUIType::File)
{
	_maxDialogSize = ImVec2(g_gameDesc.width, g_gameDesc.height);
	_minDialogSize = _maxDialogSize;
	_minDialogSize.x *= 0.55f;
	_minDialogSize.y *= 0.4f;

	_readsaveDialogPos.x = 0.f;
	_readsaveDialogPos.y = 18.f;

	_type = AssetType::None;
}

GUIFile::~GUIFile()
{
}

wstring GUIFile::SplitFileName(string name)
{
	string spName = name;
	size_t sp = spName.find_last_of(".");
	wstring rName = Utils::ToWString(spName.substr(0, sp));

	return rName;
}

void GUIFile::SavePoPUP()
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

void GUIFile::Update()
{
	//Begin MainMenu
	if (ImGui::BeginMenu("File"))
	{
		//Model Asset
		{
			ImGui::MenuItem("(Model)", NULL, false, false);

			//Asset Read
			if (ImGui::BeginMenu("Read Model Asset File"))
			{
				if (ImGui::MenuItem("Skeletal"))
				{
					string adr = Utils::ToString(RESOURCES_ADDR_ASSET_SKELETAL);
					_dialog.OpenDialog("ReadModelAssets", "File", ".fbx,.obj",
						adr, 1, nullptr, ImGuiFileDialogFlags_Modal);
					_type = AssetType::Skeletal;
				}
				if (ImGui::MenuItem("Static"))
				{
					string adr = Utils::ToString(RESOURCES_ADDR_ASSET_STATIC);
					_dialog.OpenDialog("ReadModelAssets", "File", ".fbx,.obj",
						adr, 1, nullptr, ImGuiFileDialogFlags_Modal);
				}
				ImGui::EndMenu();
			}

			//----------------------
			ImGui::Separator();
			//----------------------

			//Save Mesh File
			if (ImGui::MenuItem("Save Asset File as Mesh File"))
			{
				_isSaveMesh = true;
			}
		}

		//----------------------
		ImGui::Separator();
		//----------------------

		//Animation Asset
		{
			ImGui::MenuItem("(Animation)", NULL, false, false);

			if (ImGui::MenuItem("Read Animation Asset File"))
			{

			}

			//----------------------
			ImGui::Separator();
			//----------------------

			if (ImGui::MenuItem("Save Asset File as Anim File"))
			{

			}
		}

		//----------------------
		ImGui::Separator();
		//----------------------

		//Effect Asset
		{
			ImGui::MenuItem("(Effect)", NULL, false, false);

		}

		//End MainMenu
		ImGui::EndMenu();
	}
}

void GUIFile::Render()
{
	//PoPUp
	{
		SavePoPUP();
	}

	ImGui::SetNextWindowPos(_readsaveDialogPos);
	if (_dialog.Display("ReadModelAssets", ImGuiWindowFlags_NoCollapse, _minDialogSize, _maxDialogSize))
	{
		if (_dialog.IsOk())
		{
			_filePath = Utils::ToWString(_dialog.GetFilePathName());
			_fileName = SplitFileName(_dialog.GetCurrentFileName());
			AssetPathDesc desc;
			{
				desc.Name = _fileName;
				desc.ReadPath = _filePath;
				desc.SaveMaterialPath = RESOURCES_ADDR_TEXTURE + desc.Name + L"/" + desc.Name;
				switch (_type)
				{
				case AssetType::Skeletal:
					desc.SaveMeshPath = RESOURCES_ADDR_MESH_SKELETAL + desc.Name + L"/" + desc.Name;
					break;
				case AssetType::Static:
					desc.SaveMeshPath = RESOURCES_ADDR_MESH_STATIC + desc.Name + L"/" + desc.Name;
					break;
				}
			}

			MANAGER_MODEL()->Init();
			if (MANAGER_MODEL()->ReadAssetFile(desc))
			{
				_isReadMesh = true;
			}
		}
		_dialog.Close();
	}
	if (_dialog.Display("SaveMesh", ImGuiWindowFlags_NoCollapse, _minDialogSize, _maxDialogSize))
	{
		if (_dialog.IsOk())
		{
			string fileName = _dialog.GetCurrentFileName();
		}
		_dialog.Close();
	}
}

