////////////////////////////////////////////////////////////////////////////////
// Filename: Mesh.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MESH_H_
#define _MESH_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
using namespace DirectX;

#include <fstream>
///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Component.h"
#include "textureclass.h"

struct Face //인덱스 버퍼?
{
	unsigned int* mIndices;

	//예비
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
struct Texture {
	unsigned int id;
	string type;
};
class Mesh
{
private:
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};


	//FaceType
	Face* m_faces;
public:
	Mesh();
	bool LoadModel(const WCHAR*);
	void ReleaseModel();
	int GetIndexCount();
	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	ID3D11ShaderResourceView* GetTexture();
private:
	bool ReadFileCounts(const WCHAR*);
	bool LoadDataStructures(const WCHAR*, int, int, int, int);
private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;

	int m_vertexCount, m_indexCount, m_textureCount, m_normalCount, m_faceCount;
	TextureClass* m_Texture;
	ModelType* m_model;

	//New Mesh Data
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
};
#endif
