#pragma once
#include "Component.h"

class Model;
class Shader;
class Material;

class ModelAnimator : public Component
{
	using Super = Component;
public:
	ModelAnimator(shared_ptr<Shader> shader);
	virtual ~ModelAnimator();
private:
	shared_ptr<Shader> _shader;
	uint8 _pass = 0;
	shared_ptr<Model> _model;
private:
	vector<AnimTransform> _animTransforms;
	ComPtr<ID3D11Texture2D> _texture;
	ComPtr<ID3D11ShaderResourceView> _srv;
private:
	//AnimData
	KeyframeDesc _desc;
private:
	void CreateTexture();
	void CreateAnimationTransform(uint32 index);
public:
	void SetModel(shared_ptr<Model> model);
	void SetPass(uint8 pass) { _pass = pass; }
public:
	virtual void Update() override;
};

