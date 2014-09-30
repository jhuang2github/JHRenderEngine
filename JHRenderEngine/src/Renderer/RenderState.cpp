//
//  RenderState.cpp
//
//  Created by Jingshu Huang on 8/28/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "GLUtil.h"
#include "RenderState.h"

RenderState RenderState::kDefaultRenderState;


RenderState::RenderState() {
    mEnableDepthTest = true;
    mEnableDepthWrite = true;
    mDepthFunc = GL_LEQUAL;
    
    mEnableBlend = true;
    mBlendSrcFactor = GL_SRC_ALPHA;
    mBlendDstFactor = GL_ONE_MINUS_SRC_ALPHA;
    
    mEnableCullFace = false;
    mCullFaceMode = GL_BACK;
    
//    glDisable(GL_DITHER);
//    glDisable(GL_ALPHA_TEST);
//    glDisable(GL_STENCIL_TEST);
}

RenderState::~RenderState() {
}


void RenderState::copyFrom(const RenderState &other) {
    mEnableDepthTest = other.mEnableDepthTest;
    mEnableDepthWrite = other.mEnableDepthWrite;
    mDepthFunc = other.mDepthFunc;
    
    mEnableBlend = other.mEnableBlend;
    mBlendSrcFactor = other.mBlendSrcFactor;
    mBlendDstFactor = other.mBlendDstFactor;
    
    mEnableCullFace = other.mEnableCullFace;
    mCullFaceMode = other.mCullFaceMode;
}


void RenderState::applyBlendMode() {
    if (mEnableBlend) {
        glEnable(GL_BLEND);
        glBlendFunc(mBlendSrcFactor, mBlendDstFactor);
    } else {
        glDisable(GL_BLEND);
    }
}

void RenderState::applyDepthMode() {
    if (mEnableDepthTest) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(mDepthFunc);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    
    if (mEnableDepthWrite) {
        glDepthMask(true);
    } else {
        glDepthMask(false);
    }
}

void RenderState::applyCullMode() {
    if (mEnableCullFace) {
        glEnable(GL_CULL_FACE);
        glCullFace(mCullFaceMode);
    } else {
        glDisable(GL_CULL_FACE);
    }
}
