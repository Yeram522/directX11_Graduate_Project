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
	GameObject* Cloud;
	GameObject* Effect;
	GameObject* BigToToro;
	GameObject* BabyToToro;
	GameObject* BusStop;
	GameObject* Mei;
	GameObject* GhibliTree;
	GameObject* plane;
	GameObject* Rock;
	GameObject* water;
	GameObject* bkbillboard;
	GameObject* info;

	bool Render() override
	{
		info->getComponent<Text>()->SetFps(Scene::m_fps);
		info->getComponent<Text>()->SetCpu(Scene::m_cpu);
		Cloud->getComponent<SkyPlaneClass>()->Frame();
		Effect->getComponent<ParticleSystemClass>()->Frame(Scene::m_cpu);
		return Scene::Render();
	} 

	void update(Transform* transform)
	{
		transform->Rotate();
		return;
	}

	void InitObject() override {
		bool result;

		//Envirement
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


		Cloud = new GameObject("Cloud", "skyplane", Scene::getD3D(), Scene::getCamera(), nullptr);
		SkyPlaneClass* skyplane = Cloud->getOrAddComponent<SkyPlaneClass>();
		Cloud->update = [](Transform* transform) {
			transform->FollowUpCamera(); 
			transform->SetScale(30.0f, 70.0f, 30.0f);
		};
		result = skyplane->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/cloud001.dds", L"./data/res/cloud002.dds", Scene::getShaderManager()->getSkyPlaneShader());

		if (!result)
		{
			return;
		}

		m_GameObject.push_back(Cloud);
		if (!Cloud)
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
		BabyToToro->update = [](Transform* transform) {
			transform->SetPosition(0.0f, -3.0f, 0.0f); };
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

		ModelLoader* modelLoader;
		Mei = new GameObject("Mei", "object", Scene::getD3D(), Scene::getCamera(), nullptr);
		modelLoader = Mei->getOrAddComponent<ModelLoader>();
		Mei->getComponent<Transform>()->SetScale(0.01f, 0.01f, 0.01f);
		Mei->getComponent<Transform>()->SetPosition(0.0f, -3.0f, 5.0f);
		// Initialize the model object.
		result = modelLoader->Load(Scene::hwnd,Scene::getD3D()->GetDevice(), Scene::getD3D()->GetDeviceContext(), "./data/Mei.fbx", Scene::getShaderManager()->getLightShader());

		if (!result)
		{
			return;
		}

		if (!Mei)
		{
			return;
		}
		m_GameObject.push_back(Mei);

		




		plane = new GameObject("plane", "plane", Scene::getD3D(), Scene::getCamera(), nullptr);

		Model* planeModel = plane->getOrAddComponent<Model>();
		plane->getComponent<Transform>()->SetPosition(0.0f, -3.0f, 0.0f);

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

		Scene::refractionModel = plane;
		Scene::reflectionModel = BigToToro;

		water = new GameObject("water", "plane", Scene::getD3D(), Scene::getCamera(), nullptr);
		model = water->getOrAddComponent<Model>();
		water->getComponent<Transform>()->SetPosition(0.0f, -3.0f, 0.0f);

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

		Rock->getComponent<Transform>()->SetScale(2.0f, 2.0f, 2.0f);
		Rock->getComponent<Transform>()->SetPosition(-12.5f, -2.0f, -2.5f);
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





		bkbillboard = new GameObject("bkbillboard", "object", Scene::getD3D(), Scene::getCamera(), nullptr);
		model = bkbillboard->getOrAddComponent<Model>();
		bkbillboard->getComponent<Transform>()->SetBillBoardTransform();
		bkbillboard->getComponent<Transform>()->SetPosition(0.0f, 8.7f, 42.6f);
		bkbillboard->getComponent<Transform>()->SetScale(50.0f, 20.0f,1.0f);
		bkbillboard->update = [](Transform* transform) {
			transform->FollowForwardCamera();
		};
		result = model->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/totohorse.obj", L"./data/res/TotoroBackground.dds", L"./data/res/TotoroBackground.dds", Scene::getShaderManager()->getTransparentSahder(), Scene::getLight(), Scene::hwnd);

		if (!result)
		{
			return;
		}

		if (!bkbillboard)
		{
			return;
		}
		m_GameObject.push_back(bkbillboard);


		GameObject*  bkbillboard2 = new GameObject("bkbillboard2", "object", Scene::getD3D(), Scene::getCamera(), nullptr);
		model = bkbillboard2->getOrAddComponent<Model>();
		bkbillboard2->getComponent<Transform>()->SetBillBoardTransform();
		bkbillboard2->getComponent<Transform>()->SetPosition(-50.0f, 5.0f, -6.0f);
		bkbillboard2->getComponent<Transform>()->SetScale(1.0f, 20.0f, 50.0f);
		result = model->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/totohorse2.obj", L"./data/res/TotoroBackground.dds", L"./data/res/TotoroBackground.dds", Scene::getShaderManager()->getTransparentSahder(), Scene::getLight(), Scene::hwnd);

		if (!result)
		{
			return;
		}

		if (!bkbillboard2)
		{
			return;
		}
		m_GameObject.push_back(bkbillboard2);

		GameObject* bkbillboard3 = new GameObject("bkbillboard3", "object", Scene::getD3D(), Scene::getCamera(), nullptr);
		model = bkbillboard3->getOrAddComponent<Model>();
		bkbillboard3->getComponent<Transform>()->SetBillBoardTransform();
		bkbillboard3->getComponent<Transform>()->SetPosition(50.0f, 11.0f, -6.0f);
		bkbillboard3->getComponent<Transform>()->SetScale(1.0f, 20.0f, 50.0f);
		result = model->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/totohorse3.obj", L"./data/res/TotoroBackground.dds", L"./data/res/TotoroBackground.dds", Scene::getShaderManager()->getTransparentSahder(), Scene::getLight(), Scene::hwnd);

		if (!result)
		{
			return;
		}

		if (!bkbillboard3)
		{
			return;
		}
		m_GameObject.push_back(bkbillboard3);

		GameObject* meiHousBoard = new GameObject("meiHousBoard", "object", Scene::getD3D(), Scene::getCamera(), nullptr);
		model = meiHousBoard->getOrAddComponent<Model>();
		meiHousBoard->getComponent<Transform>()->m_BillBoard = true;
		meiHousBoard->getComponent<Transform>()->SetPosition(-19.0f, 3.0f, 9.0f);
		meiHousBoard->getComponent<Transform>()->SetScale(10.0f, 10.0f, 1.0f);
		result = model->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/totohorse.obj", L"./data/res/meihousejapan.dds", L"./data/res/meihousejapan.dds", Scene::getShaderManager()->getTransparentSahder(), Scene::getLight(), Scene::hwnd);

		if (!result)
		{
			return;
		}

		if (!meiHousBoard)
		{
			return;
		}
		m_GameObject.push_back(meiHousBoard);

		GameObject* GhibliGrass = new GameObject("GhibliGrass", "object", Scene::getD3D(), Scene::getCamera(), nullptr);
		model = GhibliGrass->getOrAddComponent<Model>();
		//	GhibliHeadhouse->getComponent<Transform>()->SetRotation(0.0f, 0.0f, 0.0f);

		GhibliGrass->getComponent<Transform>()->SetScale(0.3f, 0.3f, 0.3f);
		GhibliGrass->getComponent<Transform>()->SetPosition(0.0f, 2.5f, -1.6f);
		// Initialize the model object.
		result = model->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/treeflowergroup.obj", L"./data/res/grassesTex.dds", L"./data/res/grassesTex.dds", Scene::getShaderManager()->getTransparentSahder(), Scene::getLight(), Scene::hwnd);

		if (!result)
		{
			return;
		}

		if (!GhibliGrass)
		{
			return;
		}
		m_GameObject.push_back(GhibliGrass);
		Effect = new GameObject("Particle", "effect", Scene::getD3D(), Scene::getCamera(), nullptr);
		ParticleSystemClass* particle = Effect->getOrAddComponent<ParticleSystemClass>();
		Effect->getComponent<Transform>()->m_BillBoard = true;
		Effect->update = [](Transform* transform) {
			transform->SetPosition(0.0f, 8.0f, -16.0f);
		};
		result = particle->Initialize(Scene::getD3D()->GetDevice(), L"./data/res/rain.dds", Scene::getShaderManager()->getParticleShader());

		if (!result)
		{
			return;
		}
		if (!Effect)
		{
			return;
		}

		m_GameObject.push_back(Effect);

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
