#include "pch.h"
#include "Model.h"
#include "Utils.h"
#include "FileUtils.h"
#include <filesystem>

Model::Model() : Super(ResourceType::Model)
{
}

Model::~Model()
{
}

void Model::BindCacheInfo()
{
	// Mesh에 Material 캐싱
	for (const auto& mesh : _meshes)
	{
		// 이미 찾았으면 스킵
		if (mesh->material != nullptr)
			continue;

		mesh->material = GetMaterialByName(mesh->materialName);
	}

	// Mesh에 Bone 캐싱
	for (const auto& mesh : _meshes)
	{
		// 이미 찾았으면 스킵
		if (mesh->bone != nullptr)
			continue;

		mesh->bone = GetBoneByIndex(mesh->boneIndex);
	}

	// Bone 계층 정보 채우기
	if (_root == nullptr && _bones.size() > 0)
	{
		_root = _bones[0];


		for (int i = 1; i < _bones.size(); i++)
		{
			if (_bones[i]->parentIndex == -1)
			{
				_bones[0]->children.push_back(_bones[i]);
				_bones[i]->parent = _bones[0];
			}
			else if (_bones[i]->parentIndex >= 0)
			{
				auto parent = GetBoneByIndex(_bones[i]->parentIndex);
				parent->children.push_back(_bones[i]);
				_bones[i]->parent = parent;
			}
		}
	}
}

shared_ptr<Material> Model::GetMaterialByName(const wstring& name)
{
	for (auto& material : _materials)
	{
		if (material->GetName() == name)
			return material;
	}

	return nullptr;
}

shared_ptr<ModelMesh> Model::GetMeshByName(const wstring& name)
{
	for (auto& mesh : _meshes)
	{
		if (mesh->name == name)
			return mesh;
	}

	return nullptr;
}

shared_ptr<ModelBone> Model::GetBoneByName(const wstring& name)
{
	for (auto& bone : _bones)
	{
		if (bone->name == name)
			return bone;
	}

	return nullptr;
}

shared_ptr<ModelAnimation> Model::GetAnimationByName(wstring name)
{
	for (auto& anim : _animations)
	{
		if (anim->name == name)
			return anim;
	}

	return nullptr;
}

void Model::SetModelType(ModelType type)
{
	_modelType = type;
	_modelData.type = static_cast<uint16>(_modelType);
}

