#pragma once
#include "Converter.h"
struct AssetPathDesc
{
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
class ModelManager
{
	using ModelMap = unordered_map<wstring, shared_ptr<Model>>;
	using ObjectList = vector<shared_ptr<GameObject>>;
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
private:
	ModelMap	_models;
	ObjectList	_objects;
public:
	bool AddModelAsset(wstring key, shared_ptr<Model> model);
	shared_ptr<Model> GetModelByName(wstring name);
public:
	vector<shared_ptr<asBone>> GetBones() { return _converter->_bones; }
	vector<shared_ptr<asMesh>> GetMeshes() { return _converter->_meshes; }
	vector<shared_ptr<asMaterial>> GetMaterials() { return _converter->_materials; }
public:
	void Init();
	void Update();
	void Clear();
	bool ReadAssetFile(AssetPathDesc desc);
public:
	bool ExportMaterialData(wstring exportPath);
	bool ExportModelData(wstring exportPath);
	bool ExportAnimationData(wstring exportPath);
};

#define MANAGER_MODEL() ModelManager::GetInstance()
