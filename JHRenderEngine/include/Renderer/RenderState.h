//
//  RenderState.h
//
//  Created by Jingshu Huang on 8/28/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __RenderState__
#define __RenderState__

#include "platformDef.h"
#include "platformGL.h"


class RenderState {
public:
    RenderState();
    ~RenderState();
    
    // Setters and getters for render states.
    inline void setEnableDepthTest(const bool val) { mEnableDepthTest = val; }
    inline void setEnableDepthWrite(const bool val) { mEnableDepthWrite = val; }
    inline void setEnableBlend(const bool val) { mEnableBlend = val; }
    inline void setEnableCullFace(const bool val) { mEnableCullFace = val; }

    inline bool getEnableDepthTest() const { return mEnableDepthTest; }
    inline bool getEnableDepthWrite() const { return mEnableDepthWrite; }
    inline bool getEnableBlend() const { return mEnableBlend; }
    inline bool getEnableCullFace() const { return mEnableCullFace; }
    inline GLenum getDepthFunc() const { return mDepthFunc; }
    inline GLenum getBlendSrcFactor() const { return mBlendSrcFactor; }
    inline GLenum getBlendDstFactor() const { return mBlendDstFactor; }
    inline GLenum getCullFaceMode() const { return mCullFaceMode; }
    
    void copyFrom(const RenderState& other);

    static RenderState kDefaultRenderState;

private:
    void applyBlendMode();
    void applyDepthMode();
    void applyColorMode();
    void applyCullMode();

    bool mEnableDepthTest;
    bool mEnableDepthWrite;
    bool mEnableBlend;
    bool mEnableCullFace;
    
    GLenum mDepthFunc;
    GLenum mCullFaceMode;
    GLenum mBlendSrcFactor, mBlendDstFactor;
    
    friend class Pass;
};


#endif
