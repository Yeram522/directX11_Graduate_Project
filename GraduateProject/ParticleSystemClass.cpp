#include "ParticleSystemClass.h"

ParticleSystemClass::ParticleSystemClass(GameObject* gameObject):Component(gameObject)
{
    m_ParticleTexture = 0;
    m_particleList = 0;
    m_vertices = 0;
    m_vertexBuffer = 0;
    m_indexBuffer = 0;
    m_Shader = 0;
}

ParticleSystemClass::~ParticleSystemClass()
{
}

bool ParticleSystemClass::Initialize(ID3D11Device* device,const WCHAR* textureFilename, ParticleShaderClass* shader)
{
    bool result;


    // 파티클에 사용할 텍스쳐를 불러옵니다.
    result = LoadTexture(device, textureFilename);
    if (!result)
    {
        return false;
    }

    // 파티클 시스템을 초기화합니다.
    result = InitializeParticleSystem();
    if (!result)
    {
        return false;
    }

    // 파티클을 그리는 데 사용할 버퍼들을 생성합니다.
    result = InitializeBuffers(device);
    if (!result)
    {
        return false;
    }

    this->m_Shader = shader;
    return true;
}

void ParticleSystemClass::Shutdown()
{
    // 버퍼를 해제합니다.
    ShutdownBuffers();

    // 파티클 시스템을 해제합니다.
    ShutdownParticleSystem();

    // 파티클에 사용한 텍스쳐를 해제합니다.
    ReleaseTexture();

    return;
}

bool ParticleSystemClass::Frame(float frameTime)
{
    bool result;
    Transform* transform = Component::transform;

    // 오래된 파티클들을 해제합니다.
    KillParticles();

    // 새 파티클을 방출합니다.
    EmitParticles(frameTime);

    // 파티클들의 위치를 갱신합니다.
    UpdateParticles(frameTime);

    // 동적 정점 버퍼에 각 파티클의 갱신된 위치를 반영합니다.
    result = UpdateBuffers(transform->m_D3D->GetDeviceContext());
    if (!result)
    {
        return false;
    }

    return true;
}

void ParticleSystemClass::Render(ID3D11DeviceContext* deviceContext)
{

    // 정점 및 인덱스 버퍼를 그래픽 파이프라인에 넣어 그릴 준비를 합니다.
    RenderBuffers(deviceContext);

    return;
}

void ParticleSystemClass::update()
{

    transform->m_D3D->EnableSecondBlendState();
    Transform* transform = Component::transform;
    Render(transform->m_D3D->GetDeviceContext());

    m_Shader->Render(transform->m_D3D->GetDeviceContext(), GetIndexCount(), transform->m_worldMatrix, transform->m_viewMatrix, transform->m_projectionMatrix, GetTextureArray());
    transform->m_D3D->DisableSecondBlendState();
}


ID3D11ShaderResourceView** ParticleSystemClass::GetTextureArray()
{
    return m_ParticleTexture->GetTextureArray();
}

int ParticleSystemClass::GetIndexCount()
{
    return m_indexCount;
}

bool ParticleSystemClass::LoadTexture(ID3D11Device* device,const WCHAR* filename)
{
    bool result;


    // 텍스쳐 객체를 생성합니다.
    m_ParticleTexture = new TextureArrayClass;
    if (!m_ParticleTexture)
    {
        return false;
    }

    // 텍스쳐 객체를 초기화합니다.
    result = m_ParticleTexture->Initialize(device, filename, filename);
    if (!result)
    {
        return false;
    }

    return true;
}

void ParticleSystemClass::ReleaseTexture()
{
    // 텍스쳐 객체를 해제합니다.
    if (m_ParticleTexture)
    {
        m_ParticleTexture->Shutdown();
        delete m_ParticleTexture;
        m_ParticleTexture = 0;
    }

    return;
}

bool ParticleSystemClass::InitializeParticleSystem()
{
    int i;
    // 어디에 파티클이 생겨날지 임의의 분산값을 설정합니다.
    m_particleDeviationX = 0.5f;
    m_particleDeviationY = 0.1f;
    m_particleDeviationZ = 2.0f;

    // 파티클의 속도 및 속도 편차를 정합니다.
    m_particleVelocity = 1.0f;
    m_particleVelocityVariation = 0.2f;

    // 파티클의 실제 크기입니다.
    m_particleSize = 0.2f;

    // 파티클이 초당 생겨날 갯수입니다.
    m_particlesPerSecond = 250.0f;

    // 파티클 시스템에게 허락된 파티클의 최대 개수입니다.
    m_maxParticles = 5000;
    // 파티클 배열을 생성합니다.
    m_particleList = new ParticleType[m_maxParticles];
    if (!m_particleList)
    {
        return false;
    }
    // 파티클 목록을 초기화합니다.
    for (i = 0; i < m_maxParticles; i++)
    {
        m_particleList[i].active = false;
    }
    // 아무 파티클도 방출하지 않았기 때문에 현재 파티클 개수를 0으로 초기화합니다.
    m_currentParticleCount = 0;

    // 초당 방출 비율을 위해 누적 시간을 초기화합니다.
    m_accumulatedTime = 0.0f;

    return true;
}

