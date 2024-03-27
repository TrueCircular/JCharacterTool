#include "pch.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator() : Super(ComponentType::ModelAnimator)
{
}

ModelAnimator::ModelAnimator(shared_ptr<Shader> shader) : Super(ComponentType::ModelAnimator), _shader(shader)
{
}

ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::SetModel(shared_ptr<Model> model)
{
	_model = model;

	const auto& materials = _model->GetMaterials();
	for (auto& material : materials)
	{
		material->SetShader(_shader);
	}
}

void ModelAnimator::CreateTexture()
{
	if (_model->GetAnimationCount() == 0)
		return;

	if (_animTransforms.size() > 0)
	{
		_animTransforms.clear();
	}

	_animTransforms.resize(_model->GetAnimationCount());
	for (uint32 i = 0; i < _model->GetAnimationCount(); i++)
		CreateAnimationTransform(i);

	// Creature Texture
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = MAX_MODEL_TRANSFORMS * 4;
		desc.Height = MAX_MODEL_KEYFRAMES;
		desc.ArraySize = _model->GetAnimationCount();
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 16����Ʈ
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		const uint32 dataSize = MAX_MODEL_TRANSFORMS * sizeof(Matrix);
		const uint32 pageSize = dataSize * MAX_MODEL_KEYFRAMES;
		void* mallocPtr = ::malloc(pageSize * _model->GetAnimationCount());

		// ����ȭ�� �����͸� �����Ѵ�.
		for (uint32 c = 0; c < _model->GetAnimationCount(); c++)
		{
			uint32 startOffset = c * pageSize;

			BYTE* pageStartPtr = reinterpret_cast<BYTE*>(mallocPtr) + startOffset;

			for (uint32 f = 0; f < MAX_MODEL_KEYFRAMES; f++)
			{
				void* ptr = pageStartPtr + dataSize * f;
				::memcpy(ptr, _animTransforms[c].transforms[f].data(), dataSize);
			}
		}

		// ���ҽ� �����
		vector<D3D11_SUBRESOURCE_DATA> subResources(_model->GetAnimationCount());

		for (uint32 c = 0; c < _model->GetAnimationCount(); c++)
		{
			void* ptr = (BYTE*)mallocPtr + c * pageSize;
			subResources[c].pSysMem = ptr;
			subResources[c].SysMemPitch = dataSize;
			subResources[c].SysMemSlicePitch = pageSize;
		}

		HRESULT hr = DEVICE()->CreateTexture2D(&desc, subResources.data(), _texture.GetAddressOf());
		CHECK(hr);

		::free(mallocPtr);
	}

	// Create SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = _model->GetAnimationCount();

		HRESULT hr = DEVICE()->CreateShaderResourceView(_texture.Get(), &desc, _srv.GetAddressOf());
		CHECK(hr);
	}
}

void ModelAnimator::CreateAnimationTransform(uint32 index)
{
	vector<Matrix> tempAnimBoneTransforms(MAX_MODEL_TRANSFORMS, Matrix::Identity);

	shared_ptr<ModelAnimation> animation = _model->GetAnimationByIndex(index);

	for (uint32 f = 0; f < animation->frameCount; f++)
	{
		for (uint32 b = 0; b < _model->GetBoneCount(); b++)
		{
			shared_ptr<ModelBone> bone = _model->GetBoneByIndex(b);

			Matrix matAnimation;

			shared_ptr<ModelKeyframe> frame = animation->GetKeyframe(bone->name);

			if (frame != nullptr)
			{
				ModelKeyframeData& data = frame->transforms[f];

				Matrix S, R, T;
				S = Matrix::CreateScale(data.scale.x, data.scale.y, data.scale.z);
				R = Matrix::CreateFromQuaternion(data.rotation);
				T = Matrix::CreateTranslation(data.translation.x, data.translation.y, data.translation.z);

				matAnimation = S * R * T;
			}
			else
			{
				matAnimation = Matrix::Identity;
			}

			// [ !!!!!!! ]
			Matrix toRootMatrix = bone->transform;
			Matrix invGlobal = toRootMatrix.Invert();

			int32 parentIndex = bone->parentIndex;

			Matrix matParent = Matrix::Identity;
			if (parentIndex >= 0)
				matParent = tempAnimBoneTransforms[parentIndex];

			tempAnimBoneTransforms[b] = matAnimation * matParent;

			// ���
			_animTransforms[index].transforms[f][b] = invGlobal * tempAnimBoneTransforms[b];
		}
	}
}

