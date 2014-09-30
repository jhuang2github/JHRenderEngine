//
//  ShaderUnfm.h
//
//  Created by Jingshu Huang on 5/21/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __ShaderUnfm__
#define __ShaderUnfm__

#include "ShaderParam.h"


typedef enum {
    UNFM_SCENE_SCOPE = 0,
    UNFM_SCENE_LAYER_SCOPE,
    UNFM_MODEL_SCOPE
} EShaderUniformScope;


typedef enum {
    _1i = 0,
    _1f,
    _4f,
    _1fv,
    _2fv,
    _3fv,
    _4fv,
    _Matrix3fv,
    _Matrix4fv
} EShaderUnfmType;


class ShaderUnfm : public ShaderParam {
public:
    ShaderUnfm(const string& name,
               const GLint loc,
               const int count,
               const EShaderUnfmType type) : ShaderParam(name, loc) {
        mCount = count;
        mType = type;
        mValue = NULL;  // Need to set at run time.
    }
    ~ShaderUnfm() {}

    inline void setValue(float* value) { mValue = value; }
    inline void setValue(const float value) {
        if (!mValue) {
            mValue = new float;
        }
        *mValue = value;
    }
    inline float* getValue() { return mValue; }

    virtual bool enable() const;
    virtual void disable();

private:
    float*          mValue;
    int             mCount;
    EShaderUnfmType mType;
};

#endif
