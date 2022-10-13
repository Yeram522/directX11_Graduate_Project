////////////////////////////////////////////////////////////////////////////////
// Filename: GameObject.cpp
////////////////////////////////////////////////////////////////////////////////
#include "GameObject.h"


GameObject::GameObject(string path)
{
	m_model = new Model(path);
}


GameObject::GameObject(const GameObject& other)
{
}


GameObject::~GameObject()
{
}


bool GameObject::Initialize(ID3D11Device* device, const WCHAR* modelFilename, const WCHAR* textureFilename)
{
	bool result;

	// Load in the model data,
	result = m_model->getMesh()->LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = m_model->getMesh()->InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = m_model->getMesh()->LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void GameObject::Shutdown()
{
	// Release the model texture.
	m_model->getMesh()->ReleaseTexture();

	// Shutdown the vertex and index buffers.
	m_model->getMesh()->ShutdownBuffers();

	// Release the model data.
	m_model->getMesh()->ReleaseModel();

	return;
}


void GameObject::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_model->getMesh()->RenderBuffers(deviceContext);

	return;
}

void GameObject::Draw(LightShaderClass* shader, LightClass* m_Light, CameraClass* m_Camera, D3DClass* m_D3D)
{
	m_model->Draw(shader, m_Light, m_Camera, m_D3D);
}









