#include "pch.h"
#include "AssetManager.h"

AssetManager* AssetManager::_instance = nullptr;

AssetManager::AssetManager()
{
	_converter = make_shared<Converter>();
}

AssetManager::~AssetManager()
{
}

bool AssetManager::AddModelAsset(wstring key, shared_ptr<Model> model)
{
	if (GetModelByName(key) != nullptr)
		return false;

	_models.insert(make_pair(key, model));

	return true;
}

shared_ptr<Model> AssetManager::GetModelByName(wstring name)
{
	auto findIter = _models.find(name);

	if (findIter != _models.end())
		return findIter->second;

	return nullptr;
}

bool AssetManager::ReadAssetFile(AssetPathDesc& desc)
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

bool AssetManager::ReadAnimationFile(AnimPathDesc& desc)
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

bool AssetManager::ExportMaterialData(wstring exportPath)
{
	_converter->ExportMaterialData(exportPath);

	return true;
}

bool AssetManager::ExportModelData(wstring exportPath)
{
	_converter->ExportModelData(exportPath);

	return true;
}

bool AssetManager::ExportAnimationData(wstring exportPath)
{
	_converter->ExportAnimationData(exportPath);

	return true;
}

bool AssetManager::CreateModel()
{
	return false;
}

bool AssetManager::CreateObject()
{
	return false;
}

void AssetManager::Init()
{
	_converter->Init();
	ZeroMemory(&_assetDesc, sizeof(_assetDesc));
	ZeroMemory(&_animDesc, sizeof(_animDesc));
}

void AssetManager::Update()
{
	for (auto& object : _currentViewObjects)
	{
		object->Update();
	}
}

void AssetManager::Clear()
{
}


