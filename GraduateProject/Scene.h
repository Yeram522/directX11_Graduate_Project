////////////////////////////////////////////////////////////////////////////////
// Filename: scene.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SCENE_H_
#define _SCENE_H_


#include "d3dclass.h"
#include "cameraclass.h"
#include "GameObject.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "textureshaderclass.h"
#include "rendertextureclass.h"
#include "debugwindowclass.h"


#include<string>
#include <vector>

using namespace std;

class SceneManager;

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Scene
{
public:
	Scene(string);
	~Scene();

	string name;

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Render();

	bool isLoaded() { return isLoad; }

protected:
	SceneManager* sceneManager;
	vector<GameObject*> m_GameObject;
	virtual void InitObject() {}
	virtual bool RenderScene();

	D3DClass* getD3D();
	CameraClass* getCamera();
	bool isLoad;
};

#endif

