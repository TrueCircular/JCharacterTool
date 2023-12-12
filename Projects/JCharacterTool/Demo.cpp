#include "pch.h"
#include "Demo.h"
#include "CameraMove.h"
#include "Converter.h"
#include "ImGuiManager.h"

void Demo::Init()
{
	//리소스 매니저 초기화
	MANAGER_RESOURCES()->Init();
	{
		MANAGER_RESOURCES()->AddResource<Shader>(L"Default", make_shared<Shader>(L"StaticMesh.fx"));
	}
	MANAGER_RENDERER()->Init(MANAGER_RESOURCES()->GetResource<Shader>(L"Default"));
	//Camera
	{
		_camera = make_shared<GameObject>();
		_camera->Awake();
		_camera->AddComponent(make_shared<Camera>());
		_camera->AddComponent(make_shared<CameraMove>());
	}
	
	//CreateTower();
	CreateTank();
}

void Demo::Update()
{
	_camera->Update();
	MANAGER_RENDERER()->Update();
	{
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.5f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(0.f);
		lightDesc.direction = Vec3(1.f, 0.f, 1.f);
		MANAGER_RENDERER()->PushLightData(lightDesc);
	}
	{
		_obj->Update();
	}
}

void Demo::Render()
{

}

void Demo::CreateTower()
{
	//Model Import
	shared_ptr<Model> m1 = make_shared<Model>();
	m1->SetModelType(ModelType::Static);
	m1->ReadModel(L"Tower/Tower");
	m1->ReadMaterial(L"Tower/Tower");
	//GameObejct
	_obj = make_shared<GameObject>();
	_obj->Awake();

	_obj->GetTransform()->SetPosition(Vec3(0, 0, 15));
	_obj->GetTransform()->SetScale(Vec3(1.0f));
	constexpr float angle = ::XMConvertToRadians(90.f);
	Vec3 rot = _obj->GetTransform()->GetLocalRotation();
	rot.x += angle;
	_obj->GetTransform()->SetLocalRotation(rot);

	_obj->AddComponent(make_shared<ModelRenderer>(MANAGER_RESOURCES()->GetResource<Shader>(L"Default")));
	_obj->GetModelRenderer()->SetModel(m1);
}

void Demo::CreateTank()
{
	//Model Import
	shared_ptr<Model> m1 = make_shared<Model>();
	m1->SetModelType(ModelType::Static);
	m1->ReadModel(L"Tank/Tank");
	m1->ReadMaterial(L"Tank/Tank");
	//GameObejct
	_obj = make_shared<GameObject>();
	_obj->Awake();

	_obj->GetTransform()->SetPosition(Vec3(0, 0, 15));
	_obj->GetTransform()->SetScale(Vec3(1.0f));

	_obj->AddComponent(make_shared<ModelRenderer>(MANAGER_RESOURCES()->GetResource<Shader>(L"Default")));
	_obj->GetModelRenderer()->SetModel(m1);
	//_obj->GetModelRenderer()->SetPass(1);
}
