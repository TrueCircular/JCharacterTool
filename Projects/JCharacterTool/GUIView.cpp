#include "pch.h"
#include "GUIView.h"
#include "AssetManager.h"
#include "engine/Utils.h"

static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

void GUIView::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void GUIView::ButtonManage()
{
	//Exception
	{
		if (!_showLoadedAsset ||
			!_showScene)
		{
			_showAssetSection = false;
		}
		else
		{
			_showAssetSection = true;
		}


		if (!_showInspector ||
			!_showBoneHierarchy)
		{
			_showModelSection = false;
		}
		else
		{
			_showModelSection = true;
		}


		if ((!_showCameraWindow && _showAll) ||
			(!_showLoadedAsset && _showAll) ||
			(!_showScene && _showAll) ||
			(!_showInspector && _showAll) ||
			(!_showBoneHierarchy && _showAll) ||
			(!_showAnimation && _showAll))
		{
			_showAll = false;
		}
		else if ((!_showCameraWindow && !_showAll) ||
			(!_showLoadedAsset && !_showAll) ||
			(!_showScene && !_showAll) ||
			(!_showInspector && !_showAll) ||
			(!_showBoneHierarchy && !_showAll) ||
			(!_showAnimation && !_showAll))
		{
			_showAll = false;
		}
		else if ((_showCameraWindow && _showAll) ||
			(_showLoadedAsset && _showAll) ||
			(_showScene && _showAll) ||
			(_showInspector && _showAll) ||
			(_showBoneHierarchy && _showAll) ||
			(_showAnimation && _showAll))
		{
			_showAll = true;
		}
		else if ((_showCameraWindow && !_showAll) ||
			(_showLoadedAsset && !_showAll) ||
			(_showScene && !_showAll) ||
			(_showInspector && !_showAll) ||
			(_showBoneHierarchy && !_showAll) ||
			(_showAnimation && !_showAll))
		{
			_showAll = true;
		}

	}
}

GUIView::GUIView() : Super(GUIType::View)
{
	{
		_cameraCom = make_shared<Camera>();
		_cameraMove = make_shared<CameraMove>();
		_camera = make_shared<GameObject>();
		_camera->Awake();
		_camera->AddComponent(_cameraCom);
		_camera->AddComponent(_cameraMove);
		_camTransfrom = _camera->GetTransform();
		_cameraCom->SetWidth(1243.f);
		_cameraCom->SetHeight(642.f);
	}

	{
		_loadedAssetPos.x = 0;
		_loadedAssetPos.y = 18.f;
		_loadedAssetSize.x = 350.f;
		_loadedAssetSize.y = 250.f;
	}

	{
		_cameraWindowPos.x = g_gameDesc.width - 350.f;;
		_cameraWindowPos.y = 18.f;
		_cameraWindowSize.x = 350.f;
		_cameraWindowSize.y = 250.f;
	}

	{
		_scenePos.x = 350.f;
		_scenePos.y = 18.f;
		_sceneSize.x = g_gameDesc.width - 700.f;
		_sceneSize.y = 632.f;
	}

	{
		_boneHierarchyPos.x = 0.f;
		_boneHierarchyPos.y = 268.f;
		_boneHierarchySize.x = 350.f;
		_boneHierarchySize.y = 632.f;
	}

	{
		_inspectorPos.x = g_gameDesc.width - 350.f;
		_inspectorPos.y = 268.f;
		_inspectorSize.x = 350.f;
		_inspectorSize.y = 632.f;
	}

	{
		_animationPos.x = 350.f;
		_animationPos.y = 650.f;
		_animationSize.x = 900.f;
		_animationSize.y = 250.f;
	}
}

GUIView::~GUIView()
{
}

