////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "MainScene.h"
#include "TitleScene.h"
#include "ShaderManagerClass.h"
class SceneManager
{
public:
	//?? ?Ŵ??? = ?̱???
	static SceneManager* GetInstance()
	{
		if (Instance == nullptr) {
			Instance = new SceneManager();
		}
		return Instance;
	}
	bool Initialize(int screenWidth, int screenHeight,D3DClass* m_D3D, CameraClass* m_Camera,HWND hwnd, LightClass*, ShaderManagerClass*);

	//???? ?ε? ?? ????
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
	void UpdateHierachy();
	//void LoadScene(int sceneIndex);
	void SetShader();

	void SetFrameTime(float frameTime)
	{
		m_ActiveScene->m_frameTime = frameTime;
	}

	void SetFpsCpuInfo(int fps, int cpu)
	{
		m_ActiveScene->m_fps = fps;
		m_ActiveScene->m_cpu = cpu;
	}
	Scene* getActiveScene() const { return m_ActiveScene; }
private:
	static SceneManager* Instance;
	//?????ڴ? private???? ?д?.
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
	ShaderManagerClass* m_ShaderManager;
	LightClass* m_Light;
};

#endif

