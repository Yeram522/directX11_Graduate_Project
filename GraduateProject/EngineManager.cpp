#include "EngineManager.h"
#include "graphicsclass.h"


bool EngineManager::Initialize(D3DClass* D3D, HWND hwnd, GraphicsClass* graphicClass)
{
	m_graphicClass = graphicClass;
	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
 //   //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
//   io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(D3D->GetDevice(), D3D->GetDeviceContext());
	ImGui::StyleColorsDark();

	return true;
	
	
}

void EngineManager::renderImGui(Scene* scene)
{
	//Start theDear ImGui Frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	//Docking
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	//Content
	showInspector();
	showViewPort();
	showContentBrowser();
	showHierachy(scene);
	//content end
	

	//Assemble Together Draw Data
	ImGui::Render();
	//Render Draw Data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void EngineManager::showInspector()
{
	//Create ImGui Test Window
	ImGui::Begin("Inspector");
	ImGui::Text("PhongShader");
	ImGui::SliderFloat3("LightDirection", m_graphicClass->m_Light->GetDirectiontoFloat(), -100.0f, 100.0f);
	ImGui::SliderFloat4("AmbidientColor", m_graphicClass->m_Light->GetAmbientColortoFloat(), 0.0f, 1.0f);
	ImGui::SliderFloat4("DiffuseColor", m_graphicClass->m_Light->GetDiffuseColortoFloat(), 0.0f, 1.0f);
	ImGui::SliderFloat4("SpecularColor", m_graphicClass->m_Light->GetSpecularColortoFloat(), 0.0f, 1.0f);
	ImGui::SliderFloat("Specular power", m_graphicClass->m_Light->GetSpecularPower(0), 0.0f, 100.0f);
	ImGui::End();
}

void EngineManager::showViewPort()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("ViewPort");
	{
		// Using a Child allow to fill all the space of the window.
		// It also alows customization
		ImGui::BeginChild("GameRender");
		// Get the size of the child (i.e. the whole draw size of the windows).
		ImVec2 wsize = ImGui::GetWindowSize();
		// Because I use the texture from OpenGL, I need to invert the V from the UV.
		ImGui::Image((ImTextureID)(m_graphicClass->m_RenderTexture)->GetShaderResourceView(), wsize);
		ImGui::EndChild();
	}
	ImGui::PopStyleVar();

	ImGui::End();
}

constexpr const char* s_AssetDirectory = "data";

void EngineManager::showContentBrowser()
{
	ImGui::Begin("Content Browser");


	for (auto& p : fs::directory_iterator(s_AssetDirectory))
	{
		std::string path = p.path().string();
		
		if (ImGui::Button(path.c_str()))
		{
		}
	}
	ImGui::End();


}

//update방식처럼 아래 노드로 계속 추가되는>?방향으로 함수코드 개선필요함~
void EngineManager::showHierachy(Scene* scene)
{
	ImGui::Begin("Hierachy");
	
	for (int i = 0; i < scene->readGameObjectList().size(); i++)
	{
		GameObject* object = scene->readGameObjectList()[i];
		if (ImGui::TreeNode(object->getname().c_str()))
		{
			for (int j = 0; j < object->getchildrens().size(); j++)
			{
				// Use SetNextItemOpen() so set the default state of a node to be open. We could
				// also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
				if (j == 0)
					ImGui::SetNextItemOpen(true, ImGuiCond_Once);

				if (ImGui::TreeNode(object->getchildrens()[j]->getname().c_str()))
				{
					ImGui::TreePop();
				}
			}


			ImGui::TreePop();
		}
	}
    
	
	ImGui::End();
}
