//
//  platformPathUtil.h
//
//  Created by Jingshu Huang on 2/6/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __PLATFORMPATHUTIL_H__
#define __PLATFORMPATHUTIL_H__

#include <string>

using namespace std;


class PlatformPathUtil {
public:
    // For iOS, this is an absolute path of the bundle. For Android, this is a
    // path relative to the bundle location.
    static string assetPath;
    
    static string sceneSubPath;
    static string modelSubPath;
    static string imageSubPath;
    static string soundSubPath;
    static string materialSubPath;
    static string shaderSubPath;
    
private:
    static string platformBundlePath(void);
    static string platformAssetPath(void);
    static string platformSceneSubPath(void);
    static string platformModelSubPath(void);
    static string platformImageSubPath(void);
    static string platformSoundSubPath(void);
    static string platformMaterialSubPath(void);
    static string platformShaderSubPath(void);

    static string platformCachePath(void);
//    static string documentsDirectory(void);
};

#endif
