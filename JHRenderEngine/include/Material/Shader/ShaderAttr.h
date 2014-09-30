//
//  ShaderAttr.h
//
//  Created by Jingshu Huang on 5/21/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __ShaderAttr__
#define __ShaderAttr__

#include "ShaderParam.h"


class ShaderAttr : public ShaderParam {
public:
    ShaderAttr(const string& name, const GLint loc, const int sizeInFloat) : ShaderParam(name, loc) {
        mSizeInFloat = sizeInFloat;
        mStride = 0;  // Need to set at runtime
        mOffset = 0;  // Need to set at runtime
    }
    ~ShaderAttr() {}

    inline void setStride(const int stride) { mStride = stride; }
    inline void setOffset(uchar* offset) { mOffset = offset; }

    virtual bool enable() const;
    virtual void disable();

private:
    int             mSizeInFloat;
    GLsizei         mStride;
    byte*           mOffset;
};

#endif
