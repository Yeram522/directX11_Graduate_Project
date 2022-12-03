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


    // ��ƼŬ�� ����� �ؽ��ĸ� �ҷ��ɴϴ�.
    result = LoadTexture(device, textureFilename);
    if (!result)
    {
        return false;
    }

    // ��ƼŬ �ý����� �ʱ�ȭ�մϴ�.
    result = InitializeParticleSystem();
    if (!result)
    {
        return false;
    }

    // ��ƼŬ�� �׸��� �� ����� ���۵��� �����մϴ�.
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
    // ���۸� �����մϴ�.
    ShutdownBuffers();

    // ��ƼŬ �ý����� �����մϴ�.
    ShutdownParticleSystem();

    // ��ƼŬ�� ����� �ؽ��ĸ� �����մϴ�.
    ReleaseTexture();

    return;
}

bool ParticleSystemClass::Frame(float frameTime)
{
    bool result;
    Transform* transform = Component::transform;

    // ������ ��ƼŬ���� �����մϴ�.
    KillParticles();

    // �� ��ƼŬ�� �����մϴ�.
    EmitParticles(frameTime);

    // ��ƼŬ���� ��ġ�� �����մϴ�.
    UpdateParticles(frameTime);

    // ���� ���� ���ۿ� �� ��ƼŬ�� ���ŵ� ��ġ�� �ݿ��մϴ�.
    result = UpdateBuffers(transform->m_D3D->GetDeviceContext());
    if (!result)
    {
        return false;
    }

    return true;
}

void ParticleSystemClass::Render(ID3D11DeviceContext* deviceContext)
{

    // ���� �� �ε��� ���۸� �׷��� ���������ο� �־� �׸� �غ� �մϴ�.
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


    // �ؽ��� ��ü�� �����մϴ�.
    m_ParticleTexture = new TextureArrayClass;
    if (!m_ParticleTexture)
    {
        return false;
    }

    // �ؽ��� ��ü�� �ʱ�ȭ�մϴ�.
    result = m_ParticleTexture->Initialize(device, filename, filename);
    if (!result)
    {
        return false;
    }

    return true;
}

void ParticleSystemClass::ReleaseTexture()
{
    // �ؽ��� ��ü�� �����մϴ�.
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
    // ��� ��ƼŬ�� ���ܳ��� ������ �л갪�� �����մϴ�.
    m_particleDeviationX = 0.5f;
    m_particleDeviationY = 0.1f;
    m_particleDeviationZ = 2.0f;

    // ��ƼŬ�� �ӵ� �� �ӵ� ������ ���մϴ�.
    m_particleVelocity = 1.0f;
    m_particleVelocityVariation = 0.2f;

    // ��ƼŬ�� ���� ũ���Դϴ�.
    m_particleSize = 0.2f;

    // ��ƼŬ�� �ʴ� ���ܳ� �����Դϴ�.
    m_particlesPerSecond = 250.0f;

    // ��ƼŬ �ý��ۿ��� ����� ��ƼŬ�� �ִ� �����Դϴ�.
    m_maxParticles = 5000;
    // ��ƼŬ �迭�� �����մϴ�.
    m_particleList = new ParticleType[m_maxParticles];
    if (!m_particleList)
    {
        return false;
    }
    // ��ƼŬ ����� �ʱ�ȭ�մϴ�.
    for (i = 0; i < m_maxParticles; i++)
    {
        m_particleList[i].active = false;
    }
    // �ƹ� ��ƼŬ�� �������� �ʾұ� ������ ���� ��ƼŬ ������ 0���� �ʱ�ȭ�մϴ�.
    m_currentParticleCount = 0;

    // �ʴ� ���� ������ ���� ���� �ð��� �ʱ�ȭ�մϴ�.
    m_accumulatedTime = 0.0f;

    return true;
}

void ParticleSystemClass::ShutdownParticleSystem()
{
    // ��ƼŬ �迭�� �����մϴ�.
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


    // ���� �迭�� �ִ� ���� �����Դϴ�.
    m_vertexCount = m_maxParticles * 6;

    // �ε��� �迭�� �ִ� �ε��� �����Դϴ�.
    m_indexCount = m_vertexCount;

    // �׷����� ��ƼŬ���� ���� ���� �迭�� �����մϴ�.
    m_vertices = new VertexType[m_vertexCount];
    if (!m_vertices)
    {
        return false;
    }

    // �ε��� �迭�� �����մϴ�.
    indices = new unsigned long[m_indexCount];
    if (!indices)
    {
        return false;
    }

    // ���� �迭�� 0���� �ʱ�ȭ�մϴ�.
    memset(m_vertices, 0, (sizeof(VertexType) * m_vertexCount));

    // �ε��� �迭�� �ʱ�ȭ�մϴ�.
    for (i = 0; i < m_indexCount; i++)
    {
        indices[i] = i;
    }

    // ���� ���� ������ ��ũ������ �����մϴ�.
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // ���� ���ҽ� ����ü�� ���� �������� �����͸� �Ҵ��մϴ�.
    vertexData.pSysMem = m_vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // ���������� ���� ���۸� �����մϴ�.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // ���� �ε��� ������ ��ũ������ �����մϴ�.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // ���� ���ҽ� ����ü�� �ε��� �������� �����͸� �Ҵ��մϴ�.
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // �ε��� ���۸� �����մϴ�.
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // �� �̻� ������� �ʴ� �ε��� �迭�� �����մϴ�.
    delete[] indices;
    indices = 0;

    return true;
}

