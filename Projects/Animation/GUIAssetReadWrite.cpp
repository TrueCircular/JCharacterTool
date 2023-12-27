#include "pch.h"
#include "GUIAssetReadWrite.h"
#include "engine/Utils.h"
#include "ModelManager.h"

GUIAssetReadWrite::GUIAssetReadWrite() : Super(GUIType::AssetReadWrite)
{
	_maxDialogSize = ImVec2(g_gameDesc.width, g_gameDesc.height);
	_minDialogSize = _maxDialogSize;
	_minDialogSize.x *= 0.55f;
	_minDialogSize.y *= 0.4f;

	_readsaveDialogPos.x = 0.f;
	_readsaveDialogPos.y = 18.f;

	_hierarchySize.x = 350;
	_hierarchySize.y = g_gameDesc.height;
}

GUIAssetReadWrite::~GUIAssetReadWrite()
{
}

wstring GUIAssetReadWrite::SplitFileName(string name)
{
	string spName = name;
	size_t sp = spName.find_last_of(".");
	wstring rName = Utils::ToWString(spName.substr(0, sp));

	return rName;
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

void GUIAssetReadWrite::BoneHierarchy()
{
	if (_isReadMesh)
	{
		ImGui::SetNextWindowPos(_readsaveDialogPos);
		ImGui::SetNextWindowSize(_hierarchySize);

		if (ImGui::Begin("Bone Hierarchy"))
		{
			const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;

			if (ImGui::TreeNode("Bone List"))
			{
				static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
				static ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns;
				ImGui::CheckboxFlags("SpanAllColumns", &tree_node_flags, ImGuiTreeNodeFlags_SpanAllColumns);
				ImGui::CheckboxFlags("SpanFullWidth", &tree_node_flags, ImGuiTreeNodeFlags_SpanFullWidth);

				if (ImGui::BeginTable("3ways", 2, flags))
				{
					ImGui::TableSetupColumn("Number", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 5.f);
					ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
					ImGui::TableHeadersRow();

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGuiWindowFlags childFlags = ImGuiWindowFlags_AlwaysHorizontalScrollbar |
						ImGuiWindowFlags_AlwaysVerticalScrollbar;
					if (ImGui::TreeNodeEx("hi", tree_node_flags))
					{
						ImGui::TableNextColumn();
						ImGui::Selectable("sibal");
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Selectable("rgasd");

						ImGui::Selectable("sibsdsal");

						//Ex End
						ImGui::TreePop();

					}
					ImGui::EndTable();
				}
				//Hierarchy End
				ImGui::TreePop();
			}
			if (ImGui::CollapsingHeader("Bone Info"))
			{

			}
			//Bone Hierarchy End
			ImGui::End();
		}
	}
}

void GUIAssetReadWrite::MaterialView()
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
}

void GUIAssetReadWrite::Render()
{
	//PoPUp
	{
		SavePoPUP();
	}
	//RelativeGUI
	{
		BoneHierarchy();
	}

	ImGui::SetNextWindowPos(_readsaveDialogPos);
	if (_dialog.Display("ReadModelAssets", ImGuiWindowFlags_NoCollapse, _minDialogSize, _maxDialogSize))
	{
		if (_dialog.IsOk())
		{
			_filePath = Utils::ToWString(_dialog.GetFilePathName());
			_fileName = SplitFileName(_dialog.GetCurrentFileName());

			MANAGER_MODEL()->Init();
			if (MANAGER_MODEL()->ReadAssetFile(_filePath))
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

