#pragma once

#include "ShaderClass.h"
#include "lightclass.h"
#include "textureshaderclass.h"
#include "lightshaderclass.h"
#include "MultiTextureShaderClass.h"

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




	bool RenderTextureShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);

	bool RenderLightShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*,
		XMFLOAT3, XMFLOAT4, XMFLOAT4, XMFLOAT3, XMFLOAT4, float);

	bool RenderBumpMapShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*,
		ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);


private:
	ShaderManagerClass();

	static ShaderManagerClass* Instance;
	TextureShaderClass* m_TextureShader;
	LightShaderClass* m_LightShader;
	MultiTextureShaderClass* m_MultiTexture;
};

