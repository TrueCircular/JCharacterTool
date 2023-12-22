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
private:
	void DrawGrid();
	const float* ConvertMatrixToFloat(Matrix& mat);
public:
	virtual void Update() override;
	virtual void Render() override;
}; 