void ParticleSystemClass::ShutdownParticleSystem()
{
    // 파티클 배열을 해제합니다.
    if (m_particleList)
    {
        delete[] m_particleList;
        m_particleList = 0;
    }

    return;
}

bool ParticleSystemClass::InitializeBuffers(ID3D11Device* device)
{
    unsigned long* indices;
    int i;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;


    // 정점 배열의 최대 정점 개수입니다.
    m_vertexCount = m_maxParticles * 6;

    // 인덱스 배열의 최대 인덱스 개수입니다.
    m_indexCount = m_vertexCount;

    // 그려지는 파티클들을 위한 정점 배열을 생성합니다.
    m_vertices = new VertexType[m_vertexCount];
    if (!m_vertices)
    {
        return false;
    }

    // 인덱스 배열을 생성합니다.
    indices = new unsigned long[m_indexCount];
    if (!indices)
    {
        return false;
    }

    // 정점 배열을 0으로 초기화합니다.
    memset(m_vertices, 0, (sizeof(VertexType) * m_vertexCount));

    // 인덱스 배열을 초기화합니다.
    for (i = 0; i < m_indexCount; i++)
    {
        indices[i] = i;
    }

    // 동적 정점 버퍼의 디스크립션을 설정합니다.
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // 서브 리소스 구조체에 정점 데이터의 포인터를 할당합니다.
    vertexData.pSysMem = m_vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // 마지막으로 정점 버퍼를 생성합니다.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // 정적 인덱스 버퍼의 디스크립션을 설정합니다.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // 서브 리소스 구조체에 인덱스 데이터의 포인터를 할당합니다.
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // 인덱스 버퍼를 생성합니다.
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // 더 이상 사용하지 않는 인덱스 배열을 해제합니다.
    delete[] indices;
    indices = 0;

    return true;
}

void ParticleSystemClass::ShutdownBuffers()
{
    // 인덱스 버퍼를 해제합니다.
    if (m_indexBuffer)
    {
        m_indexBuffer->Release();
        m_indexBuffer = 0;
    }

    // 정점 버퍼를 해제합니다.
    if (m_vertexBuffer)
    {
        m_vertexBuffer->Release();
        m_vertexBuffer = 0;
    }

    return;
}

void ParticleSystemClass::EmitParticles(float frameTime)
{
    bool emitParticle, found;
    float positionX, positionY, positionZ, velocity, red, green, blue;
    int index, i, j;


    // 프레임 시간을 더합니다.
    m_accumulatedTime += frameTime;

    // 파티클 방출 여부를 false로 합니다.
    emitParticle = false;

    // 새 파티클을 낼 시간인지 확인합니다.
    if (m_accumulatedTime > (1000.0f / m_particlesPerSecond))
    {
        m_accumulatedTime = 0.0f;
        emitParticle = true;
    }

    // 파티클이 나와야 한다면 프레임마다 하나씩 방출합니다.
    if ((emitParticle == true) && (m_currentParticleCount < (m_maxParticles - 1)))
    {
        m_currentParticleCount++;

        // 임의로 파티클 속성을 생성합니다.
        positionX = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationX;
        positionY = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationY;
        positionZ = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationZ;

        velocity = m_particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * m_particleVelocityVariation;

        red = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
        green = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
        blue = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;

        // 블렌딩하기 위해서는 파티클들을 뒤에서부터 앞으로 그려야 하기 때문에 배열을 정렬합니다.
        // Z깊이값으로 정렬할 것이기 때문에 파티클이 어느 지점에 삽입되어야 할 지 찾아야 합니다.
        index = 0;
        found = false;
        while (!found)
        {
            if ((m_particleList[index].active == false) || (m_particleList[index].positionZ < positionZ))
            {
                found = true;
            }
            else
            {
                index++;
            }
        }

        // 위치가 정해졌으므로 그 위치에서부터 배열을 하나씩 밀어 복사해서 새 파티클을 위한 공간을 만듭니다.
        i = m_currentParticleCount;
        j = i - 1;

        while (i != index)
        {
            m_particleList[i].positionX = m_particleList[j].positionX;
            m_particleList[i].positionY = m_particleList[j].positionY;
            m_particleList[i].positionZ = m_particleList[j].positionZ;
            m_particleList[i].red = m_particleList[j].red;
            m_particleList[i].green = m_particleList[j].green;
            m_particleList[i].blue = m_particleList[j].blue;
            m_particleList[i].velocity = m_particleList[j].velocity;
            m_particleList[i].active = m_particleList[j].active;
            i--;
            j--;
        }

        // 올바른 깊이 순서대로 파티클 배열에 추가합니다.
        m_particleList[index].positionX = positionX;
        m_particleList[index].positionY = positionY;
        m_particleList[index].positionZ = positionZ;
        m_particleList[index].red = red;
        m_particleList[index].green = green;
        m_particleList[index].blue = blue;
        m_particleList[index].velocity = velocity;
        m_particleList[index].active = true;
    }

    return;
}

