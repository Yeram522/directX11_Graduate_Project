////////////////////////////////////////////////////////////////////////////////
// Filename: Model.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODEL_H_
#define _MODEL_H_
#include "Mesh.h"
#include "d3dclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "cameraclass.h"
#include "Transform.h"
#include "Component.h"
#include "textureclass.h"
#include "MultiTextureShaderClass.h"

#include <iostream>

class Model:public Component
{
public:
    /*  �Լ�   */
    Model(GameObject* gameObject);
   
    bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*, LightShaderClass* shader, LightClass* m_Light);

    bool LoadTexture(ID3D11Device*, const WCHAR*);
    void ReleaseTexture();
    ID3D11ShaderResourceView* GetTexture();

    void update() override;
    void Draw();
    Mesh* getMesh() { return mesh; }
private:
    /*  Model ������  */
    vector<Mesh> meshes;
    Mesh* mesh;
    TextureClass* m_Texture;
    LightShaderClass* m_LightShader;
    LightClass* m_Light;

    string directory;
    /*  �Լ�   */
    void loadModel(string path);
    

};

#endif

