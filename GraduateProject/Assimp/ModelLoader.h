#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <vector>
#include <d3d11_1.h>
#include <DirectXMath.h>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "../Transform.h"
#include "../Component.h"
#include "AssimpMesh.h"
#include "TextureLoader.h"
#include "../TextureArrayClass.h"
#include "../ShaderManagerClass.h"

using namespace DirectX;

class ModelLoader : public Component
{
public:
	ModelLoader(GameObject* gameObject);
	~ModelLoader();

	void update() override;
	bool Load(HWND hwnd, ID3D11Device* dev, ID3D11DeviceContext* devcon, std::string filename, ShaderClass* shader);
	void Draw(ID3D11DeviceContext* devcon);
	bool LoadTextures(ID3D11Device*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*);
	ID3D11ShaderResourceView** GetTextureArray();
	void Close();
private:
	ShaderClass* m_Shader;


	ID3D11Device *dev_;
	ID3D11DeviceContext *devcon_;
	std::vector<AssimpMesh> meshes_;
	std::string directory_;
	std::vector<Texture> textures_loaded_;
	HWND hwnd_;
	TextureArrayClass* m_TextureArray; //Multi texturing

private:
	void processNode(aiNode* node, const aiScene* scene);
	AssimpMesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene);
	ID3D11ShaderResourceView* loadEmbeddedTexture(const aiTexture* embeddedTexture);
};

#endif // !MODEL_LOADER_H

