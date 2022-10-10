////////////////////////////////////////////////////////////////////////////////
// Filename: Model.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODEL_H_
#define _MODEL_H_
#include "Mesh.h"

class Model
{
public:
    /*  함수   */
    Model(char* path)
    {
        loadModel(path);
    }
    //void Draw(Shader shader);
private:
    /*  Model 데이터  */
    vector<Mesh> meshes;
    string directory;
    /*  함수   */
    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        string typeName);
};

#endif

