#include "pch.h"
#include "Demo.h"
#include "CameraMove.h"
#include "Converter.h"

void Demo::Init()
{
	{
		shared_ptr<Converter> converter = make_shared<Converter>();
		//fbx-> memory
		converter->ReadAssetFile(ModelType::Static, L"Tank/Tank.fbx");
		//memory->customData
		converter->ExportMaterialData(L"Tank/Tank");
		converter->ExportModelData(L"Tank/Tank");
		//customData->memory
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
}

void Demo::Render()
{
}
