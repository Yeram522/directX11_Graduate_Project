////////////////////////////////////////////////////////////////////////////////
// Filename: skydomeshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SKYDOMESHADERCLASS_H_
#define _SKYDOMESHADERCLASS_H_

//////////////
// INCLUDES //
//////////////
#include "ShaderClass.h"

class SkyDomeShaderClass:public ShaderClass
{
private:
	struct GradientBufferType
	{
		XMFLOAT4 apexColor;
		XMFLOAT4 centerColor;
	};

public:
	SkyDomeShaderClass(LightClass* Light);
	~SkyDomeShaderClass();

	bool Initialize(ID3D11Device*, HWND)override;
	void Shutdown()override;
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView** textures = nullptr,
		XMFLOAT3 cameraPosition = XMFLOAT3(0, 0, 0))override;

private:
	bool InitializeShader(ID3D11Device*, HWND,const WCHAR*,const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, XMFLOAT4, XMFLOAT4);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_gradientBuffer;
};
#endif

