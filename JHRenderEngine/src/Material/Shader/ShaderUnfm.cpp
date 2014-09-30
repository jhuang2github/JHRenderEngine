//
//  ShaderUnfm.cpp
//
//  Created by Jingshu Huang on 5/21/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "ShaderUnfm.h"


bool ShaderUnfm::enable() const {
    if (mLoc < 0) {
//        printf("ShaderUnfm::enable %s failed.\n", mName.c_str());
        return false;
    }

    switch(mType) {
        case _1i:
            GL_uniform1i(mLoc, *mValue);
            break;
        case _1f:
            GL_uniform1f(mLoc, *mValue);
            break;
        case _1fv:
            GL_uniform1fv(mLoc, mCount, mValue);
            break;
        case _2fv:
            GL_uniform2fv(mLoc, mCount, mValue);
            break;
        case _3fv:
            GL_uniform3fv(mLoc, mCount, mValue);
            break;
        case _4fv:
            GL_uniform4fv(mLoc, mCount, mValue);
            break;
        case _Matrix4fv:
            GL_uniformMatrix4fv(mLoc, mCount, GL_FALSE, mValue);
            break;
        default:
            printf("Error: uniform type is not recognized: %d\n", mType);
            break;
    }
    return true;
}

void ShaderUnfm::disable() {
}
