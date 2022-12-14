#include "TransparentShaderClass.h"

TransparentShaderClass::TransparentShaderClass(LightClass* Light):ShaderClass(Light,"2D")
{
    m_vertexShader = 0;
    m_pixelShader = 0;
    m_layout = 0;
    m_matrixBuffer = 0;
    m_sampleState = 0;
    m_transparentBuffer = 0;
}

TransparentShaderClass::~TransparentShaderClass()
{
}

bool TransparentShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
    bool result;

    // ���� �� �ȼ� ���̴��� �ʱ�ȭ�մϴ�.
    result = InitializeShader(device, hwnd, L"./data/transparent.vs", L"./data/transparent.ps");
    if (!result)
    {
        return false;
    }

    return true;
}

void TransparentShaderClass::Shutdown()
{
    // ���� �� �ȼ� ���̴�, ���õ� ��ü���� �����մϴ�.
    ShutdownShader();

    return;
}

bool TransparentShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix,
    XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView** texture,
    XMFLOAT3 cameraPosition)
{

    bool result;


    // �������� ����� ���̴��� ���ڵ��� �����մϴ�.
    result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, 0.0f);
    if (!result)
    {
        return false;
    }

    // ���̴��� �̿��Ͽ� ���ۿ� ������ �׷����ϴ�.
    RenderShader(deviceContext, indexCount);

    return true;
}

bool TransparentShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename)
{
    HRESULT result;
    ID3D10Blob* errorMessage;
    ID3D10Blob* vertexShaderBuffer;
    ID3D10Blob* pixelShaderBuffer;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    unsigned int numElements;
    D3D11_BUFFER_DESC matrixBufferDesc;
    D3D11_SAMPLER_DESC samplerDesc;
    D3D11_BUFFER_DESC transparentBufferDesc;


    // �� �Լ����� ����� �����͵��� null�� �ʱ�ȭ�մϴ�.
    errorMessage = 0;
    vertexShaderBuffer = 0;
    pixelShaderBuffer = 0;

    // ���� ���̴��� �������մϴ�.
    result = D3DCompileFromFile(vsFilename, NULL, NULL, "TransparentVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
        0, &vertexShaderBuffer,&errorMessage);
    if (FAILED(result))
    {
        // If the shader failed to compile it should have writen something to the error message.
        if (errorMessage)
        {
            OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
        }
        // If there was  nothing in the error message then it simply could not find the shader file itself.
        else
        {
            MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
        }

        return false;
    }
    // �ȼ� ���̴��� �������մϴ�.
    result = D3DCompileFromFile(psFilename, NULL, NULL, "TransparentPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
        0,  &pixelShaderBuffer, &errorMessage);
    if (FAILED(result))
    {
        // If the shader failed to compile it should have writen something to the error message.
        if (errorMessage)
        {
            OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
        }
        // If there was  nothing in the error message then it simply could not find the file itself.
        else
        {
            MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
        }

        return false;
    }

    // ���ۿ��� ���� ���̴��� �����մϴ�.
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL,
        &m_vertexShader);
    if (FAILED(result))
    {
        return false;
    }

    // ���ۿ��� �ȼ� ���̴��� �����մϴ�.
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL,
        &m_pixelShader);
    if (FAILED(result))
    {
        return false;
    }

    // ���� �Է� ���̾ƿ� description�� �ۼ��մϴ�.
    // ModelClass �� ���̴��� �ִ� VertexType�� ������ ��ġ�ؾ� �մϴ�.
    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[1].SemanticName = "TEXCOORD";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;

    // ���̾ƿ��� �ִ� ���ҵ��� ������ ���մϴ�.
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    // ���� �Է� ���̾ƿ��� �����մϴ�.
    result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(), &m_layout);
    if (FAILED(result))
    {
        return false;
    }

    // �� �̻� ������� �ʴ� ���� �� �ȼ� ���̴��� ���۸� �����մϴ�.
    vertexShaderBuffer->Release();
    vertexShaderBuffer = 0;

    pixelShaderBuffer->Release();
    pixelShaderBuffer = 0;

    // ���� ���̴��� �ִ� ���� ��� ������ description�� �ۼ��մϴ�.
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    // ��� ������ �����͸� �����Ͽ� ���� ���̴��� ��� ���ۿ� ������ �� �ֵ��� �մϴ�.
    result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // �ؽ��� ���÷��� description�� �ۼ��մϴ�.
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // �ؽ��� ���÷��� �����մϴ�.
    result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
    if (FAILED(result))
    {
        return false;
    }

    // �ȼ� ���̴��� ���Ե� ���� ���� ��� ������ description�� �ۼ��մϴ�.
    transparentBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    transparentBufferDesc.ByteWidth = sizeof(TransparentBufferType);
    transparentBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    transparentBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    transparentBufferDesc.MiscFlags = 0;
    transparentBufferDesc.StructureByteStride = 0;

    // ���� ��� ������ �����͸� �����Ͽ� �ȼ� ���̴��� ��� ���ۿ� ������ �� �ְ� �մϴ�.
    result = device->CreateBuffer(&transparentBufferDesc, NULL, &m_transparentBuffer);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

