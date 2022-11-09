////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightclass.h"


LightClass::LightClass()
{
}


LightClass::LightClass(const LightClass& other)
{
}


LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = new float[4]{ red, green, blue, alpha };
	return;
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = new float[4]{ red, green, blue, alpha };
	return;
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
	m_fdirection = new float[3]{ x, y, z };
	return;
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = new float[4]{ red, green, blue, alpha };
	return;
}

void LightClass::SetSpecularPower(float power)
{
	m_specularPower = power;
	return;
}

XMFLOAT4 LightClass::GetAmbientColor()
{
	return XMFLOAT4(m_ambientColor[0], m_ambientColor[1], m_ambientColor[2], m_ambientColor[3]) ;
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
	return XMFLOAT4(m_diffuseColor[0], m_diffuseColor[1], m_diffuseColor[2], m_diffuseColor[3]);
}

XMFLOAT3 LightClass::GetDirection()
{
	return XMFLOAT3(m_fdirection[0], m_fdirection[1], m_fdirection[2]);
}

XMFLOAT4 LightClass::GetSpecularColor()
{
	
	return XMFLOAT4(m_specularColor[0], m_specularColor[1], m_specularColor[2], m_specularColor[3]);
}

float LightClass::GetSpecularPower()
{
	return m_specularPower;
}

float* LightClass::GetAmbientColortoFloat()
{
	return m_ambientColor;
}
float* LightClass::GetDiffuseColortoFloat()
{
	return m_diffuseColor;
}
float* LightClass::GetSpecularColortoFloat()
{
	return m_specularColor;
}

float* LightClass::GetDirectiontoFloat()
{
	return m_fdirection;
}

float* LightClass::GetSpecularPower(int i)
{
	return &m_specularPower;
}