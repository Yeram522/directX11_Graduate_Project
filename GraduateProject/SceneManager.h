////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "Scene.h"

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
	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	//¾ÀÀ» ·Îµå ¹× »ý¼º
	void CreateScene(string name);
	void LoadScene(string sceneName);
	void UpdateScene();
	//void LoadScene(int sceneIndex);

private:
	static SceneManager* Instance;
	//»ý¼ºÀÚ´Â privateÀ¸·Î µÐ´Ù.
	SceneManager() :m_sceneCount(0), m_ActiveScene(nullptr)
	{}

	int m_sceneCount;
	//int m_sceneCountInBuildSettings
	vector<Scene*> m_sceneinbuild;
	Scene* m_ActiveScene;

	//Initialize SceneManager form graphics Class
	int screenWidth,screenHeight;
	HWND hwnd;
	
};

#endif

