////////////////////////////////////////////////////////////////////////////////
// Filename: GameObject.cpp
////////////////////////////////////////////////////////////////////////////////
#include "GameObject.h"


GameObject::GameObject(string name, string tag ,
	D3DClass* m_D3D , CameraClass* m_Camera ,  // transform
	GameObject* parent, std::function<void(Transform*)> update):update(update)
{
	bool result;
	this->name = name;
	this->tag = tag;
	transform = new Transform(this);
	components.push_back(transform);

	this->m_Camera = m_Camera;
	result = transform->Initialize(m_D3D, m_Camera);
	if (!result)
	{
		return;
	}
}


GameObject::GameObject(const GameObject& other)
{
}


GameObject::~GameObject()
{
}


bool GameObject::Initialize(D3DClass* m_D3D,CameraClass* m_Camera)
{
	bool result;
	this->m_Camera= m_Camera;

	
	result = transform->Initialize(m_D3D, m_Camera);

	
	if (!result)
	{
		return false;
	}

	return true;
}


void GameObject::Shutdown()
{
	//// Release the model texture.
	//m_model->ReleaseTexture();

	//// Shutdown the vertex and index buffers.
	//m_model->getMesh()->ShutdownBuffers();

	//// Release the model data.
	//m_model->getMesh()->ReleaseModel();

	return;
}


void GameObject::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//transform->update();
	transform->update();

	//d여기에 이동및 회전하는 매트릭스 식을 넣어야함~!
	//transform->Rotate();//수정 예정.삭제시켜야됨!
	update(transform);

	for (auto component : components)
	{
		if (component->tag == "transform") continue;
		component->update();
	}
	for (auto child : children) child->Render(deviceContext);


	return;
}



void GameObject::updateHierachy()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		for (auto child : children) child->updateHierachy();
		ImGui::TreePop();
	}


}

string GameObject::getname()
{
	 return this->name; 
}










