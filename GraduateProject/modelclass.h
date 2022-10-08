////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "mesh.h"
#include "lightshaderclass.h"
////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	/*struct VertexType
	{
		XMFLOAT3 position;
	    XMFLOAT2 texture;
		XMFLOAT3 normal;
	};*/

	struct FaceType
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct Transform //1. ����ü �ȿ� ��ġ ���� ����, ũ��, ȸ���� �ٲ� �� �ִ�!
	{
		XMFLOAT3 position;
		XMFLOAT3 rotation;
		XMFLOAT3 scale;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(const std::string& filePath, ID3D11Device*, ID3D11DeviceContext*, const WCHAR* textureFilename, LightShaderClass*);
	bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	bool LoadModel(const WCHAR*);
	bool LoadModel(const std::string& filePath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void ReleaseModel();


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

	bool ReadFileCounts(const WCHAR*);
	bool LoadDataStructures(const WCHAR*, int, int, int, int);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount, m_textureCount, m_normalCount, m_faceCount;
	Transform m_transform;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	TextureClass* m_Texture;
	ModelType* m_model;

	LightShaderClass* m_LightShader;
	std::vector<Mesh> m_meshes;
};

#endif