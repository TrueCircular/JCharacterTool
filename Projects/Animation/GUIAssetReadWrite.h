#pragma once
#include "GUIInterface.h"

class GUIAssetReadWrite : public GUIInterface
{
	friend class ImGuiManager;
	using Super = GUIInterface;
public:
	GUIAssetReadWrite();
	virtual ~GUIAssetReadWrite();
private:
	wstring _filePath;
	wstring _fileName;
private:
	//Dialog
	ImVec2 _minDialogSize;
	ImVec2 _maxDialogSize;
	ImVec2 _readsaveDialogPos;
	//BoneHierarchy
	ImVec2 _hierarchySize;
	bool _isSaveMesh = false;
	bool _isReadMesh = false;
private:
	wstring SplitFileName(string name);
	void SavePoPUP();
	void BoneHierarchy();
	void MaterialView();
public:
	virtual void Update() override;
	virtual void Render() override;
};