void TransparentShaderClass::ShutdownShader()
{
    // ���� ��� ���۸� �����մϴ�.
    if (m_transparentBuffer)
    {
        m_transparentBuffer->Release();
        m_transparentBuffer = 0;
    }

    // ���÷��� �����մϴ�.
    if (m_sampleState)
    {
        m_sampleState->Release();
        m_sampleState = 0;
    }

    // ��� ��� ���۸� �����մϴ�.
    if (m_matrixBuffer)
    {
        m_matrixBuffer->Release();
        m_matrixBuffer = 0;
    }

    // ���̾ƿ��� �����մϴ�.
    if (m_layout)
    {
        m_layout->Release();
        m_layout = 0;
    }

    // �ȼ� ���̴��� �����մϴ�.
    if (m_pixelShader)
    {
        m_pixelShader->Release();
        m_pixelShader = 0;
    }

    // ���� ���̴��� �����մϴ�.
    if (m_vertexShader)
    {
        m_vertexShader->Release();
        m_vertexShader = 0;
    }

    return;
}

void TransparentShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd,
    const WCHAR* shaderFilename)
{
    char* compileErrors;
    unsigned long bufferSize, i;
    ofstream fout;


    // Get a pointer to the error message text buffer.
    compileErrors = (char*)(errorMessage->GetBufferPointer());

    // Get the length of the message.
    bufferSize = errorMessage->GetBufferSize();

    // Open a file to write the error message to.
    fout.open("shader-error.txt");


    // Pop a message up on the screen to notify the user to check the text file for compile errors.
    MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

    return;
}

bool TransparentShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
    XMMATRIX projectionMatrix, ID3D11ShaderResourceView** texture, float blend)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType* dataPtr;
    unsigned int bufferNumber;
    TransparentBufferType* dataPtr2;


    // ���̴����� �� �� �ֵ��� ����� ��ȯ���� �غ��մϴ�.
    worldMatrix = XMMatrixTranspose(worldMatrix);
    viewMatrix = XMMatrixTranspose(viewMatrix);
    projectionMatrix = XMMatrixTranspose(projectionMatrix);

    // ��� ���ۿ� ���� ���Ͽ� ���� ���� �̴ϴ�.
    result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    // ��� ��� ������ �����͸� ���ɴϴ�.
    dataPtr = (MatrixBufferType*)mappedResource.pData;

    // ��� ��� ���ۿ� ����� �����մϴ�.
    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projectionMatrix;

    // ���ۿ� �ɾ��� ���� �����մϴ�.
    deviceContext->Unmap(m_matrixBuffer, 0);

    // ���� ���̴������� ��� ��� ���� ��ġ�� ����ϴ�.
    bufferNumber = 0;

    // ���� ���̴��� ��� ��� ������ ���ŵ� ���� �ֽ��ϴ�.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, & m_matrixBuffer);

    // �ȼ� ���̴��� �ؽ��ĸ� �Ҵ��մϴ�.
    deviceContext->PSSetShaderResources(0, 2, texture);


    // ������ ����� �� �ֵ��� ���� ��� ���ۿ� ���� �̴ϴ�.
    result = deviceContext->Map(m_transparentBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    // ���� ��� ���� �������� �����͸� ���ɴϴ�.
    dataPtr2 = (TransparentBufferType*)mappedResource.pData;

    // ������ ���� ���� ��� ���ۿ� �ֽ��ϴ�.
    dataPtr2->blendAmount = blend;

    // ������ ���� �����մϴ�.
    deviceContext->Unmap(m_transparentBuffer, 0);

    // �ȼ� ���̴� �ȿ����� ���� ��� ������ ��ġ�Դϴ�.
    bufferNumber = 0;

    // ���� ��� ���۸� ���ŵ� ������ �����մϴ�.
    deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_transparentBuffer);
    return true;
}

void TransparentShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
    // ���� �Է� ���̾ƿ��� �����մϴ�.
    deviceContext->IASetInputLayout(m_layout);

    // �ﰢ���� �׸� ���� �� �ȼ� ���̴��� �غ��մϴ�.
    deviceContext->VSSetShader(m_vertexShader, NULL, 0);
    deviceContext->PSSetShader(m_pixelShader, NULL, 0);

    // �ȼ� ���̴��� ���÷��� �����մϴ�.
    deviceContext->PSSetSamplers(0, 1, &m_sampleState);

    // �ﰢ���� �׸��ϴ�.
    deviceContext->DrawIndexed(indexCount, 0, 0);

    return;
}
