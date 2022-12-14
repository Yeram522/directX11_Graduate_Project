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
		GameObject* temp2 = new GameObject("image", "sprite", Scene::getD3D(), Scene::getCamera());
		Image* image = temp2->getOrAddComponent<Image>();
		temp2->getComponent<Transform>()->SetPosition(-100.0f,100.0f, 0.0f);
		// Initialize the bitmap object.
		result = image->Initialize(Scene::getD3D()->GetDevice(), Scene::getscreenWidth(), Scene::getscreenHeight(), L"./data/totoTitle.dds", 1920, 1080,Scene::hwnd);
		if (!result)
		{
			return ;
		}
		m_GameObject.push_back(temp2);

	}
};
#endif

