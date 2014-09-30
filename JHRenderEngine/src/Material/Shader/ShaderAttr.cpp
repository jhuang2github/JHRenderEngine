//
//  ShaderAttr.cpp
//
//  Created by Jingshu Huang on 5/21/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "ShaderAttr.h"


void ShaderAttr::disable() {
    if (mLoc >= 0) {
        GL_disableVertexAttribArray(mLoc);
    }
}

bool ShaderAttr::enable() const {
    if (mLoc < 0) {
        printf("ShaderAttr::enable %s failed.\n", mName.c_str());
        return false;
    }

    GL_vertexAttribPointer(mLoc,
                          mSizeInFloat,
                          GL_FLOAT,
                          GL_FALSE,
                          mStride,
                          (void*)mOffset);
    GL_enableVertexAttribArray(mLoc);
    return true;
}

