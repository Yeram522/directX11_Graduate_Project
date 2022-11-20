#include "ShaderClass.h"
#include "ShaderManagerClass.h"

ShaderManagerClass* ShaderClass::getShaderManager()
{
    return ShaderManagerClass::GetInstance();
}

ShaderClass::ShaderClass(LightClass* Light):m_Light(Light)
{

}

ShaderClass::~ShaderClass()
{
}


