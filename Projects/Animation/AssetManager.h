#pragma once
#include "Converter.h"

struct AssetPathDesc
{
	ModelType Type;
	wstring Name;
	wstring ReadPath;
	wstring SaveMeshPath;
	wstring SaveMaterialPath;
};

struct AnimPathDesc
{
	wstring Name;
	wstring ReadPath;
	wstring SavePath;
};

class AssetManager
{
	using ModelMap = unordered_map<wstring, shared_ptr<Model>>;
	using ObjectList = vector<shared_ptr<GameObject>>;
private:
	static AssetManager* _instance;
public:
	static AssetManager* GetInstance()
	{
		if (_instance == nullptr)
			_instance = new AssetManager();

		return _instance;
	}
private:
	AssetManager();
	~AssetManager();
private:
	shared_ptr<Converter>	_converter;
	AssetPathDesc			_assetDesc;
	AnimPathDesc			_animDesc;
private:
	ModelMap	_models;
	ObjectList	_currentViewObjects;
public:
	bool AddModelAsset(wstring key, shared_ptr<Model> model);
	shared_ptr<Model> GetModelByName(wstring name);
public:

public:
	bool ReadAssetFile(AssetPathDesc& desc);
	bool ReadAnimationFile(AnimPathDesc& desc);
public:
	bool ExportMaterialData(wstring exportPath);
	bool ExportModelData(wstring exportPath);
	bool ExportAnimationData(wstring exportPath);
public:
	bool CreateModel();
	bool CreateObject();
	bool ReadAnimation();
public:
	void Init();
	void Update();
	void Clear();
};

#define MANAGER_ASSET() AssetManager::GetInstance()
