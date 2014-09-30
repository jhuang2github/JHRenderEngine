//
//  RenderTarget.h
//
//  Created by Jingshu Huang on 1/16/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _RenderTarget_h
#define _RenderTarget_h

#include <map>
#include "platformDef.h"
#include "vector4.h"


class Camera;
class Viewport;


class RenderTarget {
public:
    RenderTarget();
    virtual ~RenderTarget();

    void set(const uint _fboId, const uint _colorId, const uint _depthId);
    void setDimension(const int w, const int h);

    // Orientated width and height.
    inline int getWidth() const { return mWidth; }
    inline int getHeight() const { return mHeight; }

    virtual Viewport* addViewport(int ZOrder, float left, float top,
                                  float width, float height);

    inline const uint getClearBuffers() const { return mClearBuffersBits; }
    inline void setClearColor(const Vector4& c) { mClearColor = c; }
    inline const Vector4& getClearColor() const { return mClearColor; }

    inline void clearFrameBuffer() {
        clearFrameBuffer(mClearBuffersBits, mClearColor);
    }
    
    void clearFrameBuffer(uint clearBuffers, const Vector4& color);

    void setupDefaultViewport(const int left, const int top,
                              const int width, const int height);
    Viewport* getDefaultViewport() const;

    virtual void update(const float) = 0;
    virtual void render(const float) = 0;

protected:
    int mWidth;
    int mHeight;

    uint fboId;
    union {
        uint texId;    // tex id that binds to the fbo
        uint colorId;  // the default frame buffer color render buffer
    };
    uint depthId;

    // Viewports. Sort by z order (ascendingly -> back to front, assuming z
    // axis is pointing outward), which is the key.
    typedef map<int, Viewport*> ViewportMap;
    ViewportMap mViewports;

    
    uint mClearBuffersBits;
    Vector4 mClearColor;
};

#endif
