#pragma once
#include "Converter.h"

struct AssetData
{
};

struct AssetMeshData : public AssetData
{
	wstring							Name;
	ModelType						Type;
	vector<shared_ptr<asBone>>		SourceBones;
	vector<shared_ptr<asMesh>>		SourceMeshes;
	vector<shared_ptr<asMaterial>>	SourceMaterials;
	shared_ptr<GameObject>			Model;
};

struct AssetAnimData : public AssetData
{
	wstring					Name;
	shared_ptr<asAnimation>	SourceAnim;
	shared_ptr<ModelAnimation> Anim;
};

struct MeshPathDesc
{
	wstring Name;
	wstring ReadMeshPath;
	wstring SaveMeshPath;
	wstring SaveMaterialPath;
	ModelType Type = ModelType::None;
};

struct AnimPathDesc
{
	wstring AnimOwnerName;
	wstring AnimName;
	wstring ReadAnimPath;
	wstring SaveAnimPath;
};

class AssetManager
{
	using MeshDataDictionary = map<wstring, AssetMeshData>;
	using AnimDataDictionary = map<wstring, AssetAnimData>;
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
	MeshPathDesc			_meshDesc;
	AnimPathDesc			_animDesc;
private:
	MeshDataDictionary		_meshData;
	AnimDataDictionary		_animData;
private:
	shared_ptr<GameObject>			_selectedAsset;
	shared_ptr<ModelAnimation>		_selectedAnimation;
public:
	//Read Asset
	bool ReadMeshAssetFile(MeshPathDesc& desc);
	bool ReadAnimAssetFile(AnimPathDesc& desc);
public:
	//Export Asset
	bool ExportMaterialData(wstring& exportPath);
	bool ExportMaterialData(wstring& name, wstring& exportPath);

	bool ExportModelData(wstring& exportPath);
	bool ExportModelData(wstring& name, wstring& exportPath);

	bool ExportAnimationData(wstring& exportPath);
	bool ExportAnimationData(wstring& name, wstring& exportPath);
public:
	bool CreateMeshAsset(MeshPathDesc& desc);
	bool CreateAnimAsset(AnimPathDesc& desc);
public:
	MeshDataDictionary GetLoadedMeshDataList() const { return _meshData; }
	AnimDataDictionary GetLoadedAnimDataList() const { return _animData; }
public:
	void Init();
	void Update();
	void Clear();
};

#define MANAGER_ASSET() AssetManager::GetInstance()
