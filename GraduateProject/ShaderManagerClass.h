#pragma once

#include "ShaderClass.h"
#include "lightclass.h"
#include "textureshaderclass.h"
#include "lightshaderclass.h"
#include "MultiTextureShaderClass.h"
#include "FogShaderClass.h"
#include "WaterShaderClass.h"
#include "RefractionShaderClass.h"
#include "RenderTextureClass.h"
#include "GameObject.h"
class ShaderManagerClass
{
public:
	//���̴� �Ŵ��� = �̱���
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

	WaterShaderClass* getWaterShader()
	{
		return m_WaterShader;
	}
	RefractionShaderClass* getRefractionShader()
	{
		return m_RefractionShader;
	}


	void updateWaterTranslate(XMMATRIX reflectionMatrix)
	{
		// Update the position of the water to simulate motion.
		m_WaterShader->SetReflectionMatrix(reflectionMatrix);
		m_waterTranslation += 0.001f;
		if (m_waterTranslation > 1.0f)
		{
			m_waterTranslation -= 1.0f;
		}
		m_WaterShader->SetWaterTranslation(m_waterTranslation);

	}

	ID3D11ShaderResourceView* RenderRefractionToTexture(GameObject*, D3DClass*, CameraClass* , LightClass* );
	ID3D11ShaderResourceView* RenderReflectionToTexture(GameObject*, D3DClass*, CameraClass* , LightClass* );
private:
	ShaderManagerClass();
	RenderTextureClass* m_RefractionTexture, * m_ReflectionTexture;
	static ShaderManagerClass* Instance;
	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
	MultiTextureShaderClass* m_MultiTexture;
	FogShaderClass* m_FogShader;
	WaterShaderClass* m_WaterShader;
	RefractionShaderClass* m_RefractionShader;
	float m_waterHeight, m_waterTranslation;
};