void ParticleSystemClass::ShutdownBuffers()
{
    // �ε��� ���۸� �����մϴ�.
    if (m_indexBuffer)
    {
        m_indexBuffer->Release();
        m_indexBuffer = 0;
    }

    // ���� ���۸� �����մϴ�.
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


    // ������ �ð��� ���մϴ�.
    m_accumulatedTime += frameTime;

    // ��ƼŬ ���� ���θ� false�� �մϴ�.
    emitParticle = false;

    // �� ��ƼŬ�� �� �ð����� Ȯ���մϴ�.
    if (m_accumulatedTime > (1000.0f / m_particlesPerSecond))
    {
        m_accumulatedTime = 0.0f;
        emitParticle = true;
    }

    // ��ƼŬ�� ���;� �Ѵٸ� �����Ӹ��� �ϳ��� �����մϴ�.
    if ((emitParticle == true) && (m_currentParticleCount < (m_maxParticles - 1)))
    {
        m_currentParticleCount++;

        // ���Ƿ� ��ƼŬ �Ӽ��� �����մϴ�.
        positionX = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationX;
        positionY = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationY;
        positionZ = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationZ;

        velocity = m_particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * m_particleVelocityVariation;

        red = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
        green = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
        blue = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;

        // �����ϱ� ���ؼ��� ��ƼŬ���� �ڿ������� ������ �׷��� �ϱ� ������ �迭�� �����մϴ�.
        // Z���̰����� ������ ���̱� ������ ��ƼŬ�� ��� ������ ���ԵǾ�� �� �� ã�ƾ� �մϴ�.
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

        // ��ġ�� ���������Ƿ� �� ��ġ�������� �迭�� �ϳ��� �о� �����ؼ� �� ��ƼŬ�� ���� ������ ����ϴ�.
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

        // �ùٸ� ���� ������� ��ƼŬ �迭�� �߰��մϴ�.
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


    // Ư�� ���� ���� �Ʒ��� ������ ��ƼŬ���� ���� �����մϴ�.
    for (i = 0; i < m_maxParticles; i++)
    {
        if ((m_particleList[i].active == true) && (m_particleList[i].positionY < -3.0f))
        {
            m_particleList[i].active = false;
            m_currentParticleCount--;

            // ���� ��� �ִ� ��� ��ƼŬ���� ������ ��� ���� ������ �����ϸ鼭 ����� ��ƼŬ���� ���������� �մϴ�.
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

    // ���� �迭�� 0���� �ʱ�ȭ�մϴ�.
    memset(m_vertices, 0, (sizeof(VertexType) * m_vertexCount));

    // ��ƼŬ ��� �迭�� ������ ���� �迭�� ����ϴ�. �� ��ƼŬ�� �� ���� �ﰢ������ �̷���� �簢���Դϴ�.
    index = 0;

    for (i = 0; i < m_currentParticleCount; i++)
    {
        // ���� �Ʒ�
        m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
        m_vertices[index].texture = XMFLOAT2(0.0f, 1.0f);
        m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
        index++;

        // ���� ��
        m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
        m_vertices[index].texture = XMFLOAT2(0.0f, 0.0f);
        m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
        index++;

        // ������ �Ʒ�
        m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
        m_vertices[index].texture = XMFLOAT2(1.0f, 1.0f);
        m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
        index++;

        // ������ �Ʒ�
        m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY - m_particleSize, m_particleList[i].positionZ);
        m_vertices[index].texture = XMFLOAT2(1.0f, 1.0f);
        m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
        index++;

        // ���� ��
        m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX - m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
        m_vertices[index].texture = XMFLOAT2(0.0f, 0.0f);
        m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
        index++;

        // ������ ��
        m_vertices[index].position = XMFLOAT3(m_particleList[i].positionX + m_particleSize, m_particleList[i].positionY + m_particleSize, m_particleList[i].positionZ);
        m_vertices[index].texture = XMFLOAT2(1.0f, 0.0f);
        m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
        index++;
    }

    // ���� ���۸� ��޴ϴ�.
    result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    // ���� ������ ������ �����͸� �����ɴϴ�.
    verticesPtr = (VertexType*)mappedResource.pData;

    // ���� ���ۿ� �����͸� �����մϴ�.
    memcpy(verticesPtr, (void*)m_vertices, (sizeof(VertexType) * m_vertexCount));

    // ���� ������ ����� �����մϴ�.
    deviceContext->Unmap(m_vertexBuffer, 0);

    return true;
}

void ParticleSystemClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
    unsigned int stride;
    unsigned int offset;


    // ���� ������ ��Ʈ���̵�� �������Դϴ�.
    stride = sizeof(VertexType);
    offset = 0;

    // �Է� ������� ���� ���۸� Ȱ��ȭ�Ͽ� �׷��� �� �ֵ��� �մϴ�.
    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // �ӷ� ������� �ε��� ���۸� Ȱ��ȭ�ƿ� �׷��� �� �ֵ��� �մϴ�.
    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // �� ���� ���ۿ��� �׸� ������ Ÿ���� �����մϴ�.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return;
}
