////////////////////////////////////////////////////////////////////////////////
// Filename: textureshaderclass.h
// The TextureShaderClass is just an updated version of the ColorShaderClass from 
// the previous tutorial. This class will be used to draw the 3D models using vertex 
// and pixel shaders.
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "ShaderClass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureShaderClass
////////////////////////////////////////////////////////////////////////////////
class TextureShaderClass : public ShaderClass
{
public:
	TextureShaderClass(LightClass* Light);
	~TextureShaderClass();

	bool Initialize(ID3D11Device*, HWND) override;
	void Shutdown() override;
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView**,
		XMFLOAT3 cameraPosition = XMFLOAT3(0, 0, 0)) override;

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Device* m_D3D;
};

#endif