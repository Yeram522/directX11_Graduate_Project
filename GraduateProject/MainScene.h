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
	GameObject* BigToToro;
	GameObject* BabyToToro;
	GameObject* BusStop;
	GameObject* Mei;
	GameObject* plane;



	GameObject* water;

	void update(Transform* transform)
	{
		transform->Rotate();
		return;
	}

	void InitObject() override {
		bool result;
		BusStop = new GameObject("BusStop","object",Scene::getD3D(), Scene::getCamera(), nullptr);
		Model* model = BusStop->getOrAddComponent<Model>();
		BusStop->update = [](Transform* transform) {transform->SetPosition(-0.6f, -3.0f, 2.3f); };
		//temp->update = [](Transform* transform) {transform->Rotate(); };
		// Initialize the model object.

		result = model->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/busStop.obj", L"./data/res/BusSign_Texture.dds",L"./data/res/BusSign_Texture.dds"
			, Scene::getShaderManager()->getLightShader(), Scene::getLight(), Scene::hwnd);
		if (!result)
		{
			return;
		}

		m_GameObject.push_back(BusStop);
		if (!BusStop)
		{
			return;
		}

		BigToToro = new GameObject("Totoro", "object", Scene::getD3D(), Scene::getCamera(), nullptr);
		model = BigToToro->getOrAddComponent<Model>();
		BigToToro->update = [](Transform* transform) {transform->SetPosition(0.0f, -3.0f, 0.0f); };
		// Initialize the model object.
		result = model->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/bigTotoro.obj", L"./data/res/BigTotoro_Texture.dds", L"./data/res/BigTotoro_Texture.dds", Scene::getShaderManager()->getLightShader(), Scene::getLight(), Scene::hwnd);

		//temp1->setParent(temp);
		if (!result)
		{
			return;
		}

		m_GameObject.push_back(BigToToro);
		if (!BigToToro)
		{
			return;
		}


		BabyToToro = new GameObject("BabyTotoro", "object", Scene::getD3D(), Scene::getCamera(), nullptr);
		model = BabyToToro->getOrAddComponent<Model>();
		BabyToToro->update = [](Transform* transform) {transform->SetPosition(0.0f, -3.0f, 0.0f); };
		// Initialize the model object.
		result = model->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/chiTotoro.obj", L"./data/res/ChiTotoro_Texture.dds", L"./data/res/ChiTotoro_Texture.dds", Scene::getShaderManager()->getLightShader(), Scene::getLight(), Scene::hwnd);

		//temp1->setParent(temp);
		if (!result)
		{
			return;
		}

		if (!BabyToToro)
		{
			return;
		}
		m_GameObject.push_back(BabyToToro);


		Mei = new GameObject("Mei", "object", Scene::getD3D(), Scene::getCamera(), nullptr);
		model = Mei->getOrAddComponent<Model>();
		Mei->update = [](Transform* transform) {transform->SetPosition(0.0f, -3.0f, 5.0f); };
		// Initialize the model object.
		result = model->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/mei.obj", L"./data/res/Mei_Texture.dds", L"./data/res/Mei_Texture.dds", Scene::getShaderManager()->getLightShader(), Scene::getLight(), Scene::hwnd);

		//temp1->setParent(temp);
		if (!result)
		{
			return;
		}

		if (!Mei)
		{
			return;
		}
		m_GameObject.push_back(Mei);

		//GameObject* temp2 = new GameObject("image", "sprite", Scene::getD3D(), Scene::getCamera());
		//Image* image = temp2->getOrAddComponent<Image>();
	
		//// Initialize the bitmap object.
		//result = image->Initialize(Scene::getD3D()->GetDevice(), Scene::getscreenWidth(), Scene::getscreenHeight(), L"./data/edi.dds", 256, 256,Scene::hwnd);
		//if (!result)
		//{
		//	return ;
		//}
		//m_GameObject.push_back(temp2);

		//GameObject* temp3 = new GameObject("text", "text", Scene::getD3D(), Scene::getCamera());
		//Text* text = temp3->getOrAddComponent<Text>();

		//XMMATRIX baseviewMatrix;
		//Scene::getCamera()->GetViewMatrix(baseviewMatrix);
		//// Initialize the bitmap object.
		//result = text->Initialize(Scene::getD3D()->GetDevice(), Scene::getD3D()->GetDeviceContext(), Scene::hwnd, Scene::getscreenWidth(), Scene::getscreenHeight(), baseviewMatrix);
		//if (!result)
		//{
		//	return;
		//}
		//m_GameObject.push_back(temp3);

		plane = new GameObject("plane", "plane", Scene::getD3D(), Scene::getCamera(), nullptr);
		Model* planeModel = plane->getOrAddComponent<Model>();
		plane->update = [](Transform* transform) {transform->SetPosition(0.0f, -3.0f, 0.0f); };

		// Initialize the model object.
		result = planeModel->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/plane.obj", L"./data/res/grass.dds", L"./data/res/block.dds", Scene::getShaderManager()->getMultiTextureShader(), Scene::getLight(), Scene::hwnd);

		if (!result)
		{
			return;
		}

		if (!plane)
		{
			return;
		}
		m_GameObject.push_back(plane);

		water = new GameObject("water", "plane", Scene::getD3D(), Scene::getCamera(), nullptr);
		model = water->getOrAddComponent<Model>();
		//plane->update = [](Transform* transform) {transform->SetPosition(0.0f, -3.0f, 0.0f); };

		// Initialize the model object.
		result = model->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/plane.obj", Scene::getShaderManager()->RenderReflectionToTexture(BigToToro, Scene::getD3D(), Scene::getCamera(), Scene::getLight()), Scene::getShaderManager()->RenderRefractionToTexture(plane, Scene::getD3D(), Scene::getCamera(), Scene::getLight()), Scene::getShaderManager()->getWaterShader(), Scene::getLight(), Scene::hwnd);

		if (!result)
		{
			return;
		}

		if (!water)
		{
			return;
		}
		m_GameObject.push_back(water);


	}


};
#endif
