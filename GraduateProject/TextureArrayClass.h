////////////////////////////////////////////////////////////////////////////////
// Filename: texturearrayclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTUREARRAYCLASS_H_
#define _TEXTUREARRAYCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <vector>
class TextureArrayClass
{
public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass&);
	~TextureArrayClass();

	bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*);
	bool Initialize(ID3D11Device*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();
	ID3D11ShaderResourceView** GetMultiTextureArray();
	std::vector<ID3D11ShaderResourceView*> m_multiTexture;

private:
	ID3D11ShaderResourceView* m_textures[2];
};

#endif

