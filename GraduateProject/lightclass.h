////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <directxmath.h>

using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: LightClass
////////////////////////////////////////////////////////////////////////////////
class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();
	XMFLOAT4 GetSpecularColor();
	float GetSpecularPower();
	float* GetSpecularPower(int i);

	float* GetAmbientColortoFloat();
	float* GetDiffuseColortoFloat();
	float* GetSpecularColortoFloat();
private:
	/*XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_diffuseColor;
	XMFLOAT4 m_specularColor;*/
	XMFLOAT3 m_direction;

	float m_specularPower;
	float* m_ambientColor;
	float* m_diffuseColor;
	float* m_specularColor;
};

#endif