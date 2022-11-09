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
#include "Image.h"
#include "textclass.h"
#include "inputclass.h"

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
	bool Frame(InputClass*, double time);

	SceneManager* getSceneManager() { return m_SceneManager; }

	friend class EngineManager;

	Image* m_image;//µð¹ö±ë

private:
	bool Render();

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

	Text* m_Text;
};

#endif