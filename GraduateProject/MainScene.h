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
	GameObject* temp1;
	GameObject* temp;


	void update(Transform* transform)
	{
		transform->Rotate();
		return;
	}

	void InitObject() override {
		bool result;
		temp = new GameObject("sphere","object",Scene::getD3D(), Scene::getCamera(), nullptr);
		Model* model = temp->getOrAddComponent<Model>();
		
		temp->update = [](Transform* transform) {transform->Translate(XMFLOAT3(0, -5, 0)); };
		// Initialize the model object.
		temp->getComponent<Model>()->SetGetShader<MultiTextureShaderClass>();
		result = temp->getComponent<Model>()->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/atlas.obj", L"./data/res/Altar_diffuse.dds",L"./data/res/block.dds"
			, Scene::getLightShader(), Scene::getLight(), Scene::hwnd);
		if (!result)
		{
			return;
		}

		m_GameObject.push_back(temp);
		if (!temp)
		{
			return;
		}

		temp1 = new GameObject("chair", "object", Scene::getD3D(), Scene::getCamera(), nullptr);
		Model* model1 = temp1->getOrAddComponent<Model>();
		// Initialize the model object.
		temp1->update = [](Transform* transform) {transform->Translate(XMFLOAT3(0, -5, 0)); };
		temp1->getComponent<Model>()->SetGetShader<MultiTextureShaderClass>();
		result = temp1->getComponent<Model>()->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/platform.obj", L"./data/res/Platform_diffuse.dds", L"./data/res/block.dds", Scene::getLightShader(), Scene::getLight(), Scene::hwnd);

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

		GameObject* temp3 = new GameObject("text", "text", Scene::getD3D(), Scene::getCamera());
		Text* text = temp3->getOrAddComponent<Text>();

		XMMATRIX baseviewMatrix;
		Scene::getCamera()->GetViewMatrix(baseviewMatrix);
		// Initialize the bitmap object.
		result = text->Initialize(Scene::getD3D()->GetDevice(), Scene::getD3D()->GetDeviceContext(), Scene::hwnd, Scene::getscreenWidth(), Scene::getscreenHeight(), baseviewMatrix);
		if (!result)
		{
			return;
		}
		m_GameObject.push_back(temp3);
	}


};
#endif
