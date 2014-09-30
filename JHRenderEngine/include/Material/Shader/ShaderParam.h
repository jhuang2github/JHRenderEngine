//
//  ShaderParam.h
//
//  Created by Jingshu Huang on 5/21/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __ShaderParam__
#define __ShaderParam__

#include "GLUtil.h"
#include "platformDef.h"


class ShaderParam {
public:
    ShaderParam() {
        mLoc = -1;
        mName.clear();
    }
    ShaderParam(const string& name, const GLint loc) {
        mLoc = loc;
        mName = name;

        if (kParamNameToNameIdMap.find(mName) == kParamNameToNameIdMap.end()) {
            auto res = std::mismatch(kTexturePrefix.begin(), kTexturePrefix.end(),
                                     mName.begin());
            if (res.first != kTexturePrefix.end()) {
                // kTexturePrefix is NOT a prefix of mName
                assert(false &&
                       "[ ShaderParam::ShaderParam() ]: unable to find the param name to ID map\n");
            }
        }
        mNameId = kParamNameToNameIdMap[mName];
    }
    virtual ~ShaderParam() {}

    inline string& getName() { return mName; }
    inline stringId getNameId() { return mNameId; }
    inline GLint getLocation() { return mLoc; }

    virtual bool enable() const = 0;
    virtual void disable() = 0;


    // Default shader params.
    typedef enum {
        kPositionId = 0,
        kTexCoordsId,
        kColorId,
        kViewProjMatId,
        kProjectionMatId,
        kViewMatId,
        kModelMatId,
        kTexturePrefixId
    } EShaderParamId;

    const static string kPosition;
    const static string kTexCoords;
    const static string kColor;
    const static string kViewProjMat;
    const static string kProjectionMat;
    const static string kViewMat;
    const static string kModelMat;
    const static string kTexturePrefix;

protected:
    typedef unordered_map<string, EShaderParamId> ParamNameToNameIdMap;
    static ParamNameToNameIdMap kParamNameToNameIdMap;

    static ParamNameToNameIdMap createParamNameToNameIdMap();

    string          mName;
    stringId        mNameId;
    GLint           mLoc;
};

#endif
