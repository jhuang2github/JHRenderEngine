//
//  Texture.h
//
//  Created by Jingshu Huang on 1/12/13.
//  Copyright (c) HuangImage 2012. All rights reserved.
//


#ifndef _texture_h_
#define _texture_h_

#include "Asset.h"
#include "platformDef.h"
#include "platformGL.h"
#include "TextureSerializer.h"

class PlatformUtil;

typedef enum {
    PF_UNKNOWN = 0,

    PF_RGB565,
    PF_RGBA4,

    PF_RGB8,

    PF_BGRA8,
    PF_RGBA8,

    PF_DXT1,
    PF_DXT3,
    PF_DXT5,

    // pvrt
    PF_PVRTC_RGB4,
    PF_PVRTC_RGBA4,
} PixelFormat;


class Texture : public Asset {
public:
    Texture();
    Texture(const string& fileName);
    ~Texture();

    inline void setPath(const string& path) { mPath = path; };
    inline string& getPath() { return mPath; }
    inline void setWidth(const uint w) { mWidth = w; }
    inline void setHeight(const uint h) { mHeight = h; }
    inline void setPixelFormat(PixelFormat format) { mPixelFormat = format; }

    inline uint getTexId() const { return mTexId; }


    bool load();
    void loadTexImage(const MetaTexDataInfo* info);

private:
//    GLint internalFormat();
    GLenum pixelDataType();

private:
    string      mPath;
	uint        mTexId;
	uint        mWidth;
	uint        mHeight;
    PixelFormat mPixelFormat;
};


#endif