void Model::ReadMaterial(wstring fileName)
{
	wstring fullPath = fileName;
	_modelData.materialPath = fullPath;

	auto parentPath = filesystem::path(fullPath).parent_path();

	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	tinyxml2::XMLError error = document->LoadFile(Utils::ToString(fullPath).c_str());
	assert(error == tinyxml2::XML_SUCCESS);

	tinyxml2::XMLElement* root = document->FirstChildElement();
	tinyxml2::XMLElement* materialNode = root->FirstChildElement();

	while (materialNode)
	{
		shared_ptr<Material> material = make_shared<Material>();

		tinyxml2::XMLElement* node = nullptr;

		node = materialNode->FirstChildElement();
		material->SetName(Utils::ToWString(node->GetText()));

		// Diffuse Texture
		node = node->NextSiblingElement();
		if (node->GetText())
		{
			wstring textureStr = Utils::ToWString(node->GetText());
			if (textureStr.length() > 0)
			{
				auto texture = MANAGER_RESOURCES()->GetOrAddTexture(textureStr, (parentPath / textureStr).wstring());
				material->SetDiffuseMap(texture);
				material->GetDiffuseMap()->SetName(textureStr);
			}
		}

		// Specular Texture
		node = node->NextSiblingElement();
		if (node->GetText())
		{
			wstring texture = Utils::ToWString(node->GetText());
			if (texture.length() > 0)
			{
				wstring textureStr = Utils::ToWString(node->GetText());
				if (textureStr.length() > 0)
				{
					auto texture = MANAGER_RESOURCES()->GetOrAddTexture(textureStr, (parentPath / textureStr).wstring());
					material->SetSpecularMap(texture);
					material->GetSpecularMap()->SetName(textureStr);
				}
			}
		}

		// Normal Texture
		node = node->NextSiblingElement();
		if (node->GetText())
		{
			wstring textureStr = Utils::ToWString(node->GetText());
			if (textureStr.length() > 0)
			{
				auto texture = MANAGER_RESOURCES()->GetOrAddTexture(textureStr, (parentPath / textureStr).wstring());
				material->SetNormalMap(texture);
				material->GetNormalMap()->SetName(textureStr);
			}
		}

		// Ambient
		{
			node = node->NextSiblingElement();

			Color color;
			if (node->FloatAttribute("R") == 0)
			{
				color.x = 1.f;
			}
			else
			{
				color.x = node->FloatAttribute("R");
			}
			if (node->FloatAttribute("G") == 0)
			{
				color.y = 1.f;
			}
			else
			{
				color.y = node->FloatAttribute("G");
			}
			if (color.z = node->FloatAttribute("B") == 0)
			{
				color.z = 1.f;
			}
			else
			{
				color.z = node->FloatAttribute("B");
			}
			color.w = node->FloatAttribute("A");
			material->GetMaterialDesc().ambient = color;
		}

		// Diffuse
		{
			node = node->NextSiblingElement();

			Color color;
			color.x = node->FloatAttribute("R");
			color.y = node->FloatAttribute("G");
			color.z = node->FloatAttribute("B");
			color.w = node->FloatAttribute("A");
			material->GetMaterialDesc().diffuse = color;
		}

		// Specular
		{
			node = node->NextSiblingElement();

			Color color;
			color.x = node->FloatAttribute("R");
			color.y = node->FloatAttribute("G");
			color.z = node->FloatAttribute("B");
			color.w = node->FloatAttribute("A");
			material->GetMaterialDesc().specular = color;
		}

		// Emissive
		{
			node = node->NextSiblingElement();

			Color color;
			color.x = node->FloatAttribute("R");
			color.y = node->FloatAttribute("G");
			color.z = node->FloatAttribute("B");
			color.w = node->FloatAttribute("A");
			material->GetMaterialDesc().emissive = color;
		}

		_materials.push_back(material);

		// Next Material
		materialNode = materialNode->NextSiblingElement();
	}

	BindCacheInfo();
}

void Model::ReadModel(wstring fileName)
{
	wstring fullPath = fileName;
	_modelData.modelPath = fullPath;

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(fullPath, FileMode::Read);

	// Bones
	{
		const uint32 count = file->Read<uint32>();

		for (uint32 i = 0; i < count; i++)
		{
			shared_ptr<ModelBone> bone = make_shared<ModelBone>();
			bone->index = file->Read<int32>();
			bone->name = Utils::ToWString(file->Read<string>());
			bone->parentIndex = file->Read<int32>();
			bone->transform = file->Read<Matrix>();

			_bones.push_back(bone);
		}
	}

	// Mesh
	{
		const uint32 count = file->Read<uint32>();

		for (uint32 i = 0; i < count; i++)
		{
			shared_ptr<ModelMesh> mesh = make_shared<ModelMesh>();

			mesh->name = Utils::ToWString(file->Read<string>());
			mesh->boneIndex = file->Read<int32>();

			// Material
			mesh->materialName = Utils::ToWString(file->Read<string>());
			

			//VertexData
			{
				const uint32 count = file->Read<uint32>();
				vector<ModelVertexType> vertices;
				vertices.resize(count);

				void* data = vertices.data();
				file->Read(&data, sizeof(ModelVertexType) * count);
				mesh->geometry->AddVertices(vertices);
			}

			//IndexData
			{
				const uint32 count = file->Read<uint32>();

				vector<uint32> indices;
				indices.resize(count);

				void* data = indices.data();
				file->Read(&data, sizeof(uint32) * count);
				mesh->geometry->AddIndices(indices);
			}

			mesh->CreateBuffers();

			_meshes.push_back(mesh);
		}
	}

	BindCacheInfo();
}

