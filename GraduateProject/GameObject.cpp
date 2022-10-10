////////////////////////////////////////////////////////////////////////////////
// Filename: GameObject.cpp
////////////////////////////////////////////////////////////////////////////////
#include "GameObject.h"


GameObject::GameObject()
{
	m_mesh = new Mesh();
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
	result = m_mesh->LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = m_mesh->InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = m_mesh->LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void GameObject::Shutdown()
{
	// Release the model texture.
	m_mesh->ReleaseTexture();

	// Shutdown the vertex and index buffers.
	m_mesh->ShutdownBuffers();

	// Release the model data.
	m_mesh->ReleaseModel();

	return;
}


void GameObject::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_mesh->RenderBuffers(deviceContext);

	return;
}









