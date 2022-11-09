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
		temp = new GameObject("chair", "object", Scene::getD3D(), Scene::getCamera());
		Model* model = temp->getOrAddComponent<Model>();
		

		// Initialize the model object.
		result = temp->getComponent<Model>()->Initialize(Scene::getD3D()->GetDevice(), L"./data/chair.obj", L"./data/chair_d.dds", L"./data/chair_d.dds", Scene::getLightShader(), Scene::getLight(), Scene::hwnd);

		if (!result)
		{
			return;
		}

		m_GameObject.push_back(temp);
		if (!temp)
		{
			return;
		}
	}
};
#endif

