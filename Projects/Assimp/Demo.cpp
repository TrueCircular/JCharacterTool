#include "pch.h"
#include "Demo.h"
#include "CameraMove.h"
#include "Converter.h"

void Demo::Init()
{
	//Model Read
	//{
	//	shared_ptr<Converter> converter = make_shared<Converter>();
	//	//fbx-> memory
	//	converter->ReadAssetFile(ModelType::Static, L"House/House.fbx");
	//	//memory->customData
	//	converter->ExportMaterialData(L"House/House");
	//	converter->ExportModelData(L"House/House");
	//	//customData->memory
	//}
	{
		shared_ptr<Converter> converter = make_shared<Converter>();
		//fbx-> memory
		converter->ReadAssetFile(ModelType::Static, L"Tower/Tower.fbx");
		//memory->customData
		converter->ExportMaterialData(L"Tower/Tower");
		converter->ExportModelData(L"Tower/Tower");
		//customData->memory
	}
	//리소스 매니저 초기화
	MANAGER_RESOURCES()->Init();
	{
	}
	//Camera
	{
		_camera = make_shared<GameObject>();
		_camera->Awake();
		_camera->AddComponent(make_shared<Camera>());
		_camera->AddComponent(make_shared<CameraMove>());
	}

}

void Demo::Update()
{
	_camera->Update();
	//MANAGER_RENDERER()->Update();
}

void Demo::Render()
{
}
