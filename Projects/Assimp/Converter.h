#pragma once
class Converter
{
public:
	Converter();
	~Converter();
private:
	shared_ptr<Assimp::Importer> _importer;
	const aiScene* _scene;
	wstring _assetPath = RESOURCES_ADDR_ASSET_STATIC;
public: 
	void ReadAssetFile(wstring file);
};

