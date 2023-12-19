#pragma once

enum class GUIType
{
	None,
	AssetReadWrite,
	AnimationReadWrite,
	MeshEditing,
	End
};

class GUIInterface
{
public:
	GUIInterface(GUIType type);
	virtual ~GUIInterface();
protected:
	GUIType _type;
	ImGuiFileDialog _dialog;
public:
	virtual void Update() = 0;
	virtual void Render() = 0;
};

