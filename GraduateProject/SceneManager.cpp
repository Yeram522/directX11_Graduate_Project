#include "SceneManager.h"

bool SceneManager::Initialize(int screenWidth, int screenHeight, D3DClass* m_D3D,CameraClass* m_Camera, HWND hwnd,LightClass* m_light, ShaderManagerClass* shadermanager)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->hwnd = hwnd;
	this->m_D3D = m_D3D;
	this->m_Camera = m_Camera;
	this->m_Light = m_light;
	this->m_ShaderManager = shadermanager;

	bool result;
	m_ActiveScene = new MainScene();
	if (!m_ActiveScene)
	{
		return false;
	}

	result = m_ActiveScene->Initialize(screenWidth, screenHeight,hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Scene.", L"Error", MB_OK);
		return false;
	}

	return true;

}



void SceneManager::UpdateScene()
{
	m_ActiveScene->Render();
}

void SceneManager::UpdateHierachy()
{
	m_ActiveScene->upadteHierachy();
}

void SceneManager::SetShader()
{
	m_ActiveScene->updateshaderSetting();
}
