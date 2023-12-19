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
	//{
	//	shared_ptr<Converter> converter = make_shared<Converter>();

	//	converter->ReadAssetFile(ModelType::Skeletal, L"Ragnaros/Animations/Casting.fbx");
	//	converter->ExportAnimationData(L"Ragnaros/Casting");
	//}

	//리소스 매니저 초기화
	MANAGER_RESOURCES()->Init();
	{
		MANAGER_RESOURCES()->AddResource<Shader>(L"Default", _shader);
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


	CreateRagnaros();
	//CreateCoreHound();
	//CreateGiant();
	//CreateBaronGeddon();

	{
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.93, 0.91f, 0.67f, 1.0f);
		lightDesc.diffuse = Vec4(0.93, 0.91f, 0.67f, 1.0f);
		lightDesc.specular = Vec4(0.93, 0.91f, 0.67f, 1.0f);
		lightDesc.direction = Vec3(1.f, 0.f, 1.f);
		MANAGER_RENDERER()->PushLightData(lightDesc);
	}
}

void Demo::Update()
{
	_camera->Update();
	MANAGER_RENDERER()->Update();

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
	m1->ReadAnimation(L"Ragnaros/Appear");
	m1->ReadAnimation(L"Ragnaros/DisAppear");
	m1->ReadAnimation(L"Ragnaros/DisAppeared");
	m1->ReadAnimation(L"Ragnaros/Death");
	m1->ReadAnimation(L"Ragnaros/Battle");
	m1->ReadAnimation(L"Ragnaros/Casting");
	m1->ReadAnimation(L"Ragnaros/Attack1");
	m1->ReadAnimation(L"Ragnaros/Attack2");
	m1->ReadAnimation(L"Ragnaros/Ability");


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

	m1->ReadAnimation(L"CoreHound/Idle");
	m1->ReadAnimation(L"CoreHound/Walk");
	m1->ReadAnimation(L"CoreHound/Run");
	m1->ReadAnimation(L"CoreHound/Damaged");
	m1->ReadAnimation(L"CoreHound/Death");
	m1->ReadAnimation(L"CoreHound/Battle");
	m1->ReadAnimation(L"CoreHound/Attack1");
	m1->ReadAnimation(L"CoreHound/Attack2");

	//GameObejct
	_obj = make_shared<GameObject>();
	_obj->Awake();
	_obj->GetTransform()->SetPosition(Vec3(0, 0, 10));
	_obj->GetTransform()->SetScale(Vec3(0.01f));
	auto rot = _obj->GetTransform()->GetLocalRotation();
	rot.x += ::XMConvertToRadians(90.f);
	rot.y += ::XMConvertToRadians(90.f);
	_obj->GetTransform()->SetLocalRotation(rot);

	_obj->AddComponent(make_shared<ModelAnimator>(_shader));
	_obj->GetModelAnimator()->SetModel(m1);
}

void Demo::CreateGiant()
{
	//Model Import
	shared_ptr<Model> m1 = make_shared<Model>();
	m1->SetModelType(ModelType::Skeletal);
	m1->ReadModel(L"Giant/Giant");
	m1->ReadMaterial(L"Giant/Giant");

	m1->ReadAnimation(L"Giant/Idle");
	m1->ReadAnimation(L"Giant/Walk");
	m1->ReadAnimation(L"Giant/Run");
	m1->ReadAnimation(L"Giant/Damaged");
	m1->ReadAnimation(L"Giant/Death");
	m1->ReadAnimation(L"Giant/Battle");
	m1->ReadAnimation(L"Giant/Attack1");
	m1->ReadAnimation(L"Giant/Attack2");

	//GameObejct
	_obj = make_shared<GameObject>();
	_obj->Awake();
	_obj->GetTransform()->SetPosition(Vec3(0, 0, 10));
	_obj->GetTransform()->SetScale(Vec3(0.01f));
	auto rot = _obj->GetTransform()->GetLocalRotation();
	rot.x += ::XMConvertToRadians(90.f);
	rot.y += ::XMConvertToRadians(90.f);
	_obj->GetTransform()->SetLocalRotation(rot);

	_obj->AddComponent(make_shared<ModelAnimator>(_shader));
	_obj->GetModelAnimator()->SetModel(m1);
}

void Demo::CreateBaronGeddon()
{
	//Model Import
	shared_ptr<Model> m1 = make_shared<Model>();
	m1->SetModelType(ModelType::Skeletal);
	m1->ReadModel(L"BaronGeddon/BaronGeddon");
	m1->ReadMaterial(L"BaronGeddon/BaronGeddon");

	m1->ReadAnimation(L"BaronGeddon/Idle");
	m1->ReadAnimation(L"BaronGeddon/Walk");
	m1->ReadAnimation(L"BaronGeddon/Run");
	m1->ReadAnimation(L"BaronGeddon/Damaged");
	m1->ReadAnimation(L"BaronGeddon/Death");
	m1->ReadAnimation(L"BaronGeddon/Battle");
	m1->ReadAnimation(L"BaronGeddon/Casting");
	m1->ReadAnimation(L"BaronGeddon/Attack1");
	m1->ReadAnimation(L"BaronGeddon/Attack2");
	m1->ReadAnimation(L"BaronGeddon/Ability");


	//GameObejct
	_obj = make_shared<GameObject>();
	_obj->Awake();
	_obj->GetTransform()->SetPosition(Vec3(0, 0, 10));
	_obj->GetTransform()->SetScale(Vec3(0.01f));
	auto rot = _obj->GetTransform()->GetLocalRotation();
	rot.x += ::XMConvertToRadians(90.f);
	rot.y += ::XMConvertToRadians(90.f);
	_obj->GetTransform()->SetLocalRotation(rot);

	_obj->AddComponent(make_shared<ModelAnimator>(_shader));
	_obj->GetModelAnimator()->SetModel(m1);
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
