////////////////////////////////////////////////////////////////////////////////
// Filename: transparentshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TRANSPARENTSHADERCLASS_H_
#define _TRANSPARENTSHADERCLASS_H_

#include "ShaderClass.h"

class TransparentShaderClass: public ShaderClass
{
private:
    struct TransparentBufferType
    {
        float blendAmount;
        XMFLOAT3 padding;
    };

public:
    TransparentShaderClass(LightClass* Light);
    ~TransparentShaderClass();

    bool Initialize(ID3D11Device*, HWND) override;
    void Shutdown() override;
    bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView**, XMFLOAT3 cameraPosition = XMFLOAT3(0, 0, 0)) override;
private:
    bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*) ;
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

    bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView**, float);
    void RenderShader(ID3D11DeviceContext*, int);

private:
    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_layout;
    ID3D11Buffer* m_matrixBuffer;
    ID3D11SamplerState* m_sampleState;
    ID3D11Buffer* m_transparentBuffer;
};

#endif
