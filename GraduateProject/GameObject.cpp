////////////////////////////////////////////////////////////////////////////////
// Filename: GameObject.cpp
////////////////////////////////////////////////////////////////////////////////
#include "GameObject.h"


GameObject::GameObject()
{
	m_model = new Model();
	transform = new Transform(this);
	components.push_back(transform);
}


GameObject::GameObject(const GameObject& other)
{
}


GameObject::~GameObject()
{
}


bool GameObject::Initialize(D3DClass* m_D3D,CameraClass* m_Camera, const WCHAR* modelFilename, const WCHAR* textureFilename
	,LightShaderClass* shader, LightClass* m_Light)
{
	bool result;
	this->m_Camera= m_Camera;
	result = m_model->Initialize(m_D3D->GetDevice(), modelFilename, textureFilename, shader,m_Light);
	result = transform->Initialize(m_D3D, m_Camera);

	
	if (!result)
	{
		return false;
	}

	return true;
}


void GameObject::Shutdown()
{
	// Release the model texture.
	m_model->ReleaseTexture();

	// Shutdown the vertex and index buffers.
	m_model->getMesh()->ShutdownBuffers();

	// Release the model data.
	m_model->getMesh()->ReleaseModel();

	return;
}


void GameObject::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	transform->Render();
	transform ->Rotate();


	m_model->Render(deviceContext);

	return;
}

void GameObject::Draw()
{
	m_model->Draw(transform);
}









