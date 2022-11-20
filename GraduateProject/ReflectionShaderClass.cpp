#include "ReflectionShaderClass.h"

ReflectionShaderClass::ReflectionShaderClass(LightClass* Light):ShaderClass(Light)
{
    m_vertexShader = 0;
    m_pixelShader = 0;
    m_layout = 0;
    m_matrixBuffer = 0;
    m_sampleState = 0;
}

ReflectionShaderClass::~ReflectionShaderClass()
{
}

bool ReflectionShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
    bool result;

    // 정점 및 픽셀 셰이더를 초기화합니다.
    result = InitializeShader(device, hwnd, L"./data/reflection.vs", L"./data/reflection.ps");
    if (!result)
    {
        return false;
    }

    return true;
}

void ReflectionShaderClass::Shutdown()
{
    // 정점 및 픽셀 셰이더, 그리고 관련된 객체들을 해제합니다.
    ShutdownShader();

    return;
}

bool ReflectionShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix,
    XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView** textures,XMFLOAT3 cameraPosition)
{
    bool result;


    // 렌더링에 사용할 셰이더 인자들을 설정합니다.
    result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, textures[0], textures[1],
        m_reflectionMatrix);
    if (!result)
    {
        return false;
    }

    // 셰이더를 이용하여 버퍼에 렌더링합니다.
    RenderShader(deviceContext, indexCount);

    return true;
}

bool ReflectionShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename, const WCHAR* psFilename)
{
    HRESULT result;
    ID3D10Blob* errorMessage;
    ID3D10Blob* vertexShaderBuffer;
    ID3D10Blob* pixelShaderBuffer;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    unsigned int numElements;
    D3D11_BUFFER_DESC matrixBufferDesc;
    D3D11_SAMPLER_DESC samplerDesc;
    D3D11_BUFFER_DESC reflectionBufferDesc;


    // 이 함수에서 사용할 포인터들을 null로 초기화합니다.
    errorMessage = 0;
    vertexShaderBuffer = 0;
    pixelShaderBuffer = 0;
    // 정점 셰이더를 컴파일합니다.
    result = D3DCompileFromFile(vsFilename, NULL, NULL, "ReflectionVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
        0, &vertexShaderBuffer, &errorMessage);
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

    // 픽셀 셰이더를 컴파일합니다.
    result = D3DCompileFromFile(psFilename, NULL, NULL, "ReflectionPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
        0, &pixelShaderBuffer, &errorMessage);
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

    // 버퍼에 정점 셰이더를 생성합니다.
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL,
        &m_vertexShader);
    if (FAILED(result))
    {
        return false;
    }

    // 버퍼에 픽셀 셰이더를 생성합니다.
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL,
        &m_pixelShader);
    if (FAILED(result))
    {
        return false;
    }

    // 정점 입력 레이아웃의 description을 작성합니다.
    // 이 레이아웃은 ModelClass와 셰이더에 있는 VertexType과 그 구조가 일치해야 합니다.
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

    // 레이아웃의 원소의 수를 구합니다.
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    // 정점 입력 레이아웃을 생성합니다.
    result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(), &m_layout);
    if (FAILED(result))
    {
        return false;
    }

    // 더 이상 사용하지 않는 정점 및 픽셀 셰이더의 버퍼를 해제합니다.
    vertexShaderBuffer->Release();
    vertexShaderBuffer = 0;

    pixelShaderBuffer->Release();
    pixelShaderBuffer = 0;

    // 정점 셰이더에 있는 행렬 동적 상수 버퍼를 설정합니다.
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    // 이 클래스에서 정점 셰이더의 상수 버퍼에 접근하기 위해 버퍼를 생성합니다.
    result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // 텍스쳐 샘플러의 description을 작성합니다.
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

    // 텍스쳐 샘플러를 생성합니다.
    result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
    if (FAILED(result))
    {
        return false;
    }

    // 정점 셰이더에 들어가는 반사 행렬의 동적 상수 버퍼에 대한 description을 작성합니다.
    reflectionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    reflectionBufferDesc.ByteWidth = sizeof(ReflectionBufferType);
    reflectionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    reflectionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    reflectionBufferDesc.MiscFlags = 0;
    reflectionBufferDesc.StructureByteStride = 0;

    // 버퍼를 생성하여 정점 셰이더의 상수 버퍼에 접근할 수 있게 합니다.
    result = device->CreateBuffer(&reflectionBufferDesc, NULL, &m_reflectionBuffer);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

