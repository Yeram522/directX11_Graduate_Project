#include "ShaderManagerClass.h"

ShaderManagerClass::ShaderManagerClass()
{
	m_TextureShader = 0;
	m_LightShader = 0;
	m_MultiTexture = 0;
}

ShaderManagerClass::~ShaderManagerClass()
{

}

bool ShaderManagerClass::Initialize(ID3D11Device* device, HWND hwnd, LightClass* Light)
{
	bool result;
	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass(Light);
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass(Light);
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

		// Create the bump map shader object.
	m_MultiTexture = new MultiTextureShaderClass(Light);
	if (!m_MultiTexture)
	{
		return false;
	}

	// Initialize the bump map shader object.
	result = m_MultiTexture->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bump map shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void ShaderManagerClass::Shutdown()
{
	if (m_MultiTexture)
	{
		m_MultiTexture->Shutdown();
		delete m_MultiTexture;
		m_MultiTexture = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	return;
}