void ModelAnimator::SetAnimationByIndex(int32 index)
{
}

void ModelAnimator::Awake()
{
	//_model = GetGameObject()->GetModelRenderer()->GetModel();
	//assert(_model != nullptr);
	//_shader = GetGameObject()->GetModelRenderer()->GetShader();
	//assert(_shader != nullptr);
}

void ModelAnimator::Update()
{
	if (_isPlay)
	{
		if (_model == nullptr || _shader == nullptr)
			return;

		_currentAnimCount = _model->GetAnimationCount();

		if (_texture == nullptr || _currentAnimCount != _prevAnimCount)
			CreateTexture();

		if (_isLoop)
		{
			_keyFrameDesc.sumTime += MANAGER_TIME()->GetDeltaTime();
			_currentAnim = _model->GetAnimationByIndex(_keyFrameDesc.animIndex);
			if (_currentAnim)
			{
				_timePerFrame = 1 / (_currentAnim->frameRate * _keyFrameDesc.speed);

				if (_keyFrameDesc.sumTime >= _timePerFrame)
				{
					_keyFrameDesc.currentFrame = (_keyFrameDesc.currentFrame + 1) % _currentAnim->frameCount;
					_keyFrameDesc.nextFrame = (_keyFrameDesc.currentFrame + 1) % _currentAnim->frameCount;
					_keyFrameDesc.sumTime = 0.f;
				}

				_keyFrameDesc.ratio = (_keyFrameDesc.sumTime / _timePerFrame);
			}
		}
		else
		{
			_keyFrameDesc.sumTime += MANAGER_TIME()->GetDeltaTime();
			_currentAnim = _model->GetAnimationByIndex(_keyFrameDesc.animIndex);
			if (_currentAnim)
			{
				_timePerFrame = 1 / (_currentAnim->frameRate * _keyFrameDesc.speed);

				if (_keyFrameDesc.sumTime >= _timePerFrame)
				{
					_keyFrameDesc.currentFrame = (_keyFrameDesc.currentFrame + 1) % _currentAnim->frameCount;
					_keyFrameDesc.nextFrame = (_keyFrameDesc.currentFrame + 1) % _currentAnim->frameCount;
					_keyFrameDesc.sumTime = 0.f;
				}

				_keyFrameDesc.ratio = (_keyFrameDesc.sumTime / _timePerFrame);
			}
		}

		if (_animTransforms.size() > 0)
		{
			SetPass(1);
		}
		else if (_animTransforms.size() <= 0)
		{
			SetPass(0);
		}

		// �ִϸ��̼� ���� ������ ����
		MANAGER_RENDERER()->PushKeyframeData(_keyFrameDesc);

		// SRV�� ���� ���� ����
		_shader->GetSRV("TransformMap")->SetResource(_srv.Get());

		//Bone
		BoneDesc boneDesc;

		const uint32 boneCount = _model->GetBoneCount();
		for (uint32 i = 0; i < boneCount; i++)
		{
			shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
			boneDesc.transforms[i] = bone->transform;
		}
		MANAGER_RENDERER()->PushBoneData(boneDesc);

		auto world = GetTransform()->GetWorldMatrix();
		MANAGER_RENDERER()->PushTransformData(TransformDesc{ world });

		const auto& meshes = _model->GetMeshes();
		for (auto& mesh : meshes)
		{
			if (mesh->material)
				mesh->material->Update();

			//Bone Index
			_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

			uint32 stride = mesh->vertexBuffer->GetStride();
			uint32 offset = mesh->vertexBuffer->GetOffset();

			DC()->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetBuffer().GetAddressOf(),
				&stride, &offset);
			DC()->IASetIndexBuffer(mesh->indexBuffer->GetBuffer().Get(),
				DXGI_FORMAT_R32_UINT, 0);

			_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
		}

		_prevAnimCount = _currentAnimCount;
	}
}

