#include "ShaderClass.h"
#include "ShaderManagerClass.h"

ShaderManagerClass* ShaderClass::getShaderManager()
{
    return ShaderManagerClass::GetInstance();
}

ShaderClass::ShaderClass(LightClass* Light, string type):m_Light(Light), type(type)
{

}

ShaderClass::~ShaderClass()
{
}


