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
#include <iostream>
class Model
{
public:
    /*  함수   */
    Model(string path)
    {
        //loadModel(path);
        mesh = new Mesh();
    }
    void Draw(LightShaderClass*, LightClass* , CameraClass* , D3DClass* );
    Mesh* getMesh() { return mesh; }
private:
    /*  Model 데이터  */
    vector<Mesh> meshes;
    Mesh* mesh;
    string directory;
    /*  함수   */
    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        string typeName);

};

#endif

