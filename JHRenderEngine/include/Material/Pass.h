//
//  Pass.h
//
//  Created by Jingshu Huang on 1/23/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Pass_h
#define _Pass_h

#include "platformDef.h"
#include "RenderState.h"
#include "TextureManager.h"


class Shader;
class ShadingTechnique;
class Texture;


// A Pass is a single rendering pass, i.e. a single draw (of a material), of a
// shading technique in a material. This is a per renderable object.
class Pass {
public:
    Pass();
    Pass(ShadingTechnique* tech);
    ~Pass() {}

    void loadTexture(const string& texFileName, const int& texUnit);
    void loadShader(const string& vShaderFileName,
                    const string& fShaderFileName);

    // Get the hash of this pass.
    void setHash(const int& passIndex);
    inline uint getHash() const { return mHash; }

    inline Shader* getShader() const { return mShader; }

    void useRenderState();
    void useRenderState(RenderState& state);
    void useShader() const;
    void activateTextureUnits(const TexUnitToTextureMap* renderableTexMap);

private:
    ShadingTechnique*   mParent;

    Shader*             mShader;    // Shader program. Do not own the data.
    TexUnitToTextureMap mTextures;  // Textures used. Do not own the data. Initialized to a pass' default textures, if set.

    // A precomputed number used for sorting passes. For details, see
    // setHash(const int&).
    uint mHash;

    RenderState mRenderState;
};


#endif