void GUIView::LoadedAsset()
{
	if (_showLoadedAsset)
	{
		ImGui::SetNextWindowPos(_loadedAssetPos);
		ImGui::SetNextWindowSize(_loadedAssetSize);
		ImGuiWindowFlags assetFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;


		ImGuiTabBarFlags tabBarFlag = ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_FittingPolicyResizeDown;

		if (ImGui::Begin("AssetList", &_showLoadedAsset, assetFlags))
		{
			if (ImGui::BeginTabBar("mtbar", tabBarFlag))
			{
				//Skeletal Moddel Tab
				if (ImGui::BeginTabItem("Skeletal"))
				{
					const auto& assetList = MANAGER_ASSET()->GetLoadedMeshDataList();

					if (ImGui::BeginListBox("##SkeletalTabBox", ImVec2(_loadedAssetSize.x - 15, _loadedAssetSize.y - 58.f)))
					{
						int index = 0;
						for (auto& asset : assetList)
						{
							if (asset.second.Type == ModelType::Skeletal)
							{

								const bool is_selected = (_currentSkeletalItemIndex == index);
								if (ImGui::Selectable(Utils::ToString(asset.second.Name).c_str(), is_selected))
								{
									_currentSkeletalItemIndex = index;
								}
								if (is_selected)
									ImGui::SetItemDefaultFocus();

								index++;
							}
						}
						ImGui::EndListBox();
					}
					ImGui::EndTabItem();
				}

				//Static Model Tab
				if (ImGui::BeginTabItem("Static"))
				{
					const auto& assetList = MANAGER_ASSET()->GetLoadedMeshDataList();

					if (ImGui::BeginListBox("##StaticTabBox", ImVec2(_loadedAssetSize.x - 15, _loadedAssetSize.y - 58.f)))
					{
						int index = 0;
						for (auto& asset : assetList)
						{
							if (asset.second.Type == ModelType::Static)
							{
								const bool is_selected = (_currentSkeletalItemIndex == index);
								if (ImGui::Selectable(Utils::ToString(asset.second.Name).c_str(), is_selected))
								{
									_currentSkeletalItemIndex = index;
								}
								if (is_selected)
									ImGui::SetItemDefaultFocus();

								index++;
							}
						}
						ImGui::EndListBox();
					}
					ImGui::EndTabItem();
				}

				//Animation Tab
				if (ImGui::BeginTabItem("Animation"))
				{
					const auto& assetList = MANAGER_ASSET()->GetLoadedAnimDataList();

					if (ImGui::BeginListBox("##AnimationTabBox", ImVec2(_loadedAssetSize.x - 15, _loadedAssetSize.y - 58.f)))
					{
						int index = 0;
						for (auto& asset : assetList)
						{
							const bool is_selected = (_currentAnimationItemIndex == index);
							if (ImGui::Selectable(Utils::ToString(asset.second.Name).c_str(), is_selected))
							{
								_currentAnimationItemIndex = index;
							}
							if (is_selected)
								ImGui::SetItemDefaultFocus();

							index++;
						}

						ImGui::EndListBox();
					}
					ImGui::EndTabItem();
				}

				//Effect Tab
				if (ImGui::BeginTabItem("Effect"))
				{
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();
	}
}

void GUIView::Scene()
{
	if (_showScene)
	{
		ImGui::SetNextWindowPos(_scenePos);
		ImGui::SetNextWindowSize(_sceneSize);
		ImGuiWindowFlags scFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;

		MANAGER_ASSET()->Update();
		_camera->Update();

		ImGui::Begin("Scene", &_showScene, scFlags);
		{

			{
				pTex = GRAPHICS()->GetRenderTexture(1).Get();
				D3D11_TEXTURE2D_DESC desc;
				pTex->GetDesc(&desc);

				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
				srvDesc.Format = desc.Format;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = 1;
				DEVICE()->CreateShaderResourceView(pTex.Get(), &srvDesc, pSRV.GetAddressOf());
			}
			_sceneDrawSize = ImVec2(ImGui::GetCursorScreenPos().x + _sceneSize.x - 15.f, ImGui::GetCursorScreenPos().y + _sceneSize.y - 35.f);

			ImGui::GetWindowDrawList()->AddImage(
				(void*)pSRV.Get(),
				ImGui::GetCursorScreenPos(),
				_sceneDrawSize
			);
		}
		ImGui::End();
	}
}

void GUIView::BoneHierarchy()
{
	if (_showBoneHierarchy)
	{
		ImGui::SetNextWindowPos(_boneHierarchyPos);
		ImGui::SetNextWindowSize(_boneHierarchySize);

		ImGuiWindowFlags bhFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;

		if (ImGui::Begin("Bone Hierarchy", &_showBoneHierarchy, bhFlags))
		{
			const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;

			if (ImGui::TreeNodeEx("Bone List", ImGuiTreeNodeFlags_DefaultOpen))
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
			if (ImGui::CollapsingHeader("Bone Info", ImGuiTreeNodeFlags_DefaultOpen))
			{

			}

		}
		//Bone Hierarchy End
		ImGui::End();
	}
}

void GUIView::Inspector()
{
	if (_showInspector)
	{
		ImGui::SetNextWindowPos(_inspectorPos);
		ImGui::SetNextWindowSize(_inspectorSize);

		ImGuiWindowFlags insFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;
		//Begin Inspector
		if (ImGui::Begin("Inspector", &_showInspector, insFlags))
		{
			//Transform
			{
				this->Transform();
			}
			//Mesh
			{
				this->Mesh();
			}
			//Material
			{
				this->Material();
			}
		}
		//End Inspector
		ImGui::End();
	}
}

void GUIView::Transform()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		HelpMarker("If you want, you can adjust Transform by default through mouse drag. Alternatively, you can enter the value directly by double-clicking.");

		//Position
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::SetColumnWidth(ImGui::GetColumnIndex(), 75.f);
		ImGui::Text("Position");
		ImGui::NextColumn();
		ImGui::DragFloat3("##Position", _transformPos, 0.01f);
		ImGui::SameLine(0.f, 40.f);
		ImGui::PushID("##PosID");
		if (ImGui::ButtonEx("Reset"))
		{
			_transformPos[0] = 0.f;
			_transformPos[1] = 0.f;
			_transformPos[2] = 0.f;
		}
		ImGui::PopID();
		ImGui::Columns();

		//Rotation
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::SetColumnWidth(ImGui::GetColumnIndex(), 75.f);
		ImGui::Text("Rotaiotn");
		ImGui::NextColumn();
		ImGui::DragFloat3("##Rotation", _transformRot, 0.01f);
		ImGui::SameLine(0.f, 40.f);
		ImGui::PushID("##RotID");
		if (ImGui::ButtonEx("Reset"))
		{
			_transformRot[0] = 0.f;
			_transformRot[1] = 0.f;
			_transformRot[2] = 0.f;
		}
		ImGui::PopID();
		ImGui::Columns();

		//Scale
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::SetColumnWidth(ImGui::GetColumnIndex(), 75.f);
		ImGui::Text("Scale");
		ImGui::NextColumn();
		if (ImGui::DragFloat3("##Scale", _transformScale, 0.01f))
		{
			if (_scaleCheck)
			{
				_transformScale[2] = _transformScale[1] = _transformScale[0];
			}
		}
		ImGui::SameLine(0.f, 10.f);
		ImGui::Checkbox("##scaleCheck", &_scaleCheck);
		ImGui::SetItemTooltip("If this CheckBox is checked, All elements of the Scale\nincrease and decrease equally depending on the value of X.");
		ImGui::SameLine(0.f, 11.f);
		ImGui::PushID("##ScaleID");
		if (ImGui::ButtonEx("Reset"))
		{
			_transformScale[0] = 1.f;
			_transformScale[1] = 1.f;
			_transformScale[2] = 1.f;
		}
		ImGui::PopID();
		ImGui::Columns();
	}
}

void GUIView::Mesh()
{
	if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
}

void GUIView::Material()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{

	}
}

