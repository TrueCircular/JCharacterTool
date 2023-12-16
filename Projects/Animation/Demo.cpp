#include "pch.h"
#include "Demo.h"
#include "CameraMove.h"
#include "Converter.h"
#include "ImGuiManager.h"

void Demo::Init()
{
	_shader = make_shared<Shader>(L"Animation.fx");
	//{
	//	shared_ptr<Converter> converter = make_shared<Converter>();
	//	converter->ReadAssetFile(ModelType::Skeletal, L"Ragnaros/Ragnaros.fbx");
	//	converter->ExportMaterialData(L"Ragnaros/Ragnaros");
	//	converter->ExportModelData(L"Ragnaros/Ragnaros");
	//}
	{
		shared_ptr<Converter> converter = make_shared<Converter>();
		converter->ReadAssetFile(ModelType::Skeletal, L"Ragnaros/Animations/Stun.fbx");
		converter->ExportAnimationData(L"Ragnaros/Stun");
	}

	//{
	//	shared_ptr<Converter> converter = make_shared<Converter>();
	//	converter->ReadAssetFile(ModelType::Skeletal, L"Kachujin/Idle.fbx");
	//	converter->ExportAnimationData(L"Kachujin/Idle");
	//}

	//리소스 매니저 초기화
	MANAGER_RESOURCES()->Init();
	{
		MANAGER_RESOURCES()->AddResource<Shader>(L"Default", make_shared<Shader>(L"Animation.fx"));
	}
	MANAGER_RENDERER()->Init(_shader);
	//Camera
	{
		_camera = make_shared<GameObject>();
		_camera->Awake();
		_camera->GetTransform()->SetPosition(Vec3(0.f, 0.f, -2.f));
		_camera->AddComponent(make_shared<Camera>());
		_camera->AddComponent(make_shared<CameraMove>());
	}

	//CreateTower();
	//CreateTank();
	CreateRagnaros();
	//CreateMesh();
	//CreateCoreHound();
}

void Demo::Update()
{
	_camera->Update();
	MANAGER_RENDERER()->Update();
	{
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(1.0, 1.f, 1.f, 1.0f);
		lightDesc.diffuse = Vec4(1.0, 1.f, 1.f, 1.0f);
		lightDesc.specular = Vec4(1.0, 1.f, 0.f, 1.0f);
		lightDesc.direction = Vec3(1.f, 1.f, 1.f);
		MANAGER_RENDERER()->PushLightData(lightDesc);
	}
	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::KEY_1))
	{
		_obj->GetModelAnimator()->SetPass(0);
	}
	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::KEY_2))
	{
		_obj->GetModelAnimator()->SetPass(1);
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
}

void Demo::CreateRagnaros()
{
	//Model Import
	shared_ptr<Model> m1 = make_shared<Model>();
	m1->SetModelType(ModelType::Skeletal);
	m1->ReadModel(L"Ragnaros/Ragnaros");
	m1->ReadMaterial(L"Ragnaros/Ragnaros");

	m1->ReadAnimation(L"Ragnaros/Idle");
	m1->ReadAnimation(L"Ragnaros/Roar");
	m1->ReadAnimation(L"Ragnaros/Stun");
	m1->ReadAnimation(L"Ragnaros/Death");
	m1->ReadAnimation(L"Ragnaros/Battle");
	m1->ReadAnimation(L"Ragnaros/Attack1");
	m1->ReadAnimation(L"Ragnaros/Attack2");
	m1->ReadAnimation(L"Ragnaros/Casting");
	m1->ReadAnimation(L"Ragnaros/Ability");
	m1->ReadAnimation(L"Ragnaros/Appear");
	m1->ReadAnimation(L"Ragnaros/DisAppear");
	m1->ReadAnimation(L"Ragnaros/DisAppeared");

	//GameObejct
	_obj = make_shared<GameObject>();
	_obj->Awake();

	_obj->GetTransform()->SetPosition(Vec3(0, 0, 50));
	_obj->GetTransform()->SetScale(Vec3(0.01f));
	auto rot = _obj->GetTransform()->GetLocalRotation();
	rot.x += ::XMConvertToRadians(90.f);
	rot.y += ::XMConvertToRadians(90.f);
	_obj->GetTransform()->SetLocalRotation(rot);

	_obj->AddComponent(make_shared<ModelAnimator>(_shader));
	_obj->GetModelAnimator()->SetModel(m1);
	//_obj->GetModelAnimator()->SetPass(1);
}

void Demo::CreateCoreHound()
{
	//Model Import
	shared_ptr<Model> m1 = make_shared<Model>();
	m1->SetModelType(ModelType::Skeletal);
	m1->ReadModel(L"CoreHound/CoreHound");
	m1->ReadMaterial(L"CoreHound/CoreHound");
	//GameObejct
	_obj = make_shared<GameObject>();
	_obj->Awake();
	_obj->GetTransform()->SetPosition(Vec3(0, 0, 10));
	_obj->GetTransform()->SetScale(Vec3(0.01f));
	auto rot = _obj->GetTransform()->GetLocalRotation();
	rot.x += ::XMConvertToRadians(90.f);
	rot.y += ::XMConvertToRadians(90.f);
	_obj->GetTransform()->SetLocalRotation(rot);

	_obj->AddComponent(make_shared<ModelRenderer>(MANAGER_RESOURCES()->GetResource<Shader>(L"Default")));
	_obj->GetModelRenderer()->SetModel(m1);
}

void Demo::CreateMesh()
{
	//Model Import
	shared_ptr<Model> m1 = make_shared<Model>();
	m1->SetModelType(ModelType::Skeletal);
	m1->ReadModel(L"Kachujin/Kachujin");
	m1->ReadMaterial(L"Kachujin/Kachujin");

	m1->ReadAnimation(L"Kachujin/Idle");
	//m1->ReadAnimation(L"Kachujin/Run");
	//m1->ReadAnimation(L"Kachujin/Slash");

	//GameObejct
	_obj = make_shared<GameObject>();
	_obj->Awake();

	_obj->GetTransform()->SetPosition(Vec3(0, 0, 1));
	_obj->GetTransform()->SetScale(Vec3(0.01f));

	_obj->AddComponent(make_shared<ModelAnimator>(_shader));
	_obj->GetModelAnimator()->SetModel(m1);
	//_obj->GetModelAnimator()->SetPass(1);
}
