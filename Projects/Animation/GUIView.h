#pragma once
#include "GUIInterface.h"

class GUIView : public GUIInterface
{
	using Super = GUIInterface;
public:
	GUIView();
	virtual ~GUIView();
private:
	bool _showLoadedAsset = false;
	bool _showBoneHierarchy = false;
	bool _showTransform = false;
	bool _showMaterial = false;
private:
	//Loaded Asset
	ImVec2 _loadedAssetPos;
	ImVec2 _loadedAssetSize;
	//BoneHierarchy
	ImVec2 _boneHierarchyPos;
	ImVec2 _boneHierarchySize;
private:
	const float* ConvertMatrixToFloat(Matrix& mat);
private:
	void DrawGrid();
	void LoadedAsset();
	void BoneHierarchy();
	void Transform();
	void Material();
	void Animation();
	void Lighting();
public:
	virtual void Update() override;
	virtual void Render() override;
}; 

