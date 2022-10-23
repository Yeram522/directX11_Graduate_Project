////////////////////////////////////////////////////////////////////////////////
// Filename: titlescene.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TITLESCENE_H_
#define _TITLESCENE_H_

#include "Scene.h"
class TitleScene : public Scene
{
public:
	TitleScene():Scene("Title")
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
		result = temp->Initialize(Scene::getD3D(), Scene::getCamera(), L"./data/chair.obj", L"./data/chair_d.dds");
		if (!result)
		{
			return;
		}
	}
};
#endif

