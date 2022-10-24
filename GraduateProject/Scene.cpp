#include "Scene.h"
#include "SceneManager.h"

Scene::Scene(string name)
{
	this->name = name;
	sceneManager = SceneManager::GetInstance();
}

Scene::~Scene()
{
}

bool Scene::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	sceneManager->m_Camera->SetPosition(0.0f, 0.0f, -5.0f);	// for cube
	
	//나중에는 기본값이 아닌 씬에 있는 세팅값으로 바꿔져야함~!
	sceneManager->m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);	// for cube
	sceneManager->m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);// for cube
	sceneManager->m_Light->SetDirection(1.0f, 0.0f, 1.0f);	// for cube
	sceneManager->m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);	// for cube
	sceneManager->m_Light->SetSpecularPower(32.0f);	// for cube

	// Create the model object.
	InitObject();
	

	
	return true;
}

void Scene::Shutdown()
{

	// Release the model object.
	for (auto& object : m_GameObject)
	{
		if (object)
		{
			object->Shutdown();
			delete object;
			object = 0;
		}
	}
	

	// Release the camera object.
	if (sceneManager->m_Camera)
	{
		delete sceneManager->m_Camera;
		sceneManager->m_Camera = 0;
	}

	// Release the D3D object.
	if (sceneManager->m_D3D)
	{
		sceneManager->m_D3D->Shutdown();
		delete sceneManager->m_D3D;
		sceneManager->m_D3D = 0;
	}

	// Release the light object.
	if (sceneManager->m_Light)
	{
		delete sceneManager->m_Light;
		sceneManager->m_Light = 0;
	}

	// Release the light shader object.
	if (sceneManager->m_LightShader)
	{
		sceneManager->m_LightShader->Shutdown();
		delete sceneManager->m_LightShader;
		sceneManager->m_LightShader = 0;
	}

	return;
}

bool Scene::Render()
{
	bool result;

	// 백버퍼에 평소처럼 전체 씬을 그립니다.
	result = RenderScene();
	if (!result)
	{
		return false;
	}

	

	return true;
}

vector<GameObject*> Scene::readGameObjectList() const
{
	return m_GameObject;
}

bool Scene::RenderScene()
{

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	for (auto& object : m_GameObject)
	{
		object->Render(sceneManager->m_D3D->GetDeviceContext());
	}
	

	return true;
}

D3DClass* Scene::getD3D()
{
	return sceneManager->m_D3D;
}

CameraClass* Scene::getCamera()
{
	return sceneManager->m_Camera;
}

LightClass* Scene::getLight()
{
	return sceneManager->m_Light;
}

LightShaderClass* Scene::getLightShader()
{
	return sceneManager->m_LightShader;
}
