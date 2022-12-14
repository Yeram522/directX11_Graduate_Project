////////////////////////////////////////////////////////////////////////////////
// Filename: skyplaneshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SKYPLANESHADERCLASS_H_
#define _SKYPLANESHADERCLASS_H_

//////////////
// INCLUDES //
//////////////
#include "ShaderClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SkyPlaneShaderClass
////////////////////////////////////////////////////////////////////////////////
class SkyPlaneShaderClass : public ShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct SkyBufferType
	{
		float firstTranslationX;
		float firstTranslationZ;
		float secondTranslationX;
		float secondTranslationZ;
		float brightness;
		XMFLOAT3 padding;
	};

public:
	SkyPlaneShaderClass(LightClass*);
	~SkyPlaneShaderClass();

	bool Initialize(ID3D11Device*, HWND) override;
	void Shutdown() override;

	void SetRenderInfo(float, float, float, float, float);
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView**, XMFLOAT3 cameraPosition = XMFLOAT3(0, 0, 0)) override;

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*,const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND,const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, float, float,
		float, float, float);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_skyBuffer;

	float m_firstTranslationX;
	float m_firstTranslationZ;
	float m_secondTranslationX;
	float m_secondTranslationZ;
	float m_brightness;
};
#endif