////////////////////////////////////////////////////////////////////////////////
// Filename: EngineManager.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _EngineManager_H_
#define _EngineManager_H_

#include "d3dclass.h"
#include "SceneManager.h"

#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;


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

	void initImGui();
	void updateImGui();
	void renderImGui();


	void showInspector();
	void showViewPort();
	void showContentBrowser();
	void showHierachy(Scene*);
private:
	static EngineManager* Instance;
	GraphicsClass* m_graphicClass;
};

#endif

