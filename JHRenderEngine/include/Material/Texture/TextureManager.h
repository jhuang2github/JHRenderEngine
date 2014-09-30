//
//  TextureManager.h
//
//  Created by Jingshu Huang on 12/20/12.
//  Copyright (c) HuangImage 2012. All rights reserved.
//

#ifndef _TextureManager_h
#define _TextureManager_h

#include "platformDef.h"
#include "AssetManager.h"
#include "Singleton.h"
#include "Texture.h"


class TextureManager : public Singleton<TextureManager>, public AssetManager<string, Texture> {
};

typedef unordered_map<int, Texture*> TexUnitToTextureMap;

#endif
