#include "pch.h"
#include "Demo.h"

void Demo::Init()
{
	//���ҽ� �Ŵ��� �ʱ�ȭ
	MANAGER_RESOURCES()->Init();
	{
		_shader = make_shared<Shader>(L"GameObject.fx");
		MANAGER_RESOURCES()->AddResource<Shader>(L"Default", _shader);
	}
	//���� �Ŵ��� �ʱ�ȭ
	MANAGER_RENDERER()->Init(_shader);
	//light
	{
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.93, 0.91f, 0.67f, 1.0f);
		lightDesc.diffuse = Vec4(0.93, 0.91f, 0.67f, 1.0f);
		lightDesc.specular = Vec4(0.93, 0.91f, 0.67f, 1.0f);
		lightDesc.direction = Vec3(1.f, -1.f, -1.f);
		MANAGER_RENDERER()->PushLightData(lightDesc);
	}
}

void Demo::Update()
{
	MANAGER_RENDERER()->Update();
}

void Demo::Render()
{
}
