#pragma once

struct MetaData
{

};

struct TransformMetaData : MetaData
{
	Vec3 position;
	Vec3 rotation;
	Vec3 scale;
};

struct ModelMetaData : MetaData
{
	ModelType type;
	wstring modelPath;
	wstring materialPath;
	wstring animationPath;
};