void ReflectionShaderClass::ShutdownShader()
{
    // 반사 상수 버퍼를 해제합니다.
    if (m_reflectionBuffer)
    {
        m_reflectionBuffer->Release();
        m_reflectionBuffer = 0;
    }

    // 샘플러를 해제합니다.
    if (m_sampleState)
    {
        m_sampleState->Release();
        m_sampleState = 0;
    }

    // 행렬 상수 버퍼를 해제합니다.
    if (m_matrixBuffer)
    {
        m_matrixBuffer->Release();
        m_matrixBuffer = 0;
    }

    // 레이아웃을 해제합니다.
    if (m_layout)
    {
        m_layout->Release();
        m_layout = 0;
    }

    // 픽셀 셰이더를 해제합니다.
    if (m_pixelShader)
    {
        m_pixelShader->Release();
        m_pixelShader = 0;
    }

    // 정점 셰이더를 해제합니다.
    if (m_vertexShader)
    {
        m_vertexShader->Release();
        m_vertexShader = 0;
    }

    return;
}

void ReflectionShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd,const  WCHAR* shaderFilename)
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

    // Write out the error message.
    for (i = 0; i < bufferSize; i++)
    {
        fout << compileErrors[i];
    }

    // Close the file.
    fout.close();

    // Release the error message.
    errorMessage->Release();
    errorMessage = 0;

    // Pop a message up on the screen to notify the user to check the text file for compile errors.
    MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

    return;
}

bool ReflectionShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
    XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture,
    ID3D11ShaderResourceView* reflectionTexture, XMMATRIX reflectionMatrix)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType* dataPtr;
    unsigned int bufferNumber;
    ReflectionBufferType* dataPtr2;


    // 셰이더에 사용할 행렬을 준비합니다.
    worldMatrix = XMMatrixTranspose(worldMatrix);
    viewMatrix = XMMatrixTranspose(viewMatrix);
    projectionMatrix = XMMatrixTranspose(projectionMatrix);

    // 셰이더에 사용할 반사 행렬을 변환해 둡니다.
    reflectionMatrix = XMMatrixTranspose(reflectionMatrix);


    // 기록할 수 있도록 행렬 상수 버퍼를 잠급니다.
    result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    // 행렬 상수 버퍼의 데이터 포인터를 얻어옵니다.
    dataPtr = (MatrixBufferType*)mappedResource.pData;

    // 행렬들을 상수 버퍼에 복사합니다.
    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projectionMatrix;

    // 행렬 상수 버퍼의 잠금을 해제합니다.
    deviceContext->Unmap(m_matrixBuffer, 0);

    // 정점 셰이더상의 행렬 상수 버퍼의 위치입니다.
    bufferNumber = 0;

    // 갱신된 값으로 정점 셰이더의 행렬 상수 버퍼를 설정합니다.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

    // 기록할 수 있도록 반사 상수 버퍼를 잠급니다.
    result = deviceContext->Map(m_reflectionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    // 행렬 상수 버퍼의 데이터 포인터를 얻어옵니다.
    dataPtr2 = (ReflectionBufferType*)mappedResource.pData;

    // 행렬을 상수 버퍼에 복사합니다.
    dataPtr2->reflectionMatrix = reflectionMatrix;

    // 반사 상수 버퍼의 잠금을 해제합니다.
    deviceContext->Unmap(m_reflectionBuffer, 0);

    // 정점 셰이더상의 반사 상수 버퍼의 위치입니다.
    bufferNumber = 1;

    // 갱신된 값으로 정점 셰이더의 반사 행렬 상수 버퍼를 설정합니다.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_reflectionBuffer);

    // 픽셀 셰이더에 텍스쳐를 설정합니다.
    deviceContext->PSSetShaderResources(0, 1, &texture);

    // 픽셀 셰이더에 반사 텍스쳐를 설정합니다.
    deviceContext->PSSetShaderResources(1, 1, &reflectionTexture);

    return true;
}

void ReflectionShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
    // 정점 입력 레이아웃을 설정합니다.
    deviceContext->IASetInputLayout(m_layout);

    // 정점 및 픽셀 셰이더를 설정합니다.
    deviceContext->VSSetShader(m_vertexShader, NULL, 0);
    deviceContext->PSSetShader(m_pixelShader, NULL, 0);

    // 픽셀 셰이더의 샘플러를 설정합니다.
    deviceContext->PSSetSamplers(0, 1, &m_sampleState);

    // 모델을 그립니다.
    deviceContext->DrawIndexed(indexCount, 0, 0);

    return;
}
