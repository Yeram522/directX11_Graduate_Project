////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
SceneManager* SceneManager::Instance = nullptr;
EngineManager* EngineManager::Instance = nullptr;
ShaderManagerClass* ShaderManagerClass::Instance = nullptr;


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Light = 0;
	m_Camera = 0;
	m_RenderTexture = 0;
	m_DebugWindow = 0;
	m_TextureShader = 0;

	// Create the SceneManager object.
	m_SceneManager = SceneManager::GetInstance();
	if (!m_SceneManager)
	{
		return;
	}

	m_EngineManager = EngineManager::GetInstance();
	if (!m_EngineManager)
	{
		return;
	}

	m_ShaderManager = ShaderManagerClass::GetInstance();
	if (!m_ShaderManager)
	{
		return;
	}


	m_image = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	XMMATRIX baseViewMatrix;
	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}
	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);


	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}


	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	//	m_Light->SetAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	//	m_Light->SetDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
	//	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	//	m_Light->SetDirection(1.0f, 0.0f, 0.0f);
	m_Light->SetDirection(1.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	//set shaderManager
	result = m_ShaderManager->Initialize(m_D3D->GetDevice(), hwnd,m_Light);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the ShaderManager object.", L"Error", MB_OK);
		return false;
	}


	result = m_SceneManager->SceneManager::Initialize(screenWidth, screenHeight, m_D3D, m_Camera, hwnd, m_Light, m_ShaderManager);
	if (!result)
	{
		return false;
	}


	// RTT ��ü�� �����մϴ�.
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}



	// RTT ��ü�� �ʱ�ȭ�մϴ�.
	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	// ����� �����츦 �����մϴ�.
	m_DebugWindow = new DebugWindowClass;
	if (!m_DebugWindow)
	{
		return false;
	}

	// ����� �����츦 �ʱ�ȭ�մϴ�.
	result = m_DebugWindow->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, 100, 100);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the debug window object.", L"Error", MB_OK);
		return false;
	}


	//Setup ImGui
	result = m_EngineManager->Initialize(m_D3D, hwnd,this);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the EngineManager object.", L"Error", MB_OK);
		return false;
	}

	
	return true;
}


void GraphicsClass::Shutdown()
{
	if (m_SceneManager)
	{
		//m_Scene->Shutdown();
		delete m_SceneManager;
		m_SceneManager = 0;
	}

	// ����� �����츦 �����մϴ�.
	if (m_DebugWindow)
	{
		m_DebugWindow->Shutdown();
		delete m_DebugWindow;
		m_DebugWindow = 0;
	}

	// RTT ��ü�� �����մϴ�.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	return;
}

bool GraphicsClass::Frame(InputClass* input, int fps, int cpu, float frameTime)
{
	bool result;

	//Key Input
	DIMOUSESTATE mouseCurrState;

	BYTE keyboardState[256];

	input->DIKeyboard->Acquire();
	input->DIMouse->Acquire();

	input->DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	input->DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	if (keyboardState[DIK_ESCAPE] & 0x80)
		PostMessage(input->m_hwnd, WM_DESTROY, 0, 0);

	float speed = 0.5f;

	if (keyboardState[DIK_A] & 0x80)
	{
		m_Camera->moveLeftRight -= speed;
	}
	if (keyboardState[DIK_D] & 0x80)
	{
		m_Camera->moveLeftRight += speed;
	}
	if (keyboardState[DIK_W] & 0x80)
	{
		m_Camera->moveBackForward += speed;
	}
	if (keyboardState[DIK_S] & 0x80)
	{
		m_Camera->moveBackForward -= speed;
	}
	if (keyboardState[DIK_Q] & 0x80)
	{
		m_Camera->moveUpDown += speed;
	}
	if (keyboardState[DIK_E] & 0x80)
	{
		m_Camera->moveUpDown -= speed;
	}
	if (((mouseCurrState.lX != input->mouseLastState.lX) || (mouseCurrState.lY != input->mouseLastState.lY))&& keyboardState[DIK_LCONTROL])
	{
		m_Camera->camYaw += input->mouseLastState.lX * 0.001f;

		m_Camera->camPitch += mouseCurrState.lY * 0.001f;

		input->mouseLastState = mouseCurrState;
	}


	//FPS CPU���� ���ε�
	m_SceneManager->SceneManager::SetFpsCpuInfo(fps,cpu);
	m_SceneManager->SceneManager::SetFrameTime(frameTime);

	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}
	
	return true;
}

bool GraphicsClass::Render()
{
	bool result;

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);



//	m_SceneManager->SceneManager::SetShader(); //�ݻ� ���̴� ����.
	// Render the refraction of the scene to a texture.
	result = m_ShaderManager->RenderRefractionToTexture(m_SceneManager->getActiveScene()->refractionModel, m_D3D, m_Camera, m_Light);
	if (!result)
	{
		return false;
	}

	// Render the reflection of the scene to a texture.
	result = m_ShaderManager->RenderReflectionToTexture(m_SceneManager->getActiveScene()->reflectionModel, m_D3D, m_Camera, m_Light);
	if (!result)
	{
		return false;
	}
	m_ShaderManager->updateWaterTranslate(m_Camera->GetReflectionViewMatrix());


	float fogColor = 0.5;
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(fogColor, fogColor, fogColor, 1.0f);

	m_Camera->Render();



	// ��ü ���� �ؽ��Ŀ� �׸��ϴ�.
	result = RenderToTexture();
	if (!result)
	{
		return false;
	}

	m_EngineManager->initImGui();

	m_EngineManager->updateImGui();


	ImGui::Begin("Hierachy");
	m_SceneManager->SceneManager::UpdateHierachy();
	ImGui::End();

	

	m_EngineManager->renderImGui();



	m_D3D->EndScene();
	return true;
}

bool GraphicsClass::RenderToTexture()
{
	bool result;


	// RTT�� ������ Ÿ���� �ǵ��� �մϴ�.
	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());
	// RTT�� �ʱ�ȭ�մϴ�.
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	// ���⼭ ���� �׸��� ����� ��� RTT�� �������˴ϴ�.
	m_SceneManager->SceneManager::UpdateScene();


	// ������ Ÿ���� RTT���� �ٽ� ����۷� �����ϴ�.
	m_D3D->SetBackBufferRenderTarget();

	return true;
}


