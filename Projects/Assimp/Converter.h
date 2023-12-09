#pragma once
#include "AsTypes.h"

class Converter
{
public:
	Converter();
	~Converter();
private:
	shared_ptr<Assimp::Importer> _importer;
	const aiScene*				_scene = nullptr;
	ModelType					_currentType = ModelType::None;
private:
	vector<shared_ptr<asBone>>		_bones;
	vector<shared_ptr<asMesh>>		_meshes;
	vector<shared_ptr<asMaterial>>	_materials;
private:
	wstring _assetPath = RESOURCES_ADDR_ASSET;
	wstring _modelPath = RESOURCES_ADDR_MESH;
	wstring _texturePath = RESOURCES_ADDR_TEXTURE;
private:
	void ReadModelData(aiNode* node, int32 index, int32 parent);
	void ReadMeshData(aiNode* node, int32 bone);
	void WriteModelFile(wstring finalPath);
private:
	void ReadMaterialData();
	void WriteMaterialData(wstring finalPath);
	string WriteTexture(string saveFolder, string file);
public:
	void ExportModelData(wstring savePath);
	void ExportMaterialData(wstring savePath);
public: 
	void ReadAssetFile(ModelType type, wstring fileName);
};

