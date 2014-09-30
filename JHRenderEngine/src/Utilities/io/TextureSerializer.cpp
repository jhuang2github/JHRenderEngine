//
//  TextureSerializer.cpp
//
//  Created by Jingshu Huang on 2/11/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "DataStream.h"
#include "PlatformImageUtil.h"
#include "Texture.h"
#include "TextureSerializer.h"


MetaTexDataInfo::MetaTexDataInfo() {
    isCompressed = false;
    width = height = 0;
    internalFormat = GL_RGBA;
    pixelFormat = PF_RGBA8;
}


MetaTexDataInfo::~MetaTexDataInfo() {
    for (TexMipMapList::iterator iter = data.begin(); iter != data.end(); ++iter) {
        TexDataToSizeMap& m = *iter;
        if (m.first) {
            delete [] m.first;
        }
    }
    data.clear();
}


void PngSerializer::importTexture(FileDataStream& stream, Texture* texture) {
    string& path = stream.getFileName();
    texture->setPath(path);

    MetaTexDataInfo* info = PlatformImageUtil::loadPng(path);
    if (info) {
        texture->loadTexImage(info);
        delete info;
    } else {
        LOGE("%d [PngSerializer importTexture] Error in loadPng. MetaTexDataInfo is empty.", getThreadID());
    }
}


void PvrSerializer::importTexture(FileDataStream& stream, Texture* texture) {
    string& path = stream.getFileName();
    texture->setPath(path);

    MetaTexDataInfo* info = PlatformImageUtil::loadCompressedTexData(path);
    if (info) {
        texture->loadTexImage(info);
        delete info;
    } else {
        LOGE("%d [PvrSerializer importTexture] Error in loadPng. MetaTexDataInfo is empty.", getThreadID());
    }
}
