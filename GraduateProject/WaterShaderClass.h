////////////////////////////////////////////////////////////////////////////////
// Filename: watershaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _WATERSHADERCLASS_H_
#define _WATERSHADERCLASS_H_

#include "ShaderClass.h"

class WaterShaderClass: public ShaderClass
{
private:
    struct MatrixBufferType
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };

    struct ReflectionBufferType
    {
        XMMATRIX reflection;
    };

    struct WaterBufferType
    {
        float waterTranslation;
        float reflectRefractScale;
        XMFLOAT2 padding;
    };

public:
    WaterShaderClass(LightClass* Light);
    ~WaterShaderClass();

    bool Initialize(ID3D11Device*, HWND)override;
    void Shutdown()override;
    bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView**, XMFLOAT3 cameraPosition = XMFLOAT3(0, 0, 0))override;

    void SetReflectionMatrix(XMMATRIX reflectionMatrix)
    {
        m_reflectionMatrix = reflectionMatrix;
    }
    void SetWaterTranslation(float waterTranslation)
    {
        m_waterTranslation = waterTranslation;
    }

    void SetReflectRefractScale(float reflectRefractScale)
    {
        m_reflectRefractScale = reflectRefractScale;
    }
private:
    bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

    bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*,
        ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, float, float);
    void RenderShader(ID3D11DeviceContext*, int);

private:
    float m_waterTranslation;
    float m_reflectRefractScale = 0.01f;
    XMMATRIX m_reflectionMatrix;
    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_layout;
    ID3D11SamplerState* m_sampleState;
    ID3D11Buffer* m_matrixBuffer;

    ID3D11Buffer* m_reflectionBuffer;
    ID3D11Buffer* m_waterBuffer;
};

#endif

