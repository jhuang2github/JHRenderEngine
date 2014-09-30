//
//  Pass.cpp
//
//  Created by Jingshu Huang on 1/23/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include <sstream>

#ifdef __Apple__

#include <ext/hash_fun.h>
using namespace __gnu_cxx;

#else

#include <google/protobuf/stubs/hash.h>

#endif

#include "platformDef.h"
#include "mathUtility.h"
#include "Pass.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "ShadingTechnique.h"
#include "stringUtility.h"
#include "Texture.h"
#include "TextureManager.h"


static const string kVShaderExt = "vsh";
static const string kFShaderExt = "fsh";


Pass::Pass() {
    assert(false);
}


Pass::Pass(ShadingTechnique* tech) : mParent(tech) {
}


// Here we load the default texture of a pass in a material. The actual texture
// used when rendering an entity is set at runtime by changing the reference of
// actual texture stored in mTextures[unit].
void Pass::loadTexture(const string &texFileName, const int& texUnit) {
    TextureManager* texMgr = TextureManager::getInstance();
    Texture* texture = texMgr->getAsset(texFileName);
    if (!texture) {
        LOGD("tid=%d [Pass loadTexture] Texture Manager load pass default tex file=%s", getThreadID(), texFileName.c_str());
        texture = texMgr->load(texFileName);
    } else {
        LOGD("tid=%d [Pass loadTexture] already loaded pass default tex file=%s", getThreadID(), texFileName.c_str());
    }
    assert(texture != NULL);

    if (mTextures.find(texUnit) != mTextures.end()) {
        LOGD("Error: Pass::loadTexture(): tex unit %d is specified more than once. Please check the correctness of the material file.", texUnit);
        return;
    }

    mTextures[texUnit] = texture;
}


void Pass::loadShader(const string& vShader, const string& fShader) {
    assert(stringUtility::endsWith(vShader, kVShaderExt, true) &&
           stringUtility::endsWith(fShader, kFShaderExt, true));
    ShaderManager* shaderMgr = ShaderManager::getInstance();

    string shaderPairName = encodeShaderName(vShader, fShader);
    mShader = shaderMgr->getAsset(shaderPairName);
    if (!mShader) {
        mShader = shaderMgr->load(shaderPairName);
    }
    assert(mShader != NULL);
}


// Compute hash for sorting renderable in render queue.
// It is first hashed with the pass index so that all passes are rendered in
// order. Then it is sorted by the shader program ID and lastly texture names
// so the state changes are minimized.
void Pass::setHash(const int& passIndex) {
    // Pass index uses 4 bits. Do not allow more than 16 passes in a technique.
    mHash = passIndex << 28;

    // Program ID uses 12 bits. Do not allow more than 2^12 = 4096 shader
    // programs loaded at once.
    mHash += mShader->getProgram() << 16;

    // Texture names use 16 bits. We only use textures in texUnit0 and texUnit1
    // when computing hash.
    string fileName, path;
    int numTexes = mTextures.size();
    if (numTexes > 0) {
        stringUtility::splitFilename(mTextures[0]->getPath(), fileName, path);
        mHash += MathUtility::shiftNumberToBitPos(strHash(fileName), 15);
    }
    if (numTexes > 1) {
        stringUtility::splitFilename(mTextures[1]->getPath(), fileName, path);
        // Set the least significant 8 bits.
        mHash &= 0xFFF0;
        mHash += MathUtility::shiftNumberToBitPos(strHash(fileName), 7);
    }
}


void Pass::useShader() const {
    assert(mShader != NULL);
    mShader->use();
}


void Pass::useRenderState() {
    useRenderState(mRenderState);
}


void Pass::useRenderState(RenderState& state) {
    state.applyBlendMode();
    state.applyDepthMode();
    state.applyCullMode();
}

void Pass::activateTextureUnits(const TexUnitToTextureMap* renderableTexMap) {
    // Check if the pass actually requires textures.
    if (!renderableTexMap) {
        return;
    }

    for (TexUnitToTextureMap::iterator iter = mTextures.begin();
         iter != mTextures.end(); ++iter) {
        int texUnit = iter->first;
        //mShader->ensureTexUnit(texUnit);
        GL_activeTexture(GL_TEXTURE0 + texUnit);

        // Use the default texture of the pass specified in the material.
        Texture* texture = iter->second;
        // Check if a renderable has overwritten the default texture(s)
        // specified in the material. If it does, use it.
        TexUnitToTextureMap::const_iterator rTexIter = renderableTexMap->find(texUnit);
        if (rTexIter != renderableTexMap->end()) {
            texture = rTexIter->second;
        }

        GL_bindTexture(GL_TEXTURE_2D, texture->getTexId());
    }
}


