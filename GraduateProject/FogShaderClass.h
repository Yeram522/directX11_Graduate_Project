////////////////////////////////////////////////////////////////////////////////
// Filename: FogShaderClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FOGSHADERCLASS_H_
#define _FOGSHADERCLASS_H_

#include "ShaderClass.h"

class FogShaderClass : public ShaderClass
{
private:
    struct ConstantBufferType
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };

    struct FogBufferType
    {
        float fogStart;
        float fogEnd;
        float padding1, padding2;
    };

public:
    FogShaderClass(LightClass* Light);
    ~FogShaderClass();

    bool Initialize(ID3D11Device*, HWND) override;
    void Shutdown() override;
    bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView**,
        XMFLOAT3 cameraPosition = XMFLOAT3(0, 0, 0)) override;

    void setFogPosition(float fogstart, float fogend)
    {
        m_fogStart = fogstart;
        m_fogEnd = fogend;
    }
private:
    bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

    bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, float, float);
    void RenderShader(ID3D11DeviceContext*, int);

private:
    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_layout;
    ID3D11Buffer* m_constantBuffer;
    ID3D11SamplerState* m_sampleState;
    ID3D11Buffer* m_fogBuffer;

    float m_fogStart;
    float m_fogEnd;
};

#endif

