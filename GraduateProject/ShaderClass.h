////////////////////////////////////////////////////////////////////////////////
// Filename: shaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SHADERCLASS_H_
#define _SHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>
#include "lightclass.h"
#include <fstream>

using namespace std;
using namespace DirectX;

class ShaderClass
{
protected:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};
protected:
	LightClass* m_Light;
public:
	ShaderClass(LightClass*);
	~ShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND) { return true; }
	virtual void Shutdown() {}
	virtual bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView**,
		XMFLOAT3 lightDirection = XMFLOAT3(0, 0, 0), XMFLOAT4 ambientColor = XMFLOAT4(0, 0, 0, 0), XMFLOAT4 diffuseColor = XMFLOAT4(0, 0, 0, 0), XMFLOAT3 cameraPosition = XMFLOAT3(0, 0, 0), XMFLOAT4 specularColor = XMFLOAT4(0, 0, 0, 0), float specularPower = 0) {return true;}
};

#endif
