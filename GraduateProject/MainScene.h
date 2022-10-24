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
		temp = new GameObject("sphere","object",Scene::getD3D(), Scene::getCamera());
		Model* model = temp->getOrAddComponent<Model>();
		

		// Initialize the model object.
		result = temp->getComponent<Model>()->Initialize(Scene::getD3D()->GetDevice(), L"./data/Sphere.obj", L"./data/seafloor.dds"
			, Scene::getLightShader(), Scene::getLight());
		if (!result)
		{
			return;
		}

		m_GameObject.push_back(temp);
		if (!temp)
		{
			return;
		}

		GameObject*  temp1 = new GameObject("chair", "object", Scene::getD3D(), Scene::getCamera());
		Model* model1 = temp1->getOrAddComponent<Model>();


		// Initialize the model object.
		result = temp1->getComponent<Model>()->Initialize(Scene::getD3D()->GetDevice(), L"./data/chair.obj", L"./data/chair_d.dds"
			, Scene::getLightShader(), Scene::getLight());

		temp1->setParent(temp);
		if (!result)
		{
			return;
		}

		//m_GameObject.push_back(temp1);
		if (!temp1)
		{
			return;
		}

	}
};
#endif
