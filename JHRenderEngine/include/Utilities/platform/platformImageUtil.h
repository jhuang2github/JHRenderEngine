//
//  platformImageUtil.h
//
//  Created by Jingshu Huang on 2/12/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _platformImageUtil_h
#define _platformImageUtil_h

#include <string>
#include "Texture.h"
#include "TextureSerializer.h"


namespace PlatformImageUtil {

MetaTexDataInfo* loadPng(const std::string& path);
MetaTexDataInfo* loadCompressedTexData(const std::string& path);
}

#endif
