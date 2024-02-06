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

		if (CreateMeshAsset(_meshDesc))
		{
		}

		return true;
	}

	return false;
}

bool AssetManager::ReadAnimAssetFile(AnimPathDesc& desc)
{
	//desc init
	_animDesc.AnimName.clear();
	_animDesc.AnimOwnerName.clear();
	_animDesc.ReadAnimPath.clear();
	_animDesc.SaveAnimPath.clear();

	_animDesc.AnimName = wstring(desc.AnimName);
	_animDesc.AnimOwnerName = wstring(desc.AnimOwnerName);
	_animDesc.ReadAnimPath = wstring(desc.ReadAnimPath);
	_animDesc.SaveAnimPath = wstring(desc.SaveAnimPath);


	//converter init
	_converter->Init();

	//read animation file
	if (_converter->ReadAssetFile(_animDesc.ReadAnimPath))
	{
		if (ExportAnimationData(_animDesc.AnimName,_animDesc.SaveAnimPath))
		{
			if (CreateAnimAsset(_animDesc))
			{
				return true;
			}
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
	_converter->ExportAnimationData(name, exportPath);

	return true;
}

bool AssetManager::CreateMeshAsset(MeshPathDesc& desc)
{
	//Shader Set
	auto shader = MANAGER_RESOURCES()->GetResource<Shader>(L"Default");

	//Model Create
	shared_ptr<Model> model = make_shared<Model>();
	model->SetModelType(desc.Type);
	model->ReadModel(desc.SaveMeshPath);
	model->ReadMaterial(desc.SaveMaterialPath);

	//Object Create
	shared_ptr<GameObject> asset = make_shared<GameObject>();
	{
		//Init
		asset->SetName(desc.Name);
		asset->AddComponent(make_shared<ModelRenderer>(shader));
		asset->GetModelRenderer()->SetModel(model);
		asset->GetModelRenderer()->SetPass(0);
		asset->Awake();

		//Default Setting
		asset->GetTransform()->SetPosition(Vec3(0.f, -10.f, 0.f));
		asset->GetTransform()->SetLocalScale(Vec3(0.1f));
		auto rot = asset->GetTransform()->GetLocalRotation();
		rot.x += ::XMConvertToRadians(90.f);
		rot.y += ::XMConvertToRadians(90.f);
		asset->GetTransform()->SetRotation(rot);

		asset->SetActive(false);
	}

	//AssetData Save
	{
		AssetMeshData data;
		data.Name = _meshDesc.Name;
		data.Type = _meshDesc.Type;
		data.SourceMaterials = _converter->_materials;
		data.SourceBones = _converter->_bones;
		data.SourceMeshes = _converter->_meshes;
		data.Model = asset;
		_meshData.insert(make_pair(_meshDesc.Name, data));
	}

	return true;
}

bool AssetManager::CreateAnimAsset(AnimPathDesc& desc)
{
	shared_ptr<Model> loader = make_shared<Model>();
	loader->ReadAnimation(desc.SaveAnimPath);

	const auto& anim = loader->GetAnimationByIndex(0);

	if (anim)
	{
		wstring animName = desc.AnimOwnerName;
		animName += L"/";
		animName += desc.AnimName;

		AssetAnimData data;
		data.Name = animName;
		data.SourceAnim = _converter->_animation;
		data.Anim = anim;
		_animData.insert(make_pair(data.Name, data));

		return true;
	}

	return false;
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

		float clearColor[4] = { 1.f,1.f,1.f,0.75f };
		DC()->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());
		DC()->ClearRenderTargetView(rtv.Get(), clearColor);
		DC()->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
		DC()->RSSetViewports(1, &GRAPHICS()->GetViewport(1));
		DC()->OMSetBlendState(nullptr, 0, -1);
	}

	//Model Rendering
	{
	
	}

	{
		const auto& rtv = GRAPHICS()->GetRenderTargetView(0);
		const auto& dsv = GRAPHICS()->GetDepthStencilView(0);

		DC()->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());
		DC()->RSSetViewports(1, &GRAPHICS()->GetViewport(0));
	}
}

void AssetManager::Clear()
{
}


