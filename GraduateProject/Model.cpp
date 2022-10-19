#include "Model.h"

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	mesh->RenderBuffers(deviceContext);
}

void Model::Draw(LightShaderClass* shader, LightClass* m_Light, Transform* transform)
{
	bool result;

	//d3d랑 camera는 씬에서 가져와야하는데 일다 ㄴ씬이 없어서 transform에서 가져옴!
	result = shader->Render(transform->m_D3D->GetDeviceContext(), mesh->GetIndexCount(),
		transform->m_worldMatrix, transform->m_viewMatrix, transform->m_projectionMatrix,
		GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		transform->m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if (!result)
	{
		return;
	}
}

Model::Model()
{
	m_Texture = 0;
	mesh = new Mesh();


}

bool Model::Initialize(ID3D11Device* device, const WCHAR* modelFilename, const WCHAR* textureFilename)
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

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

bool Model::LoadTexture(ID3D11Device* device, const WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}


void Model::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* Model::GetTexture()
{
	return m_Texture->GetTexture();
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

