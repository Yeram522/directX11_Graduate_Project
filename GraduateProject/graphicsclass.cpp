////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
SceneManager* SceneManager::Instance = nullptr;


GraphicsClass::GraphicsClass()
{
	m_Scene = 0;
	m_D3D = 0;
	m_LightShader = 0;
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

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

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


	result = m_SceneManager->SceneManager::Initialize(screenWidth, screenHeight, m_D3D, m_Camera, hwnd, m_Light, m_LightShader);
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

	// �ؽ��� ���̴��� �����մϴ�.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// �ؽ��� ���̴��� �ʱ�ȭ�մϴ�.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
 //   //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
//   io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	ImGui::StyleColorsDark();


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

	if (m_Scene)
	{
		m_Scene->Shutdown();
		delete m_Scene;
		m_Scene = 0;
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

bool GraphicsClass::Frame()
{
	bool result;

	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;

	// ��ü ���� �ؽ��Ŀ� �׸��ϴ�.
	result = RenderToTexture();
	if (!result)
	{
		return false;
	}


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render();

	m_SceneManager->SceneManager::UpdateScene();


	// 2D �������� �ϱ� ���� Z���۸� ���ϴ�.
	m_D3D->TurnZBufferOff();

	// ī�޶�� d3d ��ü�κ��� ����, ��, ���� ����� ���ɴϴ�.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// ����� �������� ������ �ε��� ���۸� �׷��� ���������ο� �־� �������� �غ� �մϴ�.
	result = m_DebugWindow->Render(m_D3D->GetDeviceContext(), 50, 50);
	if (!result)
	{
		return false;
	}

	// �ؽ��� ���̴��� �̿��Ͽ� ����� �����츦 �׸��ϴ�.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), worldMatrix, viewMatrix,
		orthoMatrix, m_RenderTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}


	//// 2D�������� �������Ƿ� �ٽ� Z���۸� ŵ�ϴ�.
	m_D3D->TurnZBufferOn();

	//static int counter = 0;
	//Start theDear ImGui Frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	//Docking
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	//Create ImGui Test Window
	ImGui::Begin("Inspector");
	ImGui::Text("PhongShader");
	ImGui::SliderFloat3("LightDirection", m_Light->GetDirectiontoFloat(), -100.0f, 100.0f);
	ImGui::SliderFloat4("AmbidientColor", m_Light->GetAmbientColortoFloat(), 0.0f, 1.0f);
	ImGui::SliderFloat4("DiffuseColor", m_Light->GetDiffuseColortoFloat(), 0.0f, 1.0f);
	ImGui::SliderFloat4("SpecularColor", m_Light->GetSpecularColortoFloat(), 0.0f, 1.0f);
	ImGui::SliderFloat("Specular power", m_Light->GetSpecularPower(0), 0.0f, 100.0f);
	ImGui::End();


	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("ViewPort");
	{
		// Using a Child allow to fill all the space of the window.
		// It also alows customization
		ImGui::BeginChild("GameRender");
		// Get the size of the child (i.e. the whole draw size of the windows).
		ImVec2 wsize = ImGui::GetWindowSize();
		// Because I use the texture from OpenGL, I need to invert the V from the UV.
		ImGui::Image((ImTextureID)m_RenderTexture->GetShaderResourceView(), wsize);
		ImGui::EndChild();
	}
	ImGui::PopStyleVar();

	ImGui::End();


	ImGui::Begin("Directory");

	ImGui::End();

	//Assemble Together Draw Data
	ImGui::Render();
	//Render Draw Data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	m_D3D->EndScene();
	return true;
}

bool GraphicsClass::RenderToTexture()
{
	bool result;


	// RTT�� ������ Ÿ���� �ǵ��� �մϴ�.
	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());
	// RTT�� �ʱ�ȭ�մϴ�.
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	// ���⼭ ���� �׸��� ����� ��� RTT�� �������˴ϴ�.
	m_SceneManager->SceneManager::UpdateScene();


	// ������ Ÿ���� RTT���� �ٽ� ����۷� �����ϴ�.
	m_D3D->SetBackBufferRenderTarget();

	return true;
}


