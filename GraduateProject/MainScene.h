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
	GameObject* SkyDome;
	GameObject* BigToToro;
	GameObject* BabyToToro;
	GameObject* BusStop;
	GameObject* Mei;
	GameObject* plane;
	GameObject* Rock;
	GameObject* water;

	GameObject* info;

	bool Render() override
	{
		info->getComponent<Text>()->SetFps(Scene::m_fps);
		info->getComponent<Text>()->SetCpu(Scene::m_cpu);
		return Scene::Render();
	} 

	void update(Transform* transform)
	{
		transform->Rotate();
		return;
	}

	void InitObject() override {
		bool result;

		SkyDome = new GameObject("skydome", "skydome", Scene::getD3D(), Scene::getCamera(), nullptr); //Instancing 예정
		Model* model = SkyDome->getOrAddComponent<Model>();
		SkyDome->update = [](Transform* transform) {transform->FollowCamera(); };
		// Initialize the model object.
		result = model->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/skydome.obj", L"./data/res/Rocks_Color.dds", L"./data/res/Rocks_Roughness.dds", Scene::getShaderManager()->getSkyDomeShader(), Scene::getLight(), Scene::hwnd);

		//temp1->setParent(temp);
		if (!result)
		{
			return;
		}

		m_GameObject.push_back(SkyDome);
		if (!SkyDome)
		{
			return;
		}

		BusStop = new GameObject("BusStop","object",Scene::getD3D(), Scene::getCamera(), nullptr);
		model = BusStop->getOrAddComponent<Model>();
		BusStop->getComponent<Transform>()->SetPosition(-0.6f, -3.0f, 2.3f);
		//BusStop->update = [](Transform* transform) {transform->SetPosition(-0.6f, -3.0f, 2.3f); };
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
		BigToToro->getComponent<Transform>()->SetPosition(0.0f, -3.0f, 0.0f);
	//	BigToToro->update = [](Transform* transform) {transform->SetPosition(0.0f, -3.0f, 0.0f); };
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
		BabyToToro->getComponent<Transform>()->SetPosition(0.0f, -3.0f, 0.0f);
		//BabyToToro->update = [](Transform* transform) {transform->SetPosition(0.0f, -3.0f, 0.0f); };
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
		Mei->getComponent<Transform>()->SetPosition(0.0f, -3.0f, 5.0f);
		//Mei->update = [](Transform* transform) {transform->SetPosition(0.0f, -3.0f, 5.0f); };
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


		plane = new GameObject("plane", "plane", Scene::getD3D(), Scene::getCamera(), nullptr);
		Model* planeModel = plane->getOrAddComponent<Model>();
		plane->getComponent<Transform>()->SetPosition(0.0f, -3.0f, 0.0f);
		//plane->update = [](Transform* transform) {transform->SetPosition(0.0f, -3.0f, 0.0f); };

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

		Scene::refractionModel = BigToToro;
		Scene::reflectionMdoel = plane;

		water = new GameObject("water", "plane", Scene::getD3D(), Scene::getCamera(), nullptr);
		model = water->getOrAddComponent<Model>();
		water->getComponent<Transform>()->SetPosition(0.0f, -3.0f, 0.0f);
		//water->update = [](Transform* transform) {transform->SetPosition(0.0f, -3.0f, 0.0f); };

		// Initialize the model object.
		result = model->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/waterDemo.obj", Scene::getShaderManager()->getReflectionTexture()->GetShaderResourceView(), Scene::getShaderManager()->getRefractionTexture()->GetShaderResourceView(), Scene::getShaderManager()->getWaterShader(), Scene::getLight(), Scene::hwnd);

		if (!result)
		{
			return;
		}

		if (!water)
		{
			return;
		}
		m_GameObject.push_back(water);

		Rock = new GameObject("rock", "object", Scene::getD3D(), Scene::getCamera(), nullptr); //Instancing 예정
		model = Rock->getOrAddComponent<Model>();
		//Rock->getComponent<Transform>()->m_BillBoard = true;

		Rock->getComponent<Transform>()->SetPosition(0.0f, -2.0f, 10.0f);
		// Initialize the model object.
		result = model->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/rock.obj", L"./data/res/Rocks_Color.dds", L"./data/res/Rocks_Roughness.dds", Scene::getShaderManager()->getMultiTextureShader(), Scene::getLight(), Scene::hwnd);

		//temp1->setParent(temp);
		if (!result)
		{
			return;
		}

		m_GameObject.push_back(Rock);
		if (!Rock)
		{
			return;
		}

		GameObject* billboardingT;
		billboardingT = new GameObject("billboardingT", "object", Scene::getD3D(), Scene::getCamera(), nullptr); //Instancing 예정
		model = billboardingT->getOrAddComponent<Model>();
		billboardingT->getComponent<Transform>()->m_BillBoard = true;

		billboardingT->getComponent<Transform>()->SetPosition(0.0f, -2.0f, 15.0f);
		// Initialize the model object.
		result = model->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/2DObject.obj", L"./data/res/tree2.dds", L"./data/res/tree2A.dds", Scene::getShaderManager()->getTransparentSahder(), Scene::getLight(), Scene::hwnd);

		//temp1->setParent(temp);
		if (!result)
		{
			return;
		}

		m_GameObject.push_back(billboardingT);
		if (!billboardingT)
		{
			return;
		}


		info = new GameObject("text", "text", Scene::getD3D(), Scene::getCamera());
		Text* text = info->getOrAddComponent<Text>();

		XMMATRIX baseviewMatrix;
		Scene::getCamera()->GetViewMatrix(baseviewMatrix);
		// Initialize the bitmap object.
		result = text->Initialize(Scene::getD3D()->GetDevice(), Scene::getD3D()->GetDeviceContext(), Scene::hwnd, Scene::getscreenWidth(), Scene::getscreenHeight(), baseviewMatrix);
		if (!result)
		{
			return;
		}
		m_GameObject.push_back(info);
	}


};
#endif
