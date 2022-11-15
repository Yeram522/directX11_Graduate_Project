#include "Model.h"


void Model::update()
{
	Draw();
}

void Model::Draw()
{
	bool result;
	Transform* transform = Component::transform;
	mesh->RenderBuffers(transform->m_D3D->GetDeviceContext());//render

	//d3d랑 camera는 씬에서 가져와야하는데 일다 ㄴ씬이 없어서 transform에서 가져옴!
	/*result = m_LightShader->Render(transform->m_D3D->GetDeviceContext(), mesh->GetIndexCount(),
		transform->m_worldMatrix, transform->m_viewMatrix, transform->m_projectionMatrix,
		GetTextureArray(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		transform->m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());*/

	m_Shader->Render(transform->m_D3D->GetDeviceContext(), mesh->GetIndexCount(), transform->m_worldMatrix, transform->m_viewMatrix, transform->m_projectionMatrix,GetTextureArray());

	/*if (!result)
		return;*/
}

Model::Model(GameObject* gameObject):Component(gameObject)
{
	m_TextureArray = 0;
	mesh = new Mesh();


}

bool Model::Initialize(ID3D11Device* device, const WCHAR* modelFilename, const WCHAR* textureFilename1
, const WCHAR* textureFilename2, ShaderClass* shader, LightClass* m_Light, HWND hwnd)
{
	bool result;

	// Load in the model data,
	result = mesh->LoadMesh(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = mesh->InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the textures for this model.
	result = LoadTextures(device, textureFilename1, textureFilename2);
	if (!result)
	{
		return false;
	}

	this->m_Shader = shader;
	/*this->m_LightShader = shader;
	this->m_Light = m_Light;*/

	return true;
}

bool Model::Initialize(ID3D11Device* device, const WCHAR* modelFilename, ID3D11ShaderResourceView* texture1, ID3D11ShaderResourceView* texture2, ShaderClass* shader, LightClass* m_Light, HWND hwnd)
{
	bool result;

	// Load in the model data,
	result = mesh->LoadMesh(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = mesh->InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the textures for this model.
	result = LoadTextures(device, texture1, texture2);
	if (!result)
	{
		return false;
	}

	this->m_Shader = shader;
	/*this->m_LightShader = shader;
	this->m_Light = m_Light;*/

	return true;
}

bool Model::LoadTextures(ID3D11Device* device, const WCHAR* filename1, const WCHAR* filename2)
{
	bool result;


	// Create the texture array object.
	m_TextureArray = new TextureArrayClass;
	if (!m_TextureArray)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_TextureArray->Initialize(device, filename1, filename2);
	if (!result)
	{
		return false;
	}

	return true;
}

bool Model::LoadTextures(ID3D11Device* device, ID3D11ShaderResourceView* texture1, ID3D11ShaderResourceView* texture2)
{
	bool result;


	// Create the texture array object.
	m_TextureArray = new TextureArrayClass;
	if (!m_TextureArray)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_TextureArray->Initialize(device, texture1, texture2);
	if (!result)
	{
		return false;
	}

	return true;
}


void Model::ReleaseTexture()
{
	// Release the texture object.
	if (m_TextureArray)
	{
		m_TextureArray->Shutdown();
		delete m_TextureArray;
		m_TextureArray = 0;
	}

	return;
}

ID3D11ShaderResourceView** Model::GetTextureArray()
{
	return m_TextureArray->GetTextureArray();
}


void Model::loadModel(string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	/*if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);*/
}

