#include "Scene.h"

Scene::Scene(string name)
{
	this->name = name;

	m_D3D = 0;
	m_Camera = 0;

}

Scene::~Scene()
{
}

bool Scene::Initialize(int screenWidth, int screenHeight, D3DClass* m_D3D, CameraClass* m_Camera,HWND hwnd,
	LightClass* m_light, LightShaderClass* shader)
{
	bool result;
	
	this->m_D3D = m_D3D;
	this->m_Light = m_light;
	this->m_LightShader = shader;

	// Set the initial position of the camera.
	this->m_Camera = m_Camera;

	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);	// for cube

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
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if (m_D3D)
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





bool Scene::RenderScene()
{
	// Generate the view matrix based on the camera's position.


	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	for (auto& object : m_GameObject)
	{
		object->Render(m_D3D->GetDeviceContext());
		object->Draw(m_LightShader, m_Light);
	}
	

	return true;
}
