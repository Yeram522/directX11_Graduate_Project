///////////////////////////////////////////////////////////////////////////////
// Filename: Image.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _IMAGE_H_
#define _IMAGE_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include "Component.h"


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include "textureshaderclass.h"

#include "Component.h"
#include "Transform.h"

class Image : public Component
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	Image(GameObject* gameObject);
	~Image();

	bool Initialize(ID3D11Device*, int, int, const WCHAR*, int, int, HWND);
	void Shutdown();
	bool Render();
	void update() override;

	int GetIndexCount();
	ID3D11ShaderResourceView** GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_Texture;
	TextureShaderClass* m_TextureShader;
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;
};

#endif

