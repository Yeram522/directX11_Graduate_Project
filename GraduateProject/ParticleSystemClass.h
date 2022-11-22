////////////////////////////////////////////////////////////////////////////////
// Filename: particlesystemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PARTICLESYSTEMCLASS_H_
#define _PARTICLESYSTEMCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include <d3d11.h>
#include <directxmath.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Component.h"
#include "textureclass.h"
#include "Transform.h"
#include "ShaderManagerClass.h"


using namespace std;
using namespace DirectX;


class ParticleSystemClass :public Component
{
private:
    struct ParticleType
    {
        float positionX, positionY, positionZ;
        float red, green, blue;
        float velocity;
        bool active;
    };

    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
        XMFLOAT4 color;
    };
public:
    ParticleSystemClass(GameObject* gameObject);
    ~ParticleSystemClass();

    bool Initialize(ID3D11Device*,const WCHAR*, ParticleShaderClass*);
    void Shutdown();
    bool Frame(float);
    void Render(ID3D11DeviceContext*);
    void update() override;
    ID3D11ShaderResourceView** GetTextureArray();
    int GetIndexCount();

private:
    bool LoadTexture(ID3D11Device*,const WCHAR*);
    void ReleaseTexture();

    bool InitializeParticleSystem();
    void ShutdownParticleSystem();

    bool InitializeBuffers(ID3D11Device*);
    void ShutdownBuffers();

    void EmitParticles(float);
    void UpdateParticles(float);
    void KillParticles();

    bool UpdateBuffers(ID3D11DeviceContext*);

    void RenderBuffers(ID3D11DeviceContext*);

private:
    float m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
    float m_particleVelocity, m_particleVelocityVariation;
    float m_particleSize, m_particlesPerSecond;
    int m_maxParticles;

    int m_currentParticleCount;
    float m_accumulatedTime;

    TextureArrayClass* m_ParticleTexture;
    ParticleType* m_particleList;

    int m_vertexCount, m_indexCount;
    VertexType* m_vertices;
    ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
    ParticleShaderClass* m_Shader;
};

#endif

