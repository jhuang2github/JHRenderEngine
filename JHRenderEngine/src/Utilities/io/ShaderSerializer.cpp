//
//  ShaderSerializer.cpp
//
//  Created by Jingshu Huang on 2/15/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "DataStream.h"
#include "Shader.h"
#include "ShaderSerializer.h"


#pragma GLSLShaderSerializer

bool GLSLShaderSerializer::importShader(vector<FileDataStream*>& sources,
                                        Shader* shader) {
    assert(sources.size() >= 2);
    // Assume the first is vertex shader and second fragment shader.
    FileDataStream* vertStream = sources[0];
    FileDataStream* fragStream = sources[1];

    GLuint program = shader->loadShaderSources(vertStream->getData(),
                                               fragStream->getData());
    if (program <= 0) {
        LOGD("Error loading %s %s \n", (vertStream->getFileName()).c_str(),
             (fragStream->getFileName()).c_str());
        return false;
    }
    return true;
}