void ParticleSystemClass::UpdateParticles(float frameTime)
{
    int i;


    // Each frame we update all the particles by making them move downwards using their position, velocity, and the frame time.
    for (i = 0; i < m_currentParticleCount; i++)
    {
        m_particleList[i].positionY = m_particleList[i].positionY - (m_particleList[i].velocity * frameTime * 0.001f);
    }

    return;
}

void ParticleSystemClass::KillParticles()
{
    int i, j;


    // 특정 높이 범위 아래로 내려간 파티클들을 전부 제거합니다.
    for (i = 0; i < m_maxParticles; i++)
    {
        if ((m_particleList[i].active == true) && (m_particleList[i].positionY < -3.0f))
        {
            m_particleList[i].active = false;
            m_currentParticleCount--;

            // 아직 살아 있는 모든 파티클들을 앞으로 당겨 정렬 순서는 유지하면서 사라진 파티클들은 지워지도록 합니다.
            for (j = i; j < m_maxParticles - 1; j++)
            {
                m_particleList[j].positionX = m_particleList[j + 1].positionX;
                m_particleList[j].positionY = m_particleList[j + 1].positionY;
                m_particleList[j].positionZ = m_particleList[j + 1].positionZ;
                m_particleList[j].red = m_particleList[j + 1].red;
                m_particleList[j].green = m_particleList[j + 1].green;
                m_particleList[j].blue = m_particleList[j + 1].blue;
                m_particleList[j].velocity = m_particleList[j + 1].velocity;
                m_particleList[j].active = m_particleList[j + 1].active;
            }
        }
    }

    return;
}

bool ParticleSystemClass::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
    int index, i;
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    VertexType* verticesPtr;

    // 정점 배열을 0으로 초기화합니다.
    memset(m_vertices, 0, (sizeof(VertexType) * m_vertexCount));

    // 파티클 목록 배열을 가지고 정점 배열을 만듭니다. 각 파티클은 두 개의 삼각형으로 이루어진 사각형입니다.
    index = 0;

    for (i = 0; i < m_currentParticleCount; i++)
    {
        // 왼쪽 아래
        m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
        m_vertices[index].texture = XMFLOAT2(0.0f, 1.0f);
        m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
        index++;

        // 왼쪽 위
        m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
        m_vertices[index].texture = XMFLOAT2(0.0f, 0.0f);
        m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
        index++;

        // 오른쪽 아래
        m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
        m_vertices[index].texture = XMFLOAT2(1.0f, 1.0f);
        m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
        index++;

        // 오른쪽 아래
        m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
        m_vertices[index].texture = XMFLOAT2(1.0f, 1.0f);
        m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
        index++;

        // 왼쪽 위
        m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
        m_vertices[index].texture = XMFLOAT2(0.0f, 0.0f);
        m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
        index++;

        // 오른쪽 위
        m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
        m_vertices[index].texture = XMFLOAT2(1.0f, 0.0f);
        m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
        index++;
    }

    // 정점 버퍼를 잠급니다.
    result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    // 정점 버퍼의 데이터 포인터를 가져옵니다.
    verticesPtr = (VertexType*)mappedResource.pData;

    // 정점 버퍼에 데이터를 복사합니다.
    memcpy(verticesPtr, (void*)m_vertices, (sizeof(VertexType) * m_vertexCount));

    // 정점 버퍼의 잠금을 해제합니다.
    deviceContext->Unmap(m_vertexBuffer, 0);

    return true;
}

void ParticleSystemClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
    unsigned int stride;
    unsigned int offset;


    // 정점 버퍼의 스트라이드와 오프셋입니다.
    stride = sizeof(VertexType);
    offset = 0;

    // 입력 어셈블러에 정점 버퍼를 활성화하여 그려질 수 있도록 합니다.
    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // 임력 어셈블러에 인덱스 버퍼를 활성화아여 그려질 수 있도록 합니다.
    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // 이 정점 버퍼에서 그릴 도형의 타입을 설정합니다.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return;
}
