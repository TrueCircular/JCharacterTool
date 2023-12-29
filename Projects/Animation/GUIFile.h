#pragma once
#include "GUIInterface.h"

class GUIFile : public GUIInterface
{
	friend class ImGuiManager;
	using Super = GUIInterface;
public:
	GUIFile();
	virtual ~GUIFile();
private:
	wstring _filePath;
	wstring _fileName;
private:
	//Dialog
	ImVec2 _minDialogSize;
	ImVec2 _maxDialogSize;
	ImVec2 _readsaveDialogPos;
	bool _isSaveMesh = false;
	bool _isReadMesh = false;
	//AssetType
	AssetType _type;
private:
	wstring SplitFileName(string name);
	void SavePoPUP();
public:
	virtual void Update() override;
	virtual void Render() override;
};

