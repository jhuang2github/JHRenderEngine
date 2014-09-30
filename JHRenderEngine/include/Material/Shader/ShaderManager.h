//
//  ShaderManager.h
//
//  Created by Jingshu Huang on 12/27/12.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _ShaderManager_h
#define _ShaderManager_h

#include "platformDef.h"
#include "AssetManager.h"
#include "Shader.h"
#include "Singleton.h"


typedef enum {
    DEFAULT_NO_TEX = 0,
    DEFAULT_TEX,
    DEFAULT_TEX_ALPHA,
} EShaderType;

// Shader programs manager.
class ShaderManager : public Singleton<ShaderManager>, public AssetManager<string, Shader> {
};

static string encodeShaderName(const string& v, const string& f) {
    return v + "/" + f;
}

static bool decodeShaderName(const string& encodedName, string& v, string& f) {
    size_t i = encodedName.find_last_of("/");
    if (i == string::npos) {
        LOGD("Error in decoding shader pair names.\n");
        return false;
    }
    v = encodedName.substr(0, i);
    f = encodedName.substr(i + 1);
    return true;
}

#endif
