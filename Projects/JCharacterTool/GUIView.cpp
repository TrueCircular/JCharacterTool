#include "pch.h"
#include "GUIView.h"
#include "AssetManager.h"
#include "engine/Utils.h"
#include <array>
#include <unordered_set>

static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
static ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns;

void GUIView::DisplayBoneHierarchyNode(const shared_ptr<ModelBone>& node, const vector<shared_ptr<ModelBone>>& nodes)
{
	ImGui::TableNextRow();
	ImGui::TableNextColumn();

	const bool is_Child = (node->children.size() > 0);

	if (is_Child)
	{
		string id = ::to_string(node->index);
		bool open = ImGui::TreeNodeEx(id.c_str(), tree_node_flags);
		ImGui::TableNextColumn();
		ImGui::Text(Utils::ToString(node->name).c_str());

		if (open)
		{

			for (auto& childNode : node->children)
			{
				DisplayBoneHierarchyNode(childNode, nodes);
			}

			ImGui::TreePop();
		}
	}
	else
	{
		string id = ::to_string(node->index);
		ImGui::TreeNodeEx(id.c_str(), tree_node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);
		ImGui::TableNextColumn();
		ImGui::Text(Utils::ToString(node->name).c_str());
	}
}

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
		_skeletalCheckList.resize(MAX_SKELETAL_ASSET_COUNT, 0);
		_staticCheckList.resize(MAX_STATIC_ASSET_COUNT, 0);
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

	{
		_transformPos = Vec3(0.f);
		_transformRot = Vec3(0.f);
		_transformScale = Vec3(1.f);
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
					const auto& assetList = MANAGER_ASSET()->GetLoadedSkeletalMeshDataList();

					if (ImGui::BeginListBox("##SkeletalTabBox", ImVec2(_loadedAssetSize.x - 15, _loadedAssetSize.y - 58.f)))
					{
						int index = 0;
						for (auto& asset : assetList)
						{
							string assetName;
							assetName = Utils::ToString(asset.second.Name).c_str();

							const bool is_selected = (_currentSkeletalItemIndex == index);
							if (ImGui::Selectable(assetName.c_str(), is_selected))
							{
								_currentSkeletalItemIndex = index;
							}

							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();

								_selectedModelAsset = asset.second.Model;
							}

							if (ImGui::BeginPopupContextItem())
							{
								if (ImGui::Checkbox("##check", (bool*)&_skeletalCheckList[_currentSkeletalItemIndex]))
								{
									bool isChecked = _skeletalCheckList[_currentSkeletalItemIndex];

									if (isChecked == true)
									{
										if (asset.second.Model->GetActive() == false)
										{
											asset.second.Model->SetActive(true);
										}
									}
									else
									{
										if (asset.second.Model->GetActive() == true)
										{
											asset.second.Model->SetActive(false);
										}
									}
								}

								ImGui::SameLine();
								ImGui::Text("Rendering ");

								if (ImGui::Button("Close"))
								{
									ImGui::CloseCurrentPopup();
								}
								ImGui::EndPopup();
							}
							index++;
						}
						ImGui::EndListBox();
					}
					ImGui::EndTabItem();
				}

				//Static Model Tab
				if (ImGui::BeginTabItem("Static"))
				{
					const auto& assetList = MANAGER_ASSET()->GetLoadedStaticMeshDataList();

					if (ImGui::BeginListBox("##StaticTabBox", ImVec2(_loadedAssetSize.x - 15, _loadedAssetSize.y - 58.f)))
					{
						int index = 0;
						for (auto& asset : assetList)
						{
							string assetName;
							assetName = Utils::ToString(asset.second.Name).c_str();

							const bool is_selected = (_currentStaticItemIndex == index);
							if (ImGui::Selectable(assetName.c_str(), is_selected))
							{
								_currentStaticItemIndex = index;
							}
							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();
								_selectedModelAsset = asset.second.Model;
							}

							if (ImGui::BeginPopupContextItem())
							{
								if (ImGui::Checkbox("##check2", (bool*)&_staticCheckList[_currentStaticItemIndex]))
								{
									bool isChecked = _staticCheckList[_currentStaticItemIndex];

									if (isChecked == true)
									{
										if (asset.second.Model->GetActive() == false)
										{
											asset.second.Model->SetActive(true);
										}
									}
									else
									{
										if (asset.second.Model->GetActive() == true)
										{
											asset.second.Model->SetActive(false);
										}
									}
								}
								ImGui::SameLine();
								ImGui::Text("Rendering");

								if (ImGui::Button("Close"))
								{
									ImGui::CloseCurrentPopup();
								}
								ImGui::EndPopup();
							}
							index++;
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
				ImGui::CheckboxFlags("SpanAllColumns", &tree_node_flags, ImGuiTreeNodeFlags_SpanAllColumns);
				ImGui::CheckboxFlags("SpanFullWidth", &tree_node_flags, ImGuiTreeNodeFlags_SpanFullWidth);

				if (ImGui::BeginTable("3ways", 2, flags))
				{
					ImGui::TableSetupColumn("Number", ImGuiTableColumnFlags_NoHide);
					ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableHeadersRow();

					if (_selectedModelAsset != nullptr)
					{
						const auto& modelBones = _selectedModelAsset->GetModelRenderer()->GetModel()->GetBones();
						DisplayBoneHierarchyNode(modelBones[0], modelBones);
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
		{
			_transformPos = _selectedModelAsset->GetTransform()->GetLocalPosition();
		}
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::SetColumnWidth(ImGui::GetColumnIndex(), 75.f);
		ImGui::Text("Position");
		ImGui::NextColumn();
		ImGui::DragFloat3("##Position", (float*)&_transformPos, 0.01f);
		ImGui::SameLine(0.f, 40.f);
		ImGui::PushID("##PosID");
		if (ImGui::ButtonEx("Reset"))
		{
			_transformPos = Vec3(0.f);
		}
		_selectedModelAsset->GetTransform()->SetLocalPosition(_transformPos);
		ImGui::PopID();
		ImGui::Columns();

		//Rotation
		{
			_transformRot = _selectedModelAsset->GetTransform()->GetLocalRotation();
		}
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::SetColumnWidth(ImGui::GetColumnIndex(), 75.f);
		ImGui::Text("Rotaiotn");
		ImGui::NextColumn();
		ImGui::DragFloat3("##Rotation", (float*)&_transformRot, 0.01f);
		ImGui::SameLine(0.f, 40.f);
		ImGui::PushID("##RotID");
		if (ImGui::ButtonEx("Reset"))
		{
			_transformRot = Vec3(0.f);
		}
		_selectedModelAsset->GetTransform()->SetLocalRotation(_transformRot);
		ImGui::PopID();
		ImGui::Columns();

		//Scale
		{
			_transformScale = _selectedModelAsset->GetTransform()->GetLocalScale();
		}
		ImGui::Columns(2);
		ImGui::Separator();
		ImGui::SetColumnWidth(ImGui::GetColumnIndex(), 75.f);
		ImGui::Text("Scale");
		ImGui::NextColumn();
		if (ImGui::DragFloat3("##Scale", (float*)&_transformScale, 0.01f))
		{
			if (_scaleCheck)
			{
				_transformScale.z = _transformScale.y = _transformScale.x;
			}
		}
		ImGui::SameLine(0.f, 10.f);
		ImGui::Checkbox("##scaleCheck", &_scaleCheck);
		ImGui::SetItemTooltip("If this CheckBox is checked, All elements of the Scale\nincrease and decrease equally depending on the value of X.");
		ImGui::SameLine(0.f, 11.f);
		ImGui::PushID("##ScaleID");
		if (ImGui::ButtonEx("Reset"))
		{
			_transformScale = Vec3(1.0f);
		}
		_selectedModelAsset->GetTransform()->SetLocalScale(_transformScale);
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

		const auto& assetList = MANAGER_ASSET()->GetLoadedSkeletalMeshDataList();
		const auto& animList = MANAGER_ASSET()->GetLoadedAnimDataList();

		vector<string> modelNames;
		if (assetList.size() > 0)
		{
			modelNames.reserve(assetList.size());
			{
				for (const auto& asset : assetList)
				{
					modelNames.push_back(Utils::ToString(asset.second.Name));
				}
			}
		}

		vector<string> animNames;
		if (animList.size() > 0)
		{
			animNames.reserve(animList.size());
			{
				for (const auto& anim : animList)
				{
					animNames.push_back(Utils::ToString(anim.second.Name));
				}
			}
		}
		const char* previewModelName = modelNames[_currentModelComboIndex].c_str();
		const char* previewAnimName = animNames[_currentAnimComboIndex].c_str();

		//Begin Animation Window
		if (ImGui::Begin("Animation", &_showAnimation, aniFlags))
		{
			if (ImGui::BeginCombo("Select Model", previewModelName))
			{
				for (int n = 0; n < modelNames.size(); n++)
				{
					const bool is_selected = (_currentModelComboIndex == n);
					if (ImGui::Selectable(modelNames[n].c_str(), is_selected))
						_currentModelComboIndex = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();

				}
				ImGui::EndCombo();
			}

			if (ImGui::BeginCombo("Current Animation List In Model", previewModelName))
			{
				for (int n = 0; n < modelNames.size(); n++)
				{
					const bool is_selected = (_currentModelComboIndex == n);
					if (ImGui::Selectable(modelNames[n].c_str(), is_selected))
						_currentModelComboIndex = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();

				}
				ImGui::EndCombo();
			}

			//if (ImGui::BeginCombo("Select Animation", previewAnimName))
			//{
			//	for (int n = 0; n < animNames.size(); n++)
			//	{
			//		const bool is_selected = (_currentAnimComboIndex == n);
			//		if (ImGui::Selectable(animNames[n].c_str(), is_selected))
			//			_currentAnimComboIndex = n;

			//		if (is_selected)
			//			ImGui::SetItemDefaultFocus();

			//	}
			//	ImGui::EndCombo();
			//}

		}
		//End Animation Window
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
