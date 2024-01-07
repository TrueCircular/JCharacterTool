#pragma once

struct GameData
{
};

struct TransformData
{
	Vec3 position;
	Vec3 rotation;
	Vec3 scale;
};

struct TransformMetaData : GameData
{
	TransformData local;
	TransformData parent;
	vector<TransformData> child;
};

struct ModelMetaData : GameData
{
	uint16	type;
	wstring modelPath;
	wstring materialPath;
	wstring animationPath;
};

class MetaData
{
private:
	shared_ptr<GameData> _data;
public:
	shared_ptr<GameData> GetMetaData() { return _data; }
	void SetData(shared_ptr<GameData> data) { _data = data; }
};
