#pragma once

#include "ShaderClass.h"
#include "lightclass.h"
#include "textureshaderclass.h"
#include "lightshaderclass.h"
#include "MultiTextureShaderClass.h"
#include "FogShaderClass.h"
#include "ReflectionShaderClass.h"

class ShaderManagerClass
{
public:
	//ºŒ¿Ã¥ı ∏≈¥œ¿˙ = ΩÃ±€≈Ê
	static ShaderManagerClass* GetInstance()
	{
		if (Instance == nullptr) {
			Instance = new ShaderManagerClass();
		}
		return Instance;
	}
	~ShaderManagerClass();

	bool Initialize(ID3D11Device*, HWND, LightClass*);
	void Shutdown();

	LightShaderClass* getLightShader()
	{
		return m_LightShader;
	}



	TextureShaderClass* getTextureShader()
	{
		return m_TextureShader;
	}

	MultiTextureShaderClass* getMultiTextureShader()
	{
		return m_MultiTexture;
	}


	FogShaderClass* getFogShader()
	{
		return m_FogShader;
	}



private:
	ShaderManagerClass();

	static ShaderManagerClass* Instance;
	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
	MultiTextureShaderClass* m_MultiTexture;
	FogShaderClass* m_FogShader;

	//Reflection
	ReflectionShaderClass* m_ReflectionShader;
};

