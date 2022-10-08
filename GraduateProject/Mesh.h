////////////////////////////////////////////////////////////////////////////////
// Filename: mesh.h
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
#include <vector>

using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"

#include <fstream>
using namespace std;

struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
};

class Mesh
{
public:
	


public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11Buffer* m_vertexBuffer, DWORD*& indices);
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<VertexType>& vertices, std::vector<DWORD>& indices);
	Mesh(const Mesh& mesh);
	void RenderBuffers();
private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	ID3D11DeviceContext* m_deviceContext;
	
};

#endif

