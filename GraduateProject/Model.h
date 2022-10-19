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

#include <iostream>
class Model
{
public:
    /*  함수   */
    Model();
   
    bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*);

    bool LoadTexture(ID3D11Device*, const WCHAR*);
    void ReleaseTexture();
    ID3D11ShaderResourceView* GetTexture();


    void Draw(LightShaderClass*, LightClass* , CameraClass* , D3DClass* );
    Mesh* getMesh() { return mesh; }
private:
    /*  Model 데이터  */
    vector<Mesh> meshes;
    Mesh* mesh;
    TextureClass* m_Texture;


    string directory;
    /*  함수   */
    void loadModel(string path);
    

};

#endif

