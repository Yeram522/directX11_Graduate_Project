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
#include "TransparentShaderClass.h"
#include "SkyDomeShaderClass.h"
#include "SkyPlaneShaderClass.h"
#include "ParticleShaderClass.h"

#include "GameObject.h"
class ShaderManagerClass
{
public:
	//¼ÎÀÌ´õ ¸Å´ÏÀú = ½Ì±ÛÅæ
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

	SkyDomeShaderClass* getSkyDomeShader()
	{
		return m_SkyDomeShader;
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

	TransparentShaderClass* getTransparentSahder()
	{
		return m_TransparentShader;
	}

	SkyPlaneShaderClass* getSkyPlaneShader()
	{
		return m_SkyPlaneShader;
	}

	ParticleShaderClass* getParticleShader()
	{
		return m_ParticleShader;
	}
	RenderTextureClass* getReflectionTexture()
	{
		return m_ReflectionTexture;
	}

	RenderTextureClass* getRefractionTexture()
	{
		return m_RefractionTexture;
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

	bool RenderRefractionToTexture(GameObject*, D3DClass*, CameraClass* , LightClass* );
	bool RenderReflectionToTexture(GameObject*, D3DClass*, CameraClass* , LightClass* );
private:
	ShaderManagerClass();
	RenderTextureClass* m_RefractionTexture, *m_ReflectionTexture;
	static ShaderManagerClass* Instance;
	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
	MultiTextureShaderClass* m_MultiTexture;
	FogShaderClass* m_FogShader;
	WaterShaderClass* m_WaterShader;
	RefractionShaderClass* m_RefractionShader;
	TransparentShaderClass* m_TransparentShader;
	SkyDomeShaderClass* m_SkyDomeShader;
	SkyPlaneShaderClass* m_SkyPlaneShader;
	ParticleShaderClass* m_ParticleShader;
	float m_waterHeight, m_waterTranslation;
};

