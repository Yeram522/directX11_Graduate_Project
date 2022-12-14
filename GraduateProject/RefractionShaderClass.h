////////////////////////////////////////////////////////////////////////////////
// Filename: refractionshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _REFRACTIONSHADERCLASS_H_
#define _REFRACTIONSHADERCLASS_H_

#include "ShaderClass.h"

class RefractionShaderClass :public ShaderClass
{
private:
    struct MatrixBufferType
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };

    struct LightBufferType
    {
        XMFLOAT4 ambientColor;
        XMFLOAT4 diffuseColor;
        XMFLOAT3 lightDirection;
        float padding;
    };

    struct ClipPlaneBufferType
    {
        XMFLOAT4 clipPlane;
    };

public:
    RefractionShaderClass(LightClass* Light);
    ~RefractionShaderClass();

    bool Initialize(ID3D11Device*, HWND);
    void Shutdown();
    bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView**, XMFLOAT3 cameraPosition = XMFLOAT3(0, 0, 0));

    void SetClipPlane(XMFLOAT4 clipPlane)
    {
        m_clipPlane = clipPlane;
    }
private:
    bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

    bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3,
        XMFLOAT4, XMFLOAT4, XMFLOAT4);
    void RenderShader(ID3D11DeviceContext*, int);

private:
    XMFLOAT4 m_clipPlane;
    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_layout;
    ID3D11SamplerState* m_sampleState;
    ID3D11Buffer* m_matrixBuffer;
    ID3D11Buffer* m_lightBuffer;
    ID3D11Buffer* m_clipPlaneBuffer;
};
#endif

