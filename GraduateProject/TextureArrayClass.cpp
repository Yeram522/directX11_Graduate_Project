#include "TextureArrayClass.h"
#include "DDSTextureLoader.h"


using namespace DirectX;
TextureArrayClass::TextureArrayClass()
{
	m_textures[0] = 0;
	m_textures[1] = 0;
}


TextureArrayClass::TextureArrayClass(const TextureArrayClass& other)
{
}


TextureArrayClass::~TextureArrayClass()
{
}

bool TextureArrayClass::Initialize(ID3D11Device* device, const WCHAR* filename1, const WCHAR* filename2)
{
	HRESULT result;


	// Load the first texture in.
	result = CreateDDSTextureFromFile(device, filename1, nullptr, &m_textures[0]);
	if (FAILED(result))
	{
		return false;
	}

	// Load the second texture in.
	result = CreateDDSTextureFromFile(device, filename2, nullptr, &m_textures[1]);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool TextureArrayClass::Initialize(ID3D11Device* device, ID3D11ShaderResourceView* texture1, ID3D11ShaderResourceView* texture2)
{
	m_textures[0] = texture1;
	m_textures[1] = texture2;

	return true;
}

void TextureArrayClass::Shutdown()
{
	// Release the texture resources.
	if (m_textures[0])
	{
		m_textures[0]->Release();
		m_textures[0] = 0;
	}

	if (m_textures[1])
	{
		m_textures[1]->Release();
		m_textures[1] = 0;
	}

	return;
}

ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray()
{
	return m_textures;
}