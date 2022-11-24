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
#include "ShaderManagerClass.h"

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
	bool Frame(InputClass*, int, int, float);

	SceneManager* getSceneManager() { return m_SceneManager; }

	friend class EngineManager;

	Image* m_image;//µð¹ö±ë

private:
	bool Render();

	bool RenderToTexture();
private:
	ShaderManagerClass* m_ShaderManager;
	SceneManager* m_SceneManager;
	EngineManager* m_EngineManager;
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	LightClass* m_Light;

	RenderTextureClass* m_RenderTexture;
	DebugWindowClass* m_DebugWindow;
	TextureShaderClass* m_TextureShader;



};

#endif