//
//  ShaderSerializer.h
//
//  Created by Jingshu Huang on 2/15/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _ShaderSerializer_h
#define _ShaderSerializer_h

#include "Serializer.h"


class FileDataStream;
class Shader;


class ShaderSerializer : public Serializer {
public:
    virtual ~ShaderSerializer() {}
    virtual bool importShader(vector<FileDataStream*>&, Shader*) = 0;
};


#pragma shader serializer implementation

class GLSLShaderSerializer : public ShaderSerializer {
public:
    GLSLShaderSerializer() {
        mVersion = "vES2_0";
    }
    ~GLSLShaderSerializer() {
    }

    bool importShader(vector<FileDataStream*>&, Shader*);
};

#endif
