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

void ModelManager::Init()
{
	_converter->Init();
	ZeroMemory(&_assetDesc, sizeof(_assetDesc));
	ZeroMemory(&_animDesc, sizeof(_animDesc));
}

bool ModelManager::ReadAssetFile(wstring filePath)
{
	if (_converter->ReadAssetFile(filePath))
	{
		return true;
	}

	return false;
}
