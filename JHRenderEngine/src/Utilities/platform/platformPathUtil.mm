//
//  platformPathUtil.cc
//
//  Created by Jingshu Huang on 2/6/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/Foundation.h>
#endif

#include "PlatformPathUtil.h"

string PlatformPathUtil::assetPath = platformAssetPath();

string PlatformPathUtil::sceneSubPath = platformSceneSubPath();
string PlatformPathUtil::modelSubPath = platformModelSubPath();
string PlatformPathUtil::imageSubPath = platformImageSubPath();
string PlatformPathUtil::soundSubPath = platformSoundSubPath();
string PlatformPathUtil::materialSubPath = platformMaterialSubPath();
string PlatformPathUtil::shaderSubPath = platformShaderSubPath();


std::string PlatformPathUtil::platformBundlePath(void) {
    return platformAssetPath();
}


std::string PlatformPathUtil::platformAssetPath(void) {
#ifdef __APPLE__
    char path[PATH_MAX];

    // NSBundle* mainBundle = [NSBundle mainBundle];
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    assert(mainBundle);
    
    CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
    assert(mainBundleURL);
    
    CFStringRef cfStringRef = CFURLCopyFileSystemPath(mainBundleURL, kCFURLPOSIXPathStyle);
    assert(cfStringRef);
    
    CFStringGetCString(cfStringRef, path, PATH_MAX, kCFStringEncodingASCII);
    
    CFRelease(mainBundleURL);
    CFRelease(cfStringRef);

    return std::string(path);

#endif

    return "assets";
}


string PlatformPathUtil::platformSceneSubPath(void) {
#ifdef __APPLE__
    return ".";
#endif
    return "scenes";
}


string PlatformPathUtil::platformModelSubPath(void) {
#ifdef __APPLE__
    return ".";
#endif
    return "models";
}


string PlatformPathUtil::platformImageSubPath(void) {
#ifdef __APPLE__
    return ".";
#endif
    return "images";
}


string PlatformPathUtil::platformSoundSubPath(void) {
#ifdef __APPLE__
    return ".";
#endif
    return "sounds";
}


string PlatformPathUtil::platformMaterialSubPath(void) {
#ifdef __APPLE__
    return ".";
#endif
    return "materials";
}


string PlatformPathUtil::platformShaderSubPath(void) {
#ifdef __APPLE__
    return ".";
#endif
    return "shaders";
}


std::string PlatformPathUtil::platformCachePath(void) {
#ifdef __APPLE__
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *cachesDirectory = [paths objectAtIndex:0];
    
    return [[cachesDirectory stringByAppendingString:@"/"] cStringUsingEncoding:NSASCIIStringEncoding];
#else
    return "";
#endif
}


//std::string PlatformPathUtil::documentsDirectory(void) {
//#ifdef __APPLE__
//    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
//    NSString *documentsDirectory = [paths objectAtIndex:0];
//    
//    return std::string([documentsDirectory cStringUsingEncoding:NSASCIIStringEncoding]);
//#else
//    return "";
//#endif
//}


//ResourceReader *getReaderForPath(const char *path, bool isBundled) {
//	bool useSdCard = (path != NULL && strstr(path, "/mnt/sdcard") != NULL);
//	const char *ASSET_DIR = "assets/";
//	if (strlen(path) > 0 && path[0] == '/') {
//		// If we are given an absolute path, just respect it
//		ASSET_DIR = "";
//	}
//	char *bundledPath = (char *)malloc(strlen(ASSET_DIR) + strlen(path) + 1);
//    
//	strncpy(bundledPath, ASSET_DIR, strlen(ASSET_DIR)+1);
//	strcat(bundledPath, path);
//	LOGD("getReaderForPath: %s", bundledPath);
//	ResourceReader *reader;
//    
//	if (useSdCard || !isBundled) {
//		reader = new FileReader(bundledPath);
//	} else {
//		reader = new ApkReader(bundledPath);
//	}
//    
//	free(bundledPath);
//	return reader;
//}

