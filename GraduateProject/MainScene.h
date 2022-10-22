////////////////////////////////////////////////////////////////////////////////
// Filename: mainscene.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MAINSCENE_H_
#define _MAINSCENE_H_

#include "Scene.h"
class MainScene :public Scene
{
public:
	MainScene() :Scene("main")
	{};

	void InitObject() override {
		bool result;
		GameObject* temp = 0;
		temp = new GameObject();
		m_GameObject.push_back(temp);
		if (!temp)
		{
			return;
		}

		// Initialize the model object.
		result = temp->Initialize(m_D3D, m_Camera, L"./data/Sphere.obj", L"./data/seafloor.dds");
		if (!result)
		{
			return;
		}
	}
};
#endif
