#include "ShaderManagerClass.h"

ShaderManagerClass::ShaderManagerClass()
{
	m_TextureShader = 0;
	m_LightShader = 0;
	m_MultiTexture = 0;
	m_FogShader = 0;
	m_ReflectionShader = 0;
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


	// Create the fog shader object.
	m_FogShader = new FogShaderClass(Light);
	if (!m_FogShader)
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

	// Initialize the fog shader object.
	result = m_FogShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the fog shader object.", L"Error", MB_OK);
		return false;
	}

	m_FogShader->setFogPosition(0.0f, 10.0f);

	// 반사 셰이더 객체를 생성합니다.
	m_ReflectionShader = new ReflectionShaderClass(Light);
	if (!m_ReflectionShader)
	{
		return false;
	}

	// 반사 셰이더 객체를 초기화합니다.
	result = m_ReflectionShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection shader object.", L"Error", MB_OK);
		return false;
	}
	return true;
}

void ShaderManagerClass::Shutdown()
{
	// 반사 셰이더 객체를 해제합니다.
	if (m_ReflectionShader)
	{
		m_ReflectionShader->Shutdown();
		delete m_ReflectionShader;
		m_ReflectionShader = 0;
	}

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


	// Release the fog shader object.
	if (m_FogShader)
	{
		m_FogShader->Shutdown();
		delete m_FogShader;
		m_FogShader = 0;
	}
	return;
}
