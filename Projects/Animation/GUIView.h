#pragma once
#include "GUIInterface.h"

class GUIView : public GUIInterface
{
	friend class GUIFile;
	using Super = GUIInterface;
public:
	GUIView();
	virtual ~GUIView();
private:
	bool _showLoadedAsset = false;
	bool _showBoneHierarchy = false;
	bool _showInspector = false;
	bool _showAnimation = false;
	bool _showGrid = false;
private:
	//Loaded Asset
	ImVec2 _loadedAssetPos;
	ImVec2 _loadedAssetSize;
	//BoneHierarchy
	ImVec2 _boneHierarchyPos;
	ImVec2 _boneHierarchySize;
	//Inspector
	ImVec2 _inspectorPos;
	ImVec2 _inspectorSize;
	float _transformPos[3];
	float _transformRot[3];
	float _transformScale [3];
	bool _scaleCheck = false;
	//Animation
	ImVec2 _animationPos;
	ImVec2 _animationSize;
private:
	float* ConvertMatrixToFloat(Matrix& mat);
	void HelpMarker(const char* desc);
private:
	void DrawGrid();
	//
	void LoadedAsset();
	//
	void BoneHierarchy();
	//
	void Inspector();
	void Transform();
	void Mesh();
	void Material();
	//
	void Animation();
	//
	void Lighting();
public:
	virtual void Update() override;
	virtual void Render() override;
}; 

