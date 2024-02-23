#include "pch.h"
#include "GUIFile.h"
#include "GUIView.h"
#include "engine/Utils.h"
#include "AssetManager.h"
#include "ImGuiManager.h"

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

MeshPathDesc GUIFile::CreateMeshPathDesc(wstring& fileName, wstring& filePath)
{
	MeshPathDesc desc;
	{
		desc.Name = fileName;
		desc.ReadMeshPath = filePath;
		switch (_type)
		{
		case AssetType::SkeletalMesh:
		{
			desc.SaveMeshPath = RESOURCES_ADDR_MESH_SKELETAL + desc.Name + L"/" + desc.Name + L".mesh";
			desc.SaveMaterialPath = RESOURCES_ADDR_TEXTURE_SKELETAL + desc.Name + L"/" + desc.Name + L".xml";
			desc.Type = ModelType::Skeletal;
		}break;
		case AssetType::StaticMesh:
		{
			desc.SaveMeshPath = RESOURCES_ADDR_MESH_STATIC + desc.Name + L"/" + desc.Name + L".mesh";
			desc.SaveMaterialPath = RESOURCES_ADDR_TEXTURE_STATIC + desc.Name + L"/" + desc.Name + L".xml";
			desc.Type = ModelType::Static;
		}break;
		}
	}

	return desc;
}

AnimPathDesc GUIFile::CreateAnimPathDesc(wstring fileName, wstring filePath, wstring animOwner)
{
	AnimPathDesc desc;
	{
		desc.AnimOwnerName = animOwner;
		desc.AnimName = fileName;
		desc.ReadAnimPath = filePath;
		desc.SaveAnimPath = RESOURCES_ADDR_ANIMATION + desc.AnimOwnerName + L"/" + desc.AnimName + L".anim";
	}

	return desc;
}

wstring GUIFile::SplitFileName(string name)
{
	string spName = name;
	size_t sp = spName.find_last_of(".");
	wstring rName = Utils::ToWString(spName.substr(0, sp));

	return rName;
}

wstring GUIFile::SplitParentFilePath(string path)
{
	string spName = path;
	size_t sp = spName.find_last_of("\\");
	string spName2 = spName.substr(0, sp);
	size_t spName2Size = spName2.size();
	size_t sp2 = spName2.find_last_of("\\") + 1;
	wstring rName = Utils::ToWString(spName2.substr(sp2, sp2 - spName2Size + 1));

	return rName;
}

