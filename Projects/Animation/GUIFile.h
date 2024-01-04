#pragma once
#include "GUIInterface.h"

#pragma region Declaration
struct AssetPathDesc;
struct AnimPathDesc;
#pragma endregion

class GUIFile : public GUIInterface
{
	friend class ImGuiManager;
	using Super = GUIInterface;
public:
	GUIFile();
	virtual ~GUIFile();
private:
	//Cache
	wstring _filePath;
	wstring _fileName;
	AssetType _type;
private:
	//Dialog
	ImVec2 _minDialogSize;
	ImVec2 _maxDialogSize;
	ImVec2 _readsaveDialogPos;
private:
	//UI flag
	bool _isSaveMesh = false;
	bool _isReadMesh = false;
private:
	//Helper
	AssetPathDesc	CreateAssetPathDesc(wstring& fileName, wstring& filePath);
	AnimPathDesc	CreateAnimPathDesc(wstring& fileName, wstring& filePath);
	wstring			SplitFileName(string name);
private:
	void SavePoPUP();
	void ReadPoPUP();
public:
	virtual void Update() override;
	virtual void Render() override;
};