void GUIView::Animation()
{
	if (_showAnimation)
	{
		ImGui::SetNextWindowPos(_animationPos);
		ImGui::SetNextWindowSize(_animationSize);

		ImGuiWindowFlags aniFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;
		//Begin Inspector
		if (ImGui::Begin("Animation", &_showAnimation, aniFlags))
		{
		}
		//End Inspector
		ImGui::End();
	}
}

void GUIView::Lighting()
{
}

void GUIView::CameraWindow()
{
	if (_showCameraWindow)
	{
		ImGui::SetNextWindowPos(_cameraWindowPos);
		ImGui::SetNextWindowSize(_cameraWindowSize);
		ImGuiWindowFlags camFlags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize;

		if (ImGui::Begin("Camera", &_showCameraWindow, camFlags))
		{
			//Position
			ImGui::Columns(2);
			ImGui::Separator();
			ImGui::SetColumnWidth(ImGui::GetColumnIndex(), 75.f);
			ImGui::Text("Position");
			ImGui::NextColumn();
			Vec3 pos = _camTransfrom->GetLocalPosition();
			ImGui::DragFloat3("##CamPosition", (float*)&pos, 0.01f);
			_camTransfrom->SetLocalPosition(pos);
			ImGui::SameLine(0.f, 40.f);
			ImGui::PushID("##CamPosID");
			if (ImGui::ButtonEx("Reset"))
			{
				_camTransfrom->SetLocalPosition(Vec3(0.f));
			}
			ImGui::PopID();
			ImGui::Columns();

			//Rotation
			ImGui::Columns(2);
			ImGui::Separator();
			ImGui::SetColumnWidth(ImGui::GetColumnIndex(), 75.f);
			ImGui::Text("Rotaiotn");
			ImGui::NextColumn();
			Vec3 rot = _camTransfrom->GetLocalRotation();
			ImGui::DragFloat3("##CamRotation", (float*)&rot, 0.01f);
			_camTransfrom->SetLocalRotation(rot);
			ImGui::SameLine(0.f, 40.f);
			ImGui::PushID("##CamRotID");
			if (ImGui::ButtonEx("Reset"))
			{
				_camTransfrom->SetLocalRotation(Vec3(0.f));
			}
			ImGui::PopID();
			ImGui::Columns();
			ImGui::Separator();
		}
		ImGui::End();

	}
}

