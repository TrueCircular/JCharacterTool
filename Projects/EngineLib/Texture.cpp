#include "pch.h"
#include "Texture.h"

Texture::Texture() : Super(ResourceBase(ResourceType::Texture))
{
	_device = GRAPHICS()->GetDevice();
}

Texture::Texture(ComPtr<ID3D11Device> device) : Super(ResourceBase(ResourceType::Texture))
{
	_device = device;
}

Texture::~Texture()
{
}

void Texture::CreateTexture(const wstring& path)
{
	HRESULT hr;
	DirectX::TexMetadata md;
	//PNG,JPG
	hr = DirectX::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, _img);
	CHECK(hr);

	hr = ::CreateShaderResourceView(_device.Get(), _img.GetImages(), _img.GetImageCount(),
		md, _shaderResourceView.GetAddressOf());
	CHECK(hr);

	_size.x = md.width;
	_size.y = md.height;
	_path = path;
}

void Texture::Load(const wstring& path)
{
	HRESULT hr;
	DirectX::TexMetadata md;
	//PNG,JPG
	hr = DirectX::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, _img);

	if (FAILED(hr))
	{
		//TGA
		hr = DirectX::LoadFromTGAFile(path.c_str(), TGA_FLAGS_NONE, &md, _img);

		if (FAILED(hr))
		{
			hr = DirectX::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, &md, _img);
		}
	}
	CHECK(hr);

	hr = ::CreateShaderResourceView(_device.Get(), _img.GetImages(), _img.GetImageCount(),
		md, _shaderResourceView.GetAddressOf());
	CHECK(hr);

	_size.x = md.width;
	_size.y = md.height;
	_path = path;
}
