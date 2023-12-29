#include "pch.h"
#include "GUIView.h"

GUIView::GUIView() : Super(GUIType::View)
{
	_loadedAssetPos.x = g_gameDesc.width - 350.f;
	_loadedAssetPos.y = 18.f;

	_boneHierarchyPos.x = 0.f;
	_boneHierarchyPos.y = 18.f;
	_boneHierarchySize.x = 350.f;
	_boneHierarchySize.y = g_gameDesc.height;
}

GUIView::~GUIView()
{
}

void GUIView::DrawGrid()
{
}

void GUIView::LoadedAsset()
{
}

void GUIView::BoneHierarchy()
{
	if (_showBoneHierarchy)
	{
		ImGui::SetNextWindowPos(_boneHierarchyPos);
		ImGui::SetNextWindowSize(_boneHierarchySize);

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

void GUIView::Transform()
{
}

void GUIView::Material()
{
}

void GUIView::Animation()
{
}

void GUIView::Lighting()
{
}

const float* GUIView::ConvertMatrixToFloat(Matrix& mat)
{
	float ReturnFloat[16] = { 0, };

	ReturnFloat[0] = mat._11; ReturnFloat[1] = mat._12; ReturnFloat[2] = mat._13; ReturnFloat[3] = mat._14;
	ReturnFloat[4] = mat._21; ReturnFloat[5] = mat._22; ReturnFloat[6] = mat._23; ReturnFloat[7] = mat._24;
	ReturnFloat[8] = mat._31; ReturnFloat[9] = mat._32; ReturnFloat[10] = mat._33; ReturnFloat[11] = mat._34;
	ReturnFloat[12] = mat._41; ReturnFloat[13] = mat._42; ReturnFloat[14] = mat._43; ReturnFloat[15] = mat._44;

	return ReturnFloat;
}

void GUIView::Update()
{
	//Begin MainMenu
	if (ImGui::BeginMenu("View"))
	{

		if (ImGui::MenuItem("Loaded AssetList",NULL, _showLoadedAsset))
		{
			if (_showLoadedAsset)
			{
				_showLoadedAsset = false;
			}
			else
			{
				_showLoadedAsset = true;
			}
		}

		if (ImGui::MenuItem("BoneHierarchy", NULL, _showBoneHierarchy))
		{
			if (_showBoneHierarchy)
			{
				_showBoneHierarchy = false;
			}
			else
			{
				_showBoneHierarchy = true;
			}
		}

		//End MainMenu
		ImGui::EndMenu();
	}

}

void GUIView::Render()
{
	//LoadedAsset
	{
		LoadedAsset();
	}

	//BoneHierarchy
	{
		BoneHierarchy();
	}
}
