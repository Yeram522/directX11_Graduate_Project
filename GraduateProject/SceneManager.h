////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "MainScene.h"
#include "TitleScene.h"

class SceneManager
{
public:
	//¾À ¸Å´ÏÀú = ½Ì±ÛÅæ
	static SceneManager* GetInstance()
	{
		if (Instance == nullptr) {
			Instance = new SceneManager();
		}
		return Instance;
	}
	bool Initialize(int screenWidth, int screenHeight,D3DClass* m_D3D, CameraClass* m_Camera,HWND hwnd, LightClass*, LightShaderClass*);

	//¾ÀÀ» ·Îµå ¹× »ý¼º
	template <typename T>
	void CreateScene(T* newscene)
	{
		m_sceneinbuild.push_back((Scene*)newscene);
		m_sceneCount = m_sceneinbuild.size();
		LoadScene((Scene*)newscene);
	}

	template <typename T>
	void LoadScene(T* scene)
	{
		bool result;
		m_ActiveScene = new T();

		result = m_ActiveScene->Initialize(screenWidth, screenHeight,hwnd);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize Scene.", L"Error", MB_OK);
			return;
		}
	}
	void UpdateScene();
	//void LoadScene(int sceneIndex);

	Scene* getActiveScene() const { return m_ActiveScene; }
private:
	static SceneManager* Instance;
	//»ý¼ºÀÚ´Â privateÀ¸·Î µÐ´Ù.
	SceneManager() :m_sceneCount(0), m_ActiveScene(nullptr)
	{}

	friend class Scene;

	int m_sceneCount;
	//int m_sceneCountInBuildSettings
	vector<Scene*> m_sceneinbuild;
	Scene* m_ActiveScene;

	//Initialize SceneManager form graphics Class
	int screenWidth,screenHeight;

private:
	HWND hwnd;
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	LightClass* m_Light;
	LightShaderClass* m_LightShader;
};

#endif

