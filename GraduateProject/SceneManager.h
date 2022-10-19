////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "Scene.h"

class SceneManager
{
public:
	void CreateScene();
	void LoadScene(string sceneName);
	//void LoadScene(int sceneIndex);
	void SetActiveScene();

private:
	int m_sceneCount;
	//int m_sceneCountInBuildSettings

};

#endif