float* GUIView::ConvertMatrixToFloat(Matrix& mat)
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
		if (ImGui::MenuItem("Show All", NULL, _showAll))
		{
			if (_showAll)
			{
				_showAll = false;

				_showCameraWindow = false;
				_showLoadedAsset = false;
				_showScene = false;
				_showInspector = false;
				_showBoneHierarchy = false;
				_showAnimation = false;
			}
			else
			{
				_showAll = true;

				_showCameraWindow = true;
				_showLoadedAsset = true;
				_showScene = true;
				_showInspector = true;
				_showBoneHierarchy = true;
				_showAnimation = true;
			}
		}
		//----------------------
		ImGui::Separator();
		//----------------------

		ImGui::MenuItem("(Asset)", NULL, false, false);

		//----------------------
		ImGui::Separator();
		//----------------------

		if (ImGui::MenuItem("Section...", NULL, _showAssetSection))
		{
			if (_showAssetSection)
			{
				_showAssetSection = false;
				_showLoadedAsset = false;
				_showScene = false;
				_showCameraWindow = false;
			}
			else
			{
				_showAssetSection = true;
				_showLoadedAsset = true;
				_showScene = true;
				_showCameraWindow = true;
			}
		}

		if (ImGui::MenuItem("Loaded AssetList", NULL, _showLoadedAsset))
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

		if (ImGui::MenuItem("Scene", NULL, _showScene))
		{
			if (_showScene)
			{
				_showScene = false;
			}
			else
			{
				_showScene = true;
			}
		}

		if (ImGui::MenuItem("Camera", NULL, _showCameraWindow))
		{
			if (_showCameraWindow)
			{
				_showCameraWindow = false;
			}
			else
			{
				_showCameraWindow = true;
			}
		}

		//----------------------
		ImGui::Separator();
		//----------------------

		ImGui::MenuItem("(Model & Effect)", NULL, false, false);

		//----------------------
		ImGui::Separator();
		//----------------------

		if (ImGui::MenuItem("Section...", NULL, _showModelSection))
		{
			if (_showModelSection)
			{
				_showModelSection = false;
				_showInspector = false;
				_showBoneHierarchy = false;
			}
			else
			{
				_showModelSection = true;
				_showInspector = true;
				_showBoneHierarchy = true;
			}
		}

		if (ImGui::MenuItem("Inspector", NULL, _showInspector))
		{
			if (_showInspector)
			{
				_showInspector = false;
			}
			else
			{
				_showInspector = true;
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

		//----------------------
		ImGui::Separator();
		//----------------------

		ImGui::MenuItem("(Animation)", NULL, false, false);

		//----------------------
		ImGui::Separator();
		//----------------------

		if (ImGui::MenuItem("Animation", NULL, _showAnimation))
		{
			if (_showAnimation)
			{
				_showAnimation = false;
			}
			else
			{
				_showAnimation = true;
			}
		}

		//End MainMenu
		ImGui::EndMenu();
	}

	ButtonManage();
}

void GUIView::Render()
{

	//LoadedAsset
	{
		LoadedAsset();
	}

	//Camera
	{
		CameraWindow();
	}

	//Scene
	{
		Scene();
	}

	//Inspector
	{
		Inspector();
	}

	//BoneHierarchy
	{
		BoneHierarchy();
	}

	//Animation
	{
		Animation();
	}


}
