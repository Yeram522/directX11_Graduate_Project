#include "SceneManager.h"

bool SceneManager::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->hwnd;

	bool result;
	m_ActiveScene = new Scene("Main");
	if (!m_ActiveScene)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_ActiveScene->Initialize(screenWidth, screenHeight, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Scene.", L"Error", MB_OK);
		return false;
	}

	return true;

}

void SceneManager::CreateScene(string name)
{
	m_sceneinbuild.push_back(new Scene(name));
	m_sceneCount = m_sceneinbuild.size();
	LoadScene(name);
}

void SceneManager::LoadScene(string sceneName)
{
	for (auto& scene : m_sceneinbuild)
	{
		if (sceneName == scene->name)
			m_ActiveScene = scene;
	}

	bool result;
	// Initialize the Direct3D object.
	result = m_ActiveScene->Initialize(screenWidth, screenHeight, hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Scene.", L"Error", MB_OK);
		return ;
	}
}

void SceneManager::UpdateScene()
{
	m_ActiveScene->Render();
}
