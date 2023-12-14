#pragma once
#include "engine/IExecute.h"

class Demo : public IExecute
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
private:
	void CreateTower();
	void CreateTank();
	void CreateRagnaros();
	void CreateCoreHound();
	void CreateMesh();
private:
	shared_ptr<Shader> _shader;
	//obj
	shared_ptr<GameObject> _obj;
	//Camera
	shared_ptr<GameObject> _camera;
};

