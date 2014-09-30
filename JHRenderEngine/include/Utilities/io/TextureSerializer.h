//
//  TextureSerializer.h
//
//  Created by Jingshu Huang on 2/11/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _TextureSerializer_h
#define _TextureSerializer_h

#include "Serializer.h"


class FileDataStream;
class Texture;


typedef pair<byte*, int> TexDataToSizeMap;
typedef vector<TexDataToSizeMap> TexMipMapList;


class MetaTexDataInfo {
public:
    int width;
    int height;
    uint internalFormat;
    int pixelFormat;

    bool isCompressed;
    TexMipMapList data;

    MetaTexDataInfo();
    ~MetaTexDataInfo();
};


class TextureSerializer : public Serializer {
public:
    virtual ~TextureSerializer() {}
    virtual void importTexture(FileDataStream&, Texture*) = 0;

};


#pragma texture serializer implementation

class PngSerializer : public TextureSerializer {
public:
    PngSerializer() {
        mVersion = "v1_0";
    }
    ~PngSerializer() {
    }

    void importTexture(FileDataStream&, Texture*);
};


class PvrSerializer : public TextureSerializer {
public:
    PvrSerializer() {
        mVersion = "v1_0";
    }
    ~PvrSerializer() {
    }
    
    void importTexture(FileDataStream&, Texture*);
};


#endif
