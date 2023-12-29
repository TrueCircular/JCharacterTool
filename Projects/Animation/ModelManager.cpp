#include "pch.h"
#include "ModelManager.h"

ModelManager* ModelManager::_instance = nullptr;

ModelManager::ModelManager()
{
	_converter = make_shared<Converter>();
}

ModelManager::~ModelManager()
{
}

bool ModelManager::AddModelAsset(wstring key, shared_ptr<Model> model)
{
	if (GetModelByName(key) != nullptr)
		return false;

	_models.insert(make_pair(key, model));

	return true;
}

shared_ptr<Model> ModelManager::GetModelByName(wstring name)
{
	auto findIter = _models.find(name);

	if (findIter != _models.end())
		return findIter->second;

	return nullptr;
}

bool ModelManager::ReadAssetFile(AssetPathDesc& desc)
{
	//desc init
	_assetDesc = desc;

	//converter init
	_converter->Init();

	//read asset file
	if (_converter->ReadAssetFile(_assetDesc.ReadPath))
	{
		ExportMaterialData(_assetDesc.SaveMaterialPath);
		ExportModelData(_assetDesc.SaveMeshPath);

		return true;
	}

	return false;
}

bool ModelManager::ReadAnimationFile(AnimPathDesc& desc)
{
	//desc init
	_animDesc = desc;

	//converter init
	_converter->Init();

	//read animation file
	if (_converter->ReadAssetFile(_animDesc.ReadPath))
	{
		if (ExportAnimationData(_animDesc.SavePath))
		{
			return true;
		}
	}

	return false;
}

bool ModelManager::ExportMaterialData(wstring exportPath)
{
	_converter->ExportMaterialData(exportPath);

	return true;
}

bool ModelManager::ExportModelData(wstring exportPath)
{
	_converter->ExportModelData(exportPath);

	return true;
}

bool ModelManager::ExportAnimationData(wstring exportPath)
{
	_converter->ExportAnimationData(exportPath);

	return true;
}

bool ModelManager::CreateModel()
{
	return false;
}

bool ModelManager::CreateObject()
{
	return false;
}

void ModelManager::Init()
{
	_converter->Init();
	ZeroMemory(&_assetDesc, sizeof(_assetDesc));
	ZeroMemory(&_animDesc, sizeof(_animDesc));
}

void ModelManager::Update()
{
	for (auto& object : _objects)
	{
		object->Update();
	}
}

void ModelManager::Clear()
{
}