void Model::ReadAnimation(wstring filename)
{
	wstring fullPath = filename;
	_modelData.animationPath = fullPath;

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(fullPath, FileMode::Read);

	shared_ptr<ModelAnimation> animation = make_shared<ModelAnimation>();
	animation->name = Utils::ToWString(file->Read<string>());
	animation->duration = file->Read<float>();
	animation->frameRate = file->Read<float>();
	animation->frameCount = file->Read<uint32>();

	uint32 keyframesCount = file->Read<uint32>();
	for (uint32 i = 0; i < keyframesCount; i++)
	{
		shared_ptr<ModelKeyframe> keyframe = make_shared<ModelKeyframe>();
		keyframe->boneName = Utils::ToWString(file->Read<string>());

		uint32 size = file->Read<uint32>();

		if (size > 0)
		{
			keyframe->transforms.resize(size);

			void* ptr = &keyframe->transforms[0];
			file->Read(&ptr, sizeof(ModelKeyframeData) * size);
		}

		animation->keyframes[keyframe->boneName] = keyframe;
	}

	_animations.push_back(animation);
}

void Model::SaveModel(wstring filePath, wstring fileName)
{
	//Material Data
	{	
		filesystem::path path;
		wstring mtrPath;
		if (_modelType == ModelType::Skeletal)
		{
			mtrPath = RESOURCES_ADDR_TEXTURE_SKELETAL;
			mtrPath += fileName;
			mtrPath += L"/";
			path = filesystem::path(mtrPath);
			filesystem::create_directory(path.parent_path());
		}
		else if (_modelType == ModelType::Static)
		{
			mtrPath = RESOURCES_ADDR_TEXTURE_STATIC;
			mtrPath += fileName;
			mtrPath += L"/";
			path = filesystem::path(mtrPath);
			filesystem::create_directory(path.parent_path());
		}
		//doc 생성
		shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();
		//버전 정의
		tinyxml2::XMLDeclaration* decl = document->NewDeclaration();
		document->LinkEndChild(decl);
		//머티리얼 목록
		tinyxml2::XMLElement* root = document->NewElement("Materials");
		document->LinkEndChild(root);

		HRESULT hr;
		for (auto& mat : _materials)
		{
			//머티리얼 당
			tinyxml2::XMLElement* node = document->NewElement("Material");
			root->LinkEndChild(node);
			tinyxml2::XMLElement* element = nullptr;

			//Name
			element = document->NewElement("Name");
			element->SetText(Utils::ToString(mat->GetName()).c_str());
			node->LinkEndChild(element);

			// Save Texture File
			//Diffuse
			if (mat->GetDiffuseMap() != nullptr)
			{	
				wstring fName = mat->GetDiffuseMap()->GetName();
				wstring dName = mtrPath;
				dName += fName;

				element = document->NewElement("DiffuseFile");
				element->SetText(Utils::ToString(fName).c_str());
				node->LinkEndChild(element);

				hr = DirectX::SaveToDDSFile(*mat->GetDiffuseMap()->GetInfo().GetImages(), DirectX::DDS_FLAGS_NONE, dName.c_str());
				CHECK(hr);
			}
			else
			{
				element = document->NewElement("DiffuseFile");
				element->SetText("");
				node->LinkEndChild(element);
			}
			//Specular
			if (mat->GetSpecularMap() != nullptr)
			{
				wstring fName = mat->GetSpecularMap()->GetName();
				wstring sName = mtrPath;
				sName += fName;

				element = document->NewElement("SpecularFile");
				element->SetText(Utils::ToString(fName).c_str());
				node->LinkEndChild(element);

				hr = DirectX::SaveToDDSFile(*mat->GetSpecularMap()->GetInfo().GetImages(), DirectX::DDS_FLAGS_NONE, sName.c_str());
				CHECK(hr);
			}
			else
			{
				element = document->NewElement("SpecularFile");
				element->SetText("");
				node->LinkEndChild(element);
			}
			//Normal
			if (mat->GetNormalMap() != nullptr)
			{
				wstring fName = mat->GetNormalMap()->GetName();
				wstring nName = mtrPath;
				nName += fName;

				element = document->NewElement("NormalFile");
				element->SetText(Utils::ToString(fName).c_str());
				node->LinkEndChild(element);

				hr = DirectX::SaveToDDSFile(*mat->GetNormalMap()->GetInfo().GetImages(), DirectX::DDS_FLAGS_NONE, nName.c_str());
				CHECK(hr);
			}
			else 
			{
				element = document->NewElement("NormalFile");
				element->SetText("");
				node->LinkEndChild(element);
			}

			//Ambient Influence
			element = document->NewElement("Ambient");
			element->SetAttribute("R", mat->GetMaterialDesc().ambient.x);
			element->SetAttribute("G", mat->GetMaterialDesc().ambient.y);
			element->SetAttribute("B", mat->GetMaterialDesc().ambient.z);
			element->SetAttribute("A", mat->GetMaterialDesc().ambient.w);
			node->LinkEndChild(element);
			//Diffuse Influence
			element = document->NewElement("Diffuse");
			element->SetAttribute("R", mat->GetMaterialDesc().diffuse.x);
			element->SetAttribute("G", mat->GetMaterialDesc().diffuse.y);
			element->SetAttribute("B", mat->GetMaterialDesc().diffuse.z);
			element->SetAttribute("A", mat->GetMaterialDesc().diffuse.w);
			node->LinkEndChild(element);
			//Specular Influence
			element = document->NewElement("Specular");
			element->SetAttribute("R", mat->GetMaterialDesc().specular.x);
			element->SetAttribute("G", mat->GetMaterialDesc().specular.y);
			element->SetAttribute("B", mat->GetMaterialDesc().specular.z);
			element->SetAttribute("A", mat->GetMaterialDesc().specular.w);
			node->LinkEndChild(element);
			//Emissive Influence
			element = document->NewElement("Emissive");
			element->SetAttribute("R", mat->GetMaterialDesc().emissive.x);
			element->SetAttribute("G", mat->GetMaterialDesc().emissive.y);
			element->SetAttribute("B", mat->GetMaterialDesc().emissive.z);
			element->SetAttribute("A", mat->GetMaterialDesc().emissive.w);
			node->LinkEndChild(element);
		}
		wstring docPath = mtrPath;
		docPath += fileName;
		docPath += L".xml";
		document->SaveFile(Utils::ToString(docPath).c_str());
	}

	//Save Model 디렉토리 생성
	wstring SaveModelPath = filePath + fileName;
	SaveModelPath += L".mesh";

	auto path = filesystem::path(SaveModelPath);

	filesystem::create_directory(path.parent_path());
	//파일을 쓰기모드로 생성
	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(SaveModelPath, FileMode::Write);

	//Bone Data
	file->Write<uint32>(_bones.size());
	for (auto& bone : _bones)
	{
		file->Write<int32>(bone->index);
		file->Write<string>(Utils::ToString(bone->name));
		file->Write<int32>(bone->parentIndex);
		file->Write<Matrix>(bone->transform);
	}

	//Mesh Data
	file->Write<uint32>(_meshes.size());
	for (auto& meshData : _meshes)
	{
		file->Write<string>(Utils::ToString(meshData->name));
		file->Write<int32>(meshData->boneIndex);
		file->Write<string>(Utils::ToString(meshData->materialName));
		//Vertex
		file->Write<uint32>(meshData->geometry->GetVertices().size());
		auto vertices = meshData->geometry->GetVertices();
		file->Write(&vertices[0], (uint32)(sizeof(ModelVertexType) * meshData->geometry->GetVertices().size()));
		//Index
		uint32 indexCount = meshData->geometry->GetIndices().size();
		auto indices = meshData->geometry->GetIndices();
		file->Write<uint32>(indexCount);
		file->Write(&indices[0], sizeof(uint32) * indexCount);
	}
}