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
#include "SkyPlaneClass.h"
#include "ParticleSystemClass.h"
#include "textureshaderclass.h"
#include "rendertextureclass.h"
#include "debugwindowclass.h"
#include "ShaderManagerClass.h"
#include "Assimp/ModelLoader.h"

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
	virtual bool Render();
	void upadteHierachy();
	bool isLoaded() { return isLoad; }
	void updateshaderSetting();
	vector<GameObject*> readGameObjectList() const;

	//System Parameter
	int m_fps, m_cpu;
	float m_frameTime;
protected:
	SceneManager* sceneManager;
	vector<GameObject*> m_GameObject;
	virtual void InitObject() {}
	virtual bool RenderScene();
	int getscreenHeight();
	int getscreenWidth();
	HWND hwnd;
	D3DClass* getD3D();
	CameraClass* getCamera();
	LightClass* getLight();
	ShaderManagerClass* getShaderManager();
	bool isLoad;

	//Reflection Model
	GameObject* refractionModel;
	GameObject* reflectionMdoel;

};

#endif

