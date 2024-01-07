#include "pch.h"
#include "AssetManager.h"

AssetManager* AssetManager::_instance = nullptr;

AssetManager::AssetManager()
{
	_converter = make_shared<Converter>();
}

AssetManager::~AssetManager()
{
}

bool AssetManager::ReadMeshAssetFile(MeshPathDesc& desc)
{
	//desc init
	_meshDesc = desc;

	//converter init
	_converter->Init();

	//read asset file
	if (_converter->ReadAssetFile(_meshDesc.ReadMeshPath))
	{
		ExportMaterialData(_meshDesc.SaveMaterialPath);
		ExportModelData(_meshDesc.SaveMeshPath);

		AssetData data;
		data.materials = _converter->_materials;
		data.meshes = _converter->_meshes;
		data.bones = _converter->_bones;

		_assetData.insert(make_pair(_meshDesc.Name, data));

		if (CreateMeshAsset(_meshDesc))
		{
			_currentAssets.push_back(GetAssetByName(_meshDesc.Name));
		}

		return true;
	}

	return false;
}

bool AssetManager::ReadAnimAssetFile(AnimPathDesc& desc)
{
	//desc init
	_animDesc = desc;

	//converter init
	_converter->Init();

	//read animation file
	if (_converter->ReadAssetFile(_animDesc.ReadAnimPath))
	{
		if (ExportAnimationData(_animDesc.SaveAnimPath))
		{
			return true;
		}
	}

	return false;
}

bool AssetManager::ExportMaterialData(wstring& exportPath)
{
	_converter->ExportMaterialData(exportPath);

	return true;
}

bool AssetManager::ExportMaterialData(wstring& name, wstring& exportPath)
{
	return false;
}

bool AssetManager::ExportModelData(wstring& exportPath)
{
	_converter->ExportModelData(exportPath);

	return true;
}

bool AssetManager::ExportModelData(wstring& name, wstring& exportPath)
{
	return false;
}

bool AssetManager::ExportAnimationData(wstring& exportPath)
{
	_converter->ExportAnimationData(exportPath);

	return true;
}

bool AssetManager::ExportAnimationData(wstring& name, wstring& exportPath)
{
	return false;
}

bool AssetManager::CreateMeshAsset(MeshPathDesc& desc)
{
	auto shader = MANAGER_RESOURCES()->GetResource<Shader>(L"Default");
	shared_ptr<Model> model = make_shared<Model>();
	model->SetModelType(desc.Type);
	model->ReadModel(desc.SaveMeshPath);
	model->ReadMaterial(desc.SaveMaterialPath);

	shared_ptr<GameObject> asset = make_shared<GameObject>();
	asset->AddComponent(make_shared<ModelRenderer>(shader));
	asset->GetModelRenderer()->SetModel(model);
	asset->GetModelRenderer()->SetPass(0);
	asset->Awake();

	asset->GetTransform()->SetPosition(Vec3(0.f, -10.f, 0.f));
	asset->GetTransform()->SetLocalScale(Vec3(0.1f));
	auto rot = asset->GetTransform()->GetLocalRotation();
	rot.x += ::XMConvertToRadians(90.f);
	rot.y += ::XMConvertToRadians(90.f);
	asset->GetTransform()->SetRotation(rot);
	_assets.insert(make_pair(desc.Name, asset));

	//test
	{
		shared_ptr<GameObject> obj1 = make_shared<GameObject>();
		obj1->Awake();
		obj1->GetTransform()->SetPosition(Vec3(2.f));

		shared_ptr<GameObject> obj2 = make_shared<GameObject>();
		obj2->Awake();
		obj2->GetTransform()->SetPosition(Vec3(3.f));
		obj1->GetTransform()->AddChild(obj2->GetTransform());

		shared_ptr<GameObject> obj3 = make_shared<GameObject>();
		obj3->Awake();
		obj3->GetTransform()->SetPosition(Vec3(4.f));
		obj1->GetTransform()->AddChild(obj3->GetTransform());

		shared_ptr<GameObject> obj4 = make_shared<GameObject>();
		obj4->Awake();
		obj4->GetTransform()->SetPosition(Vec3(5.f));
		obj3->GetTransform()->AddChild(obj4->GetTransform());

		asset->GetTransform()->AddChild(obj1->GetTransform());
		wstring tString = L"test";
		asset->GetTransform()->SaveMetaData(tString);
	}

	return true;
}

bool AssetManager::CreateAnimAsset(AnimPathDesc& desc)
{
	return false;
}

shared_ptr<GameObject> AssetManager::GetAssetByName(wstring& name)
{
	auto findIter = _assets.find(name);

	if (findIter != _assets.end())
		return findIter->second;

	return nullptr;
}

AssetData& AssetManager::GetAssetDataByName(wstring& name)
{
	AssetData ta;
	const auto& findIter = _assetData.find(name);

	if (findIter != _assetData.end())
	{
		ta = findIter->second;
	}

	return ta;
}

void AssetManager::Init()
{
	_converter->Init();
	ZeroMemory(&_meshDesc, sizeof(_meshDesc));
	ZeroMemory(&_animDesc, sizeof(_animDesc));
}

void AssetManager::Update()
{
	{
		auto rtv = GRAPHICS()->GetRenderTargetView(1);
		auto dsv = GRAPHICS()->GetDepthStencilView(1);

		float clearColor[4] = { 1.f,1.f,1.f,0.3f };
		DC()->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());
		DC()->ClearRenderTargetView(rtv.Get(), clearColor);
		DC()->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
		DC()->RSSetViewports(1, &GRAPHICS()->GetViewport());
	}

	for (int i = 0; i < _currentAssets.size(); i++)
	{
		_currentAssets[i]->Update();
		_currentAssets[i]->LateUpdate();
	}

	{
		const auto& rtv = GRAPHICS()->GetRenderTargetView(0);
		const auto& dsv = GRAPHICS()->GetDepthStencilView(0);

		DC()->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());
		DC()->RSSetViewports(1, &GRAPHICS()->GetViewport());
	}
}

void AssetManager::Clear()
{
}


