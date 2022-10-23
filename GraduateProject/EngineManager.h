////////////////////////////////////////////////////////////////////////////////
// Filename: EngineManager.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _EngineManager_H_
#define _EngineManager_H_

#include "d3dclass.h"
#include "ImGui\\imgui.h"
#include "ImGui\\imgui_impl_win32.h"
#include "ImGui\\imgui_impl_dx11.h"

class GraphicsClass;
class EngineManager
{
public:
	static EngineManager* GetInstance()
	{
		if (Instance == nullptr)
			Instance = new EngineManager();

		return Instance;
	}

	bool Initialize(D3DClass*, HWND, GraphicsClass*);
	void renderImGui();
	void showInspector();
	void showViewPort();
	void showContentBrowser();

private:
	static EngineManager* Instance;
	GraphicsClass* m_graphicClass;
};

#endif

