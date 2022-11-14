////////////////////////////////////////////////////////////////////////////////
// Filename: Model.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODEL_H_
#define _MODEL_H_
#include "Mesh.h"
#include "d3dclass.h"

#include "cameraclass.h"
#include "Transform.h"
#include "Component.h"
#include "textureclass.h"
#include "TextureArrayClass.h"
#include "ShaderManagerClass.h"
#include <iostream>

class Model:public Component
{
public:
    /*  함수   */
    Model(GameObject* gameObject);
   
    template<typename T>
    T* SetGetShader()
    {
        T* newShader = new T();
        m_Shader = newShader;

        auto result = dynamic_cast<T*>(m_Shader);
        return result;
    }


    bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*, const WCHAR* , ShaderClass* shader, LightClass* m_Light, HWND);

    bool LoadTextures(ID3D11Device*, const WCHAR*, const WCHAR*);
    void ReleaseTexture();
    ID3D11ShaderResourceView** GetTextureArray();

    void update() override;
    void Draw();
    Mesh* getMesh() { return mesh; }
private:
    /*  Model 데이터  */
    vector<Mesh> meshes;
    Mesh* mesh;
    TextureArrayClass* m_TextureArray; //Multi texturing
   // MultiTextureShaderClass* m_MultiTextureShader;
    LightShaderClass* m_LightShader;
    LightClass* m_Light;

    ShaderClass* m_Shader;
    string directory;
    /*  함수   */
    void loadModel(string path);
    

};

#endif

