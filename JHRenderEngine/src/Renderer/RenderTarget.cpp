//
//  RenderTarget.cpp
//
//  Created by Jingshu Huang on 3/14/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "GLUtil.h"
#include "RenderTarget.h"
#include "Viewport.h"


RenderTarget::RenderTarget() {
    mClearBuffersBits = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    mClearColor = Vector4(0.5f, 0.5f, 0.5f, 1.f);
    mWidth = mHeight = 0;
    mViewports.clear();
}


RenderTarget::~RenderTarget() {
    for (ViewportMap::iterator iter = mViewports.begin();
         iter != mViewports.end(); ++iter) {
        delete iter->second;
    }
    mViewports.clear();
}

void RenderTarget::set(const uint _fboId, const uint _colorId, const uint _depthId) {
    fboId = _fboId;
    colorId = _colorId;
    depthId = _depthId;
}

void RenderTarget::setDimension(const int w, const int h) {
    mWidth = w;
    mHeight = h;
}

void RenderTarget::clearFrameBuffer(uint clearBuffers, const Vector4& color) {
    GL_clearColor(color.x, color.y, color.z, color.w);
    GL_clear(clearBuffers);
}

void RenderTarget::setupDefaultViewport(const int left, const int top,
                                        const int width, const int height) {
    setDimension(width, height);
    if (mViewports.empty()) {
        addViewport(0, left, top, mWidth, mHeight);
    }
}

Viewport* RenderTarget::getDefaultViewport() const {
    assert(!mViewports.empty() && "Error: default viewport is not set up properly.");
    return mViewports.begin()->second;
}

Viewport* RenderTarget::addViewport(int ZOrder, float left, float top,
                                    float width, float height) {
    // Check that no existing viewport with this Z-order.
    ViewportMap::iterator it = mViewports.find(ZOrder);
    if (it != mViewports.end()) {
        LOGD("Cannot create another viewport because another viewport with the z order %d already exists\n", ZOrder);
        return NULL;
    }
    // Add viewport to list. Order based on Z-Order
    Viewport* vp = new Viewport(this, Rect(left, top, width, height), ZOrder);
    mViewports.insert(ViewportMap::value_type(ZOrder, vp));
    return vp;
}
