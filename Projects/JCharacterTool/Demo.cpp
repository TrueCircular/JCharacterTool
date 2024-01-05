#include "pch.h"
#include "Demo.h"
#include "CameraMove.h"
#include "Converter.h"
#include "ImGuiManager.h"
#include "AssetManager.h"

void Demo::Init()
{
	//리소스 매니저 초기화
	MANAGER_RESOURCES()->Init();
	{
		_shader = make_shared<Shader>(L"GameObject.fx");
		MANAGER_RESOURCES()->AddResource<Shader>(L"Default", _shader);
	}
	//랜더 매니저 초기화
	MANAGER_RENDERER()->Init(_shader);
	//Camera
	{
		_camera = make_shared<GameObject>();
		_camera->Awake();
		_camera->GetTransform()->SetPosition(Vec3(0.f, 8.f, -50.f));
		_camera->AddComponent(make_shared<Camera>());
		_camera->AddComponent(make_shared<CameraMove>());
	}
	//light
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
	MANAGER_ASSET()->Update();
}

void Demo::Render()
{
}

void Demo::CreateRagnaros()
{
	//Model Import
	shared_ptr<Model> m1 = make_shared<Model>();
	m1->SetModelType(ModelType::Skeletal);
	wstring adr = RESOURCES_ADDR_MESH_SKELETAL;
	adr += L"Ragnaros";
	adr += L"/Ragnaros.mesh";
	m1->ReadModel(adr);
	wstring adr2 = RESOURCES_ADDR_TEXTURE_SKELETAL;
	adr2 += L"Ragnaros";
	adr2 += L"/Ragnaros.xml";
	m1->ReadMaterial(adr2);

	//GameObejct
	//_obj = make_shared<GameObject>();
	//_obj->AddComponent(make_shared<ModelRenderer>(_shader));
	//_obj->GetModelRenderer()->SetModel(m1);
	////_obj->AddComponent(make_shared<ModelAnimator>(_shader));
	////_obj->GetModelAnimator()->SetPlay(true);
	//_obj->Awake();

	//_obj->GetTransform()->SetPosition(Vec3(0, 0, 0));
	//_obj->GetTransform()->SetScale(Vec3(0.01f));
	//auto rot = _obj->GetTransform()->GetLocalRotation();
	//rot.x += ::XMConvertToRadians(90.f);
	//rot.y += ::XMConvertToRadians(90.f);
	//_obj->GetTransform()->SetLocalRotation(rot);
	//_obj->GetModelRenderer()->SetPass(0);
	////_obj->GetModelAnimator()->SetPass(1);
}
