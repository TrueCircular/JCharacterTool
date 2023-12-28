#pragma once
#include "GUIInterface.h"

class GUIView : public GUIInterface
{
	using Super = GUIInterface;
public:
	GUIView();
	virtual ~GUIView();
private:
	bool _isViewGrid = false;
	bool _isBoneHierarchy = false;
private:
	ImVec2 _boneHierarchyPos;
	ImVec2 _boneHierarchysize;
private:
	const float* ConvertMatrixToFloat(Matrix& mat);
private:
	void DrawGrid();
	void BoneHierarchy();
	void LoadedAsset();
public:
	virtual void Update() override;
	virtual void Render() override;
}; 