void GUIFile::MeshReadPoPUp()
{
	if (_isReadMesh)
	{
		ImGui::OpenPopup("Success Mesh Read!");
		if (ImGui::BeginPopupModal("Success Mesh Read!", 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::Button("OK", ImVec2(120.f, 20.f)))
			{
				_isReadMesh = false;

				MANAGER_IMGUI()->GetGui<GUIView>()->_showCameraWindow = true;
				MANAGER_IMGUI()->GetGui<GUIView>()->_showLoadedAsset = true;
				MANAGER_IMGUI()->GetGui<GUIView>()->_showScene = true;
				MANAGER_IMGUI()->GetGui<GUIView>()->_showBoneHierarchy = true;
				MANAGER_IMGUI()->GetGui<GUIView>()->_showInspector = true;

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void GUIFile::MeshSavePoPUp()
{
	if (_isSaveMesh)
	{
		ImGuiWindowFlags saveFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;

		ImGui::OpenPopup("Save Mesh");
		if (ImGui::BeginPopupModal("Save Mesh", 0, saveFlags))
		{
			//Save Address
			string adr;
			static int _currentModelComboIndex = 0;

			//Select ComboBox
			if (_type == AssetType::SkeletalMesh)
			{
				//Set Save Address
				adr = Utils::ToString(RESOURCES_ADDR_MESH_SKELETAL);

				//Load SkeletalMeshList
				const auto& SkeletalMeshList = MANAGER_ASSET()->GetLoadedSkeletalMeshDataList();

				//Mesh Names Push
				vector<string> meshNames;
				if (SkeletalMeshList.size() > 0)
				{
					meshNames.reserve(SkeletalMeshList.size());
					{
						for (const auto& asset : SkeletalMeshList)
						{
							meshNames.push_back(Utils::ToString(asset.second.Name));
						}
					}
				}
				const char* previewMeshName = meshNames[_currentModelComboIndex].c_str();

				//Render Combobox
				if (ImGui::BeginCombo("Select Model", previewMeshName))
				{
					for (int n = 0; n < meshNames.size(); n++)
					{
						const bool is_selected = (_currentModelComboIndex == n);
						if (ImGui::Selectable(meshNames[n].c_str(), is_selected))
							_currentModelComboIndex = n;

						if (is_selected)
							ImGui::SetItemDefaultFocus();

					}
					ImGui::EndCombo();
				}

			}
			else if (_type == AssetType::StaticMesh)
			{
				adr = Utils::ToString(RESOURCES_ADDR_MESH_SKELETAL);

				const auto& StaticMeshList = MANAGER_ASSET()->GetLoadedStaticMeshDataList();

				vector<string> meshNames;
				if (StaticMeshList.size() > 0)
				{
					meshNames.reserve(StaticMeshList.size());
					{
						for (const auto& asset : StaticMeshList)
						{
							meshNames.push_back(Utils::ToString(asset.second.Name));
						}
					}
				}
			}

			//Save Button
			if (ImGui::Button("Save", ImVec2(150.f, 20.f)))
			{
				_dialog.OpenDialog("SaveMesh", "Choose a Directory", ".mesh", adr,
					"enter a file name", 1, nullptr, ImGuiFileDialogFlags_Modal);

				_isSaveMesh = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();
			
			//Close Button
			if (ImGui::Button("Close", ImVec2(150.f, 20.f)))
			{
				_isSaveMesh = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void GUIFile::AnimationReadPoPUp()
{
	if (_isReadAnimationAsset)
	{
		ImGui::OpenPopup("Success Mesh Read!");
		if (ImGui::BeginPopupModal("Success Mesh Read!", 0, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::Button("OK", ImVec2(120.f, 20.f)))
			{
				_isReadAnimationAsset = false;

				MANAGER_IMGUI()->GetGui<GUIView>()->_showAnimation = true;

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void GUIFile::AnimationSavePoPUp()
{
}

void GUIFile::AssetListTab()
{
}

void GUIFile::Update()
{
	//Begin MainMenu
	if (ImGui::BeginMenu("File"))
	{
		//Model Asset
		{
			ImGui::MenuItem("(Mesh)", NULL, false, false);

			//Asset Read
			if (ImGui::BeginMenu("Read Mesh Asset File"))
			{
				if (ImGui::MenuItem("Skeletal"))
				{
					//Dialog Open
					string adr = Utils::ToString(RESOURCES_ADDR_ASSET_SKELETAL);
					_dialog.OpenDialog("ReadModelAssets", "Read", ".fbx,.FBX,.obj",
						adr, 1, nullptr, ImGuiFileDialogFlags_Modal);
					//Asset Type Set
					_type = AssetType::SkeletalMesh;
				}
				if (ImGui::MenuItem("Static"))
				{
					//Dialog Open
					string adr = Utils::ToString(RESOURCES_ADDR_ASSET_STATIC);
					_dialog.OpenDialog("ReadModelAssets", "Read", ".fbx,.FBX,.obj",
						adr, 1, nullptr, ImGuiFileDialogFlags_Modal);
					//Asset Type Set
					_type = AssetType::StaticMesh;
				}
				ImGui::EndMenu();
			}

			//----------------------
			ImGui::Separator();
			//----------------------

			//Save Mesh File
			if (ImGui::BeginMenu("Save Mesh File"))
			{
				if (ImGui::MenuItem("Skeletal"))
				{
					_type = AssetType::SkeletalMesh;
					_isSaveMesh = true;
				}
				if (ImGui::MenuItem("Static"))
				{
					_type = AssetType::StaticMesh;
					_isSaveMesh = true;
				}
				ImGui::EndMenu();
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
				string adr = Utils::ToString(RESOURCES_ADDR_ASSET_ANIMATION);
				_dialog.OpenDialog("ReadAnimAsset", "Read", ".fbx,.FBX",
					adr, 1, nullptr, ImGuiFileDialogFlags_Modal);
			}

			//----------------------
			ImGui::Separator();
			//----------------------

			if (ImGui::MenuItem("Read Animation Anim File"))
			{
				string adr = Utils::ToString(RESOURCES_ADDR_ANIMATION);
				_dialog.OpenDialog("ReadAnim", "Read", ".fbx,.FBX",
					adr, 1, nullptr, ImGuiFileDialogFlags_Modal);
			}
		}

		//----------------------
		ImGui::Separator();
		//----------------------

		//Effect Asset
		{
			ImGui::MenuItem("(Effect)", NULL, false, false);

			if (ImGui::MenuItem("Read Effect Asset File"))
			{

			}

			if (ImGui::MenuItem("Save Effect File"))
			{

			}
		}

		//End MainMenu
		ImGui::EndMenu();
	}
}

void GUIFile::Render()
{
	//PoPUp
	{
		MeshReadPoPUp();
		MeshSavePoPUp();
		AnimationReadPoPUp();
	}

	ImGui::SetNextWindowPos(_readsaveDialogPos);

	if (_dialog.Display("ReadModelAssets", ImGuiWindowFlags_NoCollapse, _minDialogSize, _maxDialogSize))
	{
		if (_dialog.IsOk())
		{
			_filePath = Utils::ToWString(_dialog.GetFilePathName());
			_fileName = SplitFileName(_dialog.GetCurrentFileName());

			MeshPathDesc desc = CreateMeshPathDesc(_fileName, _filePath);

			if (MANAGER_ASSET()->ReadMeshAssetFile(desc))
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
			_filePath = Utils::ToWString(_dialog.GetCurrentPath());
			_fileName = SplitFileName(_dialog.GetCurrentFileName());
		}
		_dialog.Close();
	}

	if (_dialog.Display("ReadAnimAsset", ImGuiWindowFlags_NoCollapse, _minDialogSize, _maxDialogSize))
	{
		if (_dialog.IsOk())
		{
			_fileName = SplitFileName(_dialog.GetCurrentFileName());
			_filePath = Utils::ToWString(_dialog.GetFilePathName());
			_fileParentPath = SplitParentFilePath(_dialog.GetFilePathName());

			AnimPathDesc desc = CreateAnimPathDesc(_fileName, _filePath, _fileParentPath);

			if (MANAGER_ASSET()->ReadAnimAssetFile(desc))
			{
				_isReadAnimationAsset = true;
			}
		}
		_dialog.Close();
	}
}

