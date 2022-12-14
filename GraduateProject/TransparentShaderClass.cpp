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

    // 정점 및 픽셀 셰이더를 초기화합니다.
    result = InitializeShader(device, hwnd, L"./data/transparent.vs", L"./data/transparent.ps");
    if (!result)
    {
        return false;
    }

    return true;
}

void TransparentShaderClass::Shutdown()
{
    // 정점 및 픽셀 셰이더, 관련된 객체들을 해제합니다.
    ShutdownShader();

    return;
}

bool TransparentShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix,
    XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView** texture,
    XMFLOAT3 cameraPosition)
{

    bool result;


    // 렌더링에 사용할 셰이더의 인자들을 설정합니다.
    result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, 0.0f);
    if (!result)
    {
        return false;
    }

    // 셰이더를 이용하여 버퍼에 내용을 그려냅니다.
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


    // 이 함수에서 사용할 포인터들을 null로 초기화합니다.
    errorMessage = 0;
    vertexShaderBuffer = 0;
    pixelShaderBuffer = 0;

    // 정점 셰이더를 컴파일합니다.
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
    // 픽셀 셰이더를 컴파일합니다.
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

    // 버퍼에서 정점 셰이더를 생성합니다.
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL,
        &m_vertexShader);
    if (FAILED(result))
    {
        return false;
    }

    // 버퍼에서 픽셀 셰이더를 생성합니다.
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL,
        &m_pixelShader);
    if (FAILED(result))
    {
        return false;
    }

    // 정점 입력 레이아웃 description을 작성합니다.
    // ModelClass 및 셰이더에 있는 VertexType과 형식이 일치해야 합니다.
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

    // 레이아웃에 있는 원소들의 개수를 구합니다.
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

    // 정점 셰이더에 있는 동적 상수 버퍼의 description을 작성합니다.
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    // 상수 버퍼의 포인터를 생성하여 정점 셰이더의 상수 버퍼에 접근할 수 있도록 합니다.
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

    // 픽셀 셰이더에 포함된 투명도 동적 상수 버퍼의 description을 작성합니다.
    transparentBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    transparentBufferDesc.ByteWidth = sizeof(TransparentBufferType);
    transparentBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    transparentBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    transparentBufferDesc.MiscFlags = 0;
    transparentBufferDesc.StructureByteStride = 0;

    // 동적 상수 버퍼의 포인터를 생성하여 픽셀 셰이더의 상수 버퍼에 접근할 수 있게 합니다.
    result = device->CreateBuffer(&transparentBufferDesc, NULL, &m_transparentBuffer);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

void TransparentShaderClass::ShutdownShader()
{
    // 투명도 상수 버퍼를 해제합니다.
    if (m_transparentBuffer)
    {
        m_transparentBuffer->Release();
        m_transparentBuffer = 0;
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


    // 셰이더에서 쓸 수 있도록 행렬을 변환시켜 준비합니다.
    worldMatrix = XMMatrixTranspose(worldMatrix);
    viewMatrix = XMMatrixTranspose(viewMatrix);
    projectionMatrix = XMMatrixTranspose(projectionMatrix);

    // 상수 버퍼에 쓰기 위하여 먼저 락을 겁니다.
    result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    // 행렬 상수 버퍼의 포인터를 얻어옵니다.
    dataPtr = (MatrixBufferType*)mappedResource.pData;

    // 행렬 상수 버퍼에 행렬을 복사합니다.
    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projectionMatrix;

    // 버퍼에 걸었던 락을 해제합니다.
    deviceContext->Unmap(m_matrixBuffer, 0);

    // 정점 셰이더에서의 행렬 상수 버퍼 위치를 잡습니다.
    bufferNumber = 0;

    // 정점 셰이더에 행렬 상수 버퍼의 갱신된 값을 넣습니다.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, & m_matrixBuffer);

    // 픽셀 셰이더에 텍스쳐를 할당합니다.
    deviceContext->PSSetShaderResources(0, 2, texture);


    // 내용을 기록할 수 있도록 투명도 상수 버퍼에 락을 겁니다.
    result = deviceContext->Map(m_transparentBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    // 투명도 상수 버퍼 데이터의 포인터를 얻어옵니다.
    dataPtr2 = (TransparentBufferType*)mappedResource.pData;

    // 블렌딩할 값을 투명도 상수 버퍼에 넣습니다.
    dataPtr2->blendAmount = blend;

    // 버퍼의 락을 해제합니다.
    deviceContext->Unmap(m_transparentBuffer, 0);

    // 픽셀 셰이더 안에서의 투명도 상수 버퍼의 위치입니다.
    bufferNumber = 0;

    // 투명도 상수 버퍼를 갱신된 값으로 설정합니다.
    deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_transparentBuffer);
    return true;
}

void TransparentShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
    // 정점 입력 레이아웃을 설정합니다.
    deviceContext->IASetInputLayout(m_layout);

    // 삼각형을 그릴 정점 및 픽셀 셰이더를 준비합니다.
    deviceContext->VSSetShader(m_vertexShader, NULL, 0);
    deviceContext->PSSetShader(m_pixelShader, NULL, 0);

    // 픽셀 셰이더의 샘플러를 설정합니다.
    deviceContext->PSSetSamplers(0, 1, &m_sampleState);

    // 삼각형을 그립니다.
    deviceContext->DrawIndexed(indexCount, 0, 0);

    return;
}
