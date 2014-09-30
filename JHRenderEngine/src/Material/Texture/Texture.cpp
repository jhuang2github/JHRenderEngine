//
//  Texture.cpp
//
//  Created by Jingshu Huang on 12/29/12.
//  Copyright (c) HuangImage 2012. All rights reserved.
//

#include "DataStream.h"
#include "GLUtil.h"
#include "platformUtil.h"
#include "platformPathUtil.h"
#include "SerializerManager.h"
#include "Texture.h"
#include "TextureSerializer.h"


Texture::Texture() {
    mPath.clear();
    mTexId = 0;
    mWidth = mHeight = 0;
    mPixelFormat = PF_RGBA8;
}

Texture::Texture(const string& fileName) {
    mPath = PlatformPathUtil::assetPath + "/" + PlatformPathUtil::imageSubPath + "/" + fileName;
    mTexId = 0;
    mWidth = mHeight = 0;
    mPixelFormat = PF_RGBA8;
}

Texture::~Texture() {
}

bool Texture::load() {
    TextureSerializer* serializer = SerializerManager::getInstance()->getTextureSerializer(mPath);
    if (PlatformUtil::isZipped(mPath)) {
        ZipFileDataStream data(mPath);
        serializer->importTexture(data, this);
    } else {
        FileDataStream data(mPath);
        serializer->importTexture(data, this);
    }
    return true;
}

// Only need to handle uncompressed texture here.
GLenum Texture::pixelDataType() {
    switch (mPixelFormat) {
        case PF_RGBA4:
            return GL_UNSIGNED_SHORT_4_4_4_4;
        case PF_RGB565:
            return GL_UNSIGNED_SHORT_5_6_5;
        default:
            break;
    }
    return GL_UNSIGNED_BYTE;
}

void Texture::loadTexImage(const MetaTexDataInfo* info) {
    if (!info || info->data.size() <= 0) {
        LOGE("%d [Texture loadTexImage] Error: MetatTexDataInfo is empty. texFile=%s",
             getThreadID(), mPath.c_str());
        return;
    }
    if (info->data.size() <= 0) {
        LOGE("%d [Texture loadTexImage] Error: info data size = 0. texFile=%s",
             getThreadID(), mPath.c_str());
        return;
    }

    setWidth(info->width);
    setHeight(info->height);
    setPixelFormat((PixelFormat)info->pixelFormat);

	GL_genTextures(1, &mTexId);
	GL_bindTexture(GL_TEXTURE_2D, mTexId);

	GL_texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	GL_texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GL_texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int numMipmaps = info->data.size();
    if (numMipmaps > 1) {
        GL_texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    } else {
        GL_texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    // Do not use the opengl's default 4-byte alignment for the tex data.
    // Pack and unpack the data tightly.
    GL_pixelStorei(GL_PACK_ALIGNMENT, 1);
    GL_pixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int mipLevelWidth = mWidth;
    int mipLevelHeight = mHeight;
	for (int i = 0; i < numMipmaps; ++i) {
        const TexDataToSizeMap& m = info->data[i];
		byte* data = m.first;
        int size = m.second;
        if (info->isCompressed) {
            GL_compressedTexImage2D(GL_TEXTURE_2D, i, info->internalFormat,
                                   mipLevelWidth, mipLevelHeight, 0, size, data);
        } else {
            GL_texImage2D(GL_TEXTURE_2D, i, info->internalFormat, mWidth, mHeight,
                         0, info->internalFormat, pixelDataType(), data);
        }

        if (glGetErrorPrettyPrint(__LINE__)) {
			LOGE("[Texture loadTexImage]: Error: failed to send texture to GPU: %s!\n", mPath.c_str());
		}
		mipLevelWidth = std::max(mipLevelWidth >> 1, 1);
		mipLevelHeight = std::max(mipLevelHeight >> 1, 1);
	}
}

