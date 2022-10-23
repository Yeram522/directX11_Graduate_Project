////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "SceneManager.h"
#include "EngineManager.h"




////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

	SceneManager* getSceneManager() { return m_SceneManager; }

	friend class EngineManager;
private:
	bool Render(float);

	bool RenderToTexture();
private:
	SceneManager* m_SceneManager;
	EngineManager* m_EngineManager;
	Scene* m_Scene;
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	RenderTextureClass* m_RenderTexture;
	DebugWindowClass* m_DebugWindow;
	TextureShaderClass* m_TextureShader;
};

#endif