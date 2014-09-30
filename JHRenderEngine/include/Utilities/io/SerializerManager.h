//
//  SerializerManager.h
//
//  Created by Jingshu Huang on 2/1/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _SerializerManager_h
#define _SerializerManager_h

#include "platformDef.h"
#include "Singleton.h"

class MaterialSerializer;
class MeshSerializer;
class SceneSerializer;
class ShaderSerializer;
class TextureSerializer;


class SerializerManager : public Singleton<SerializerManager> {
public:
    SceneSerializer* getSceneSerializer(const string&);
    MeshSerializer* getMeshSerializer(const string&);
    MaterialSerializer* getMaterialSerializer(const string&);
    TextureSerializer*  getTextureSerializer(const string&);
    ShaderSerializer* getShaderSerializer(const string&);

    bool isSceneFile(const string& fileName);

    SerializerManager();
    ~SerializerManager();

private:
    typedef unordered_map<string, SceneSerializer*> SceneSerializerMap;
    SceneSerializerMap mSceneSerializers;

    typedef unordered_map<string, MeshSerializer*> MeshSerializerMap;
    MeshSerializerMap mMeshSerializers;

    typedef unordered_map<string, MaterialSerializer*> MaterialSerializerMap;
    MaterialSerializerMap mMaterialSerializers;

    typedef unordered_map<string, TextureSerializer*> TextureSerializerMap;
    TextureSerializerMap mTextureSerializers;

    typedef unordered_map<string, ShaderSerializer*> ShaderSerializerMap;
    ShaderSerializerMap mShaderSerializers;
};

#endif
