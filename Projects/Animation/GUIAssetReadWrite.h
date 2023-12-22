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
	ImVec2 _minDialogSize;
	ImVec2 _maxDialogSize;
	ImVec2 _readsaveDialogPos;

	bool _isSaveMesh = false;
	bool _isReadMesh = false;
private:
	void SavePoPUP();
public:
	virtual void Update() override;
	virtual void Render() override;
};

