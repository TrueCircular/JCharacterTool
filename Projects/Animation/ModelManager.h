#pragma once
#include "Converter.h"

struct AssetPathDesc
{
	wstring assetName;
	wstring assetReadPath;
	wstring assetSavepath;
};

struct AnimPathDesc
{
	wstring animName;
	wstring animReadPath;
	wstring animSavePath;
};

class ModelManager
{
private:
	static ModelManager* _instance;
public:
	static ModelManager* GetInstance()
	{
		if (_instance == nullptr)
			_instance = new ModelManager();

		return _instance;
	}
private:
	ModelManager();
	~ModelManager();
private:
	shared_ptr<Converter> _converter;
	AssetPathDesc _assetDesc;
	AnimPathDesc _animDesc;
public:
	vector<shared_ptr<asBone>> GetBones() { return _converter->_bones; }
	vector<shared_ptr<asMesh>> GetMeshes() { return _converter->_meshes; }
	vector<shared_ptr<asMaterial>> GetMaterials() { return _converter->_materials; }
public:
	void Init();
	bool ReadAssetFile(wstring filePath);
	bool ExportMaterialData(wstring savePath);
};

#define MANAGER_MODEL() ModelManager::GetInstance()
