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

	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	ImGui::StyleColorsDark();

	//ImGui_ImplDX11_Init(m_D3D->GetDevice(), m_D3D->GetDeviceContext());

	return true;
}


void GraphicsClass::Shutdown()
{
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
	bool result;
	
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Rotate the world matrix by the rotation value so that the triangle will spin.

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_GameObject->Render(m_D3D->GetDeviceContext());
	m_GameObject->Draw(m_LightShader, m_Light);

	static int counter = 0;
	//Start theDear ImGui Frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	//Create ImGui Test Window
	ImGui::Begin("Inspector");
	ImGui::Text("PhongShader");
	ImGui::SliderFloat3("LightDirection", m_Light->GetDirectiontoFloat(), -100.0f, 100.0f);
	ImGui::SliderFloat4("AmbidientColor", m_Light->GetAmbientColortoFloat(), 0.0f, 1.0f);
	ImGui::SliderFloat4("DiffuseColor", m_Light->GetDiffuseColortoFloat(), 0.0f, 1.0f);
	ImGui::SliderFloat4("SpecularColor", m_Light->GetSpecularColortoFloat(), 0.0f, 1.0f);
	ImGui::SliderFloat("Specular power", m_Light->GetSpecularPower(0), 0.0f, 100.0f);
	

	/*ImGui::Text("This is example text.");
	if (ImGui::Button("Click me"))
		counter += 1;
	ImGui::SameLine();
	std::string clickCount = "Click Count: " + std::to_string(counter);
	ImGui::Text(clickCount.c_str());*/
	ImGui::End();
	//Assemble Together Draw Data
	ImGui::Render();
	//Render Draw Data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}