////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_GameObject = 0;
	m_LightShader = 0;
	m_Light = 0;

	m_RenderTexture = 0;
	m_DebugWindow = 0;
	m_TextureShader = 0;
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
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);	// for cube
		
	// Create the model object.
	m_GameObject = new GameObject();
	if(!m_GameObject)
	{
		return false;
	}

	// Initialize the model object.
	result = m_GameObject->Initialize(m_D3D, m_Camera,L"./data/Sphere.obj", L"./data/seafloor.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the game object.", L"Error", MB_OK);
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


	// RTT 객체를 생성합니다.
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}



	// RTT 객체를 초기화합니다.
	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	// 디버그 윈도우를 생성합니다.
	m_DebugWindow = new DebugWindowClass;
	if (!m_DebugWindow)
	{
		return false;
	}

	// 디버그 윈도우를 초기화합니다.
	result = m_DebugWindow->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, 100, 100);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the debug window object.", L"Error", MB_OK);
		return false;
	}

	// 텍스쳐 셰이더를 생성합니다.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// 텍스쳐 셰이더를 초기화합니다.
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
	// 디버그 윈도우를 해제합니다.
	if (m_DebugWindow)
	{
		m_DebugWindow->Shutdown();
		delete m_DebugWindow;
		m_DebugWindow = 0;
	}

	// RTT 객체를 해제합니다.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// Release the model object.
	if(m_GameObject)
	{
		m_GameObject->Shutdown();
		delete m_GameObject;
		m_GameObject = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
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
	
	// 전체 씬을 텍스쳐에 그립니다.
	result = RenderToTexture();
	if (!result)
	{
		return false;
	}


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	
	// 백버퍼에 평소처럼 전체 씬을 그립니다.
	result = RenderScene();
	if (!result)
	{
		return false;
	}

	// 2D 렌더링을 하기 위해 Z버퍼를 끕니다.
	m_D3D->TurnZBufferOff();

	// 카메라와 d3d 객체로부터 월드, 뷰, 직교 행렬을 얻어옵니다.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// 디버그 윈도우의 정점과 인덱스 버퍼를 그래픽 파이프라인에 넣어 렌더링할 준비를 합니다.
	result = m_DebugWindow->Render(m_D3D->GetDeviceContext(), 50, 50);
	if (!result)
	{
		return false;
	}

	// 텍스쳐 셰이더를 이용하여 디버그 윈도우를 그립니다.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), worldMatrix, viewMatrix,
		orthoMatrix, m_RenderTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}


	//// 2D렌더링이 끝났으므로 다시 Z버퍼를 킵니다.
	m_D3D->TurnZBufferOn();

	//static int counter = 0;
	//Start theDear ImGui Frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	//Docking
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
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
	ImGui::End();


	ImGui::Begin("Directory");
	
	ImGui::End();
	//Assemble Together Draw Data
	ImGui::Render();
	//Render Draw Data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	m_D3D->EndScene();

	// Present the rendered scene to the screen.
	return true;
}

bool GraphicsClass::RenderToTexture()
{
	bool result;


	// RTT가 렌더링 타겟이 되도록 합니다.
	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());
	// RTT를 초기화합니다.
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	// 여기서 씬을 그리면 백버퍼 대신 RTT에 렌더링됩니다.
	result = RenderScene();
	if (!result)
	{
		return false;
	}

	// 렌더링 타겟을 RTT에서 다시 백버퍼로 돌립니다.
	m_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::RenderScene()
{
	// Generate the view matrix based on the camera's position.
	m_Camera->Render();


	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_GameObject->Render(m_D3D->GetDeviceContext());
	m_GameObject->Draw(m_LightShader, m_Light);

	return true;
}
