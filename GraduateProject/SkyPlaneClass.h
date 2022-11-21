////////////////////////////////////////////////////////////////////////////////
// Filename: skyplaneclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SKYPLANECLASS_H_
#define _SKYPLANECLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Component.h"
#include "textureclass.h"
#include "Transform.h"
#include "ShaderManagerClass.h"
class SkyPlaneClass:public Component
{
private:
	struct SkyPlaneType
	{
		float x, y, z;
		float tu, tv;
	};
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	SkyPlaneClass(GameObject* gameObject);
	~SkyPlaneClass();

	bool Initialize(ID3D11Device*,const WCHAR*,const WCHAR*, SkyPlaneShaderClass*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	void Frame();
	void update() override;
	int GetIndexCount();

	ID3D11ShaderResourceView** GetTextureArray();
	float GetBrightness();
	float GetTranslation(int);

private:
	bool InitializeSkyPlane(int, float, float, float, int);
	void ShutdownSkyPlane();

	bool InitializeBuffers(ID3D11Device*, int);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*,const WCHAR*,const WCHAR*);
	void ReleaseTextures();
private:
	SkyPlaneType* m_skyPlane;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	TextureArrayClass* m_CloudTexture;
	float m_brightness;
	float m_translationSpeed[4];
	float m_textureTranslation[4];
	SkyPlaneShaderClass* m_Shader;
};

#endif

