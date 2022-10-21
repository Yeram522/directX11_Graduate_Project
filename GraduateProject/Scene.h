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
#include "ImGui\\imgui.h"
#include "ImGui\\imgui_impl_win32.h"
#include "ImGui\\imgui_impl_dx11.h"

#include<string>

using namespace std;


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
private:
	bool RenderToTexture();
	bool RenderScene();
private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	GameObject* m_GameObject;

	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	RenderTextureClass* m_RenderTexture;
	DebugWindowClass* m_DebugWindow;
	TextureShaderClass* m_TextureShader;
};

#endif

