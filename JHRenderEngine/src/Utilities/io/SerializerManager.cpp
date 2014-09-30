//
//  SerializerManager.cpp
//
//  Created by Jingshu Huang on 2/1/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//


// TODO(jhuang): change most of the getters to template.
//

#include "ShaderSerializer.h"
#include "MaterialSerializer.h"
#include "MeshSerializer.h"
#include "MeshSerializer.h"
#include "SceneSerializer.h"
#include "SerializerManager.h"
#include "ShaderSerializer.h"
#include "stringUtility.h"
#include "TextureSerializer.h"


// scene. The difference between a ".def" and ".scene" files are when we encouter
// a duplicate scene file, we make a duplicate of the corresponding scene node
// and its children, and added them to the scene graph. For ".def" file, we do
// not duplicate.
const string kSceneExt = "scene";
const string kDefExt = "def";

// mesh.
const string kObjExt = "obj";

// material.
const string kMaterialExt = "material";

// texture.
const string kPngExt = "png";
const string kPvrExt = "pvr";

// shaders.
static const string kVertShaderExt = "vsh";
static const string kFragShaderExt = "fsh";


SerializerManager::SerializerManager() {
    // Scene serializers.
    mSceneSerializers[kSceneExt] = new SceneSerializer;
    mSceneSerializers[kDefExt] = new SceneSerializer;

    // Mesh serializers.
    mMeshSerializers[kObjExt] = new ObjSerializer;

    // Material serializers.
    mMaterialSerializers[kMaterialExt] = new MatSerializer;

    // Texture serializers.
    mTextureSerializers[kPngExt] = new PngSerializer;
    mTextureSerializers[kPvrExt] = new PvrSerializer;

    // Shader serialiers.
    mShaderSerializers[kVertShaderExt] =
    mShaderSerializers[kFragShaderExt] = new GLSLShaderSerializer;
}

SerializerManager::~SerializerManager() {
    for (MeshSerializerMap::iterator iter = mMeshSerializers.begin();
         iter != mMeshSerializers.end(); ++iter) {
        if (iter->second) {
            delete iter->second;
        }
    }
    mMeshSerializers.clear();
}

bool SerializerManager::isSceneFile(const string &fileName) {
    return stringUtility::getFileExtension(fileName) == kSceneExt;
}

SceneSerializer* SerializerManager::getSceneSerializer(const string& name) {
    string extension = stringUtility::getFileExtension(name);
    if (mSceneSerializers.find(extension) != mSceneSerializers.end()) {
        return mSceneSerializers[extension];
    }
    LOGD("Cannot find the correct scene serializer for %s\n", name.c_str());
    return NULL;
}

MeshSerializer* SerializerManager::getMeshSerializer(const string& name) {
    string extension = stringUtility::getFileExtension(name);
    if (mMeshSerializers.find(extension) != mMeshSerializers.end()) {
        return mMeshSerializers[extension];
    }
    return NULL;
}

MaterialSerializer* SerializerManager::getMaterialSerializer(const string& name) {
    string extension = stringUtility::getFileExtension(name);
    if (mMaterialSerializers.find(extension) != mMaterialSerializers.end()) {
        return mMaterialSerializers[extension];
    }
    LOGD("Cannot find the correct material serializer for %s\n", name.c_str());
    return NULL;
}

TextureSerializer* SerializerManager::getTextureSerializer(const string& name) {
    string extension = stringUtility::getFileExtension(name);
    if (mTextureSerializers.find(extension) != mTextureSerializers.end()) {
        return mTextureSerializers[extension];
    }
    LOGD("Cannot find the correct texture serializer for %s\n", name.c_str());
    return NULL;
}

ShaderSerializer* SerializerManager::getShaderSerializer(const string& name) {
    string extension = stringUtility::getFileExtension(name);
    if (mShaderSerializers.find(extension) != mShaderSerializers.end()) {
        return mShaderSerializers[extension];
    }
    LOGD("Cannot find the correct shader serializer for %s\n", name.c_str());
    return NULL;
}

