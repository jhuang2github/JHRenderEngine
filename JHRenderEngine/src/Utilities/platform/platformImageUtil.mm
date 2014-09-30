//
//  platformImageUtil.mm
//
//  Created by Jingshu Huang on 2/12/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>
#endif

#include "DataStream.h"
#include "platformDef.h"
#include "PlatformImageUtil.h"
#include "rect.h"

using namespace JHEngine;

namespace PlatformImageUtil {

MetaTexDataInfo* loadPng(const std::string& path) {
    MetaTexDataInfo* info = new MetaTexDataInfo;

#ifdef __APPLE__
    // get a data provider referencing the relevant file
    CGDataProviderRef dataProvider = CGDataProviderCreateWithFilename(path.c_str());
    // use the data provider to get a CGImage; release the data provider
    CGImageRef image = CGImageCreateWithPNGDataProvider(dataProvider, NULL, NO, 
                                                        kCGRenderingIntentDefault);
    CGDataProviderRelease(dataProvider);

    // make a bitmap context of a suitable size to draw to, forcing decode
    int width = info->width = CGImageGetWidth(image);
    int height = info->height = CGImageGetHeight(image);
    info->internalFormat = GL_RGBA;  // default for now.
    size_t dataSize = width * height * 4;
    byte* imageBuffer = (byte*)malloc(dataSize);
    info->pixelFormat = PF_RGBA8;
    info->data.push_back(make_pair(imageBuffer, dataSize));

    CGColorSpaceRef colourSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef imageContext = CGBitmapContextCreate(
                imageBuffer, width, height, 8, width * 4, colourSpace,
                kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Little);
    CGColorSpaceRelease(colourSpace);
    // draw the image to the context, release it
    CGContextDrawImage(imageContext, CGRectMake(0, 0, width, height), image);
    CGImageRelease(image);
    
//    // now get an image ref from the context
//    CGImageRef outputImage = CGBitmapContextCreateImage(imageContext);
//    // post that off to the main thread, where you might do something like
//    // [UIImage imageWithCGImage:outputImage]
//    [self performSelectorOnMainThread:@selector(haveThisImage:) 
//                           withObject:[NSValue valueWithPointer:outputImage]
//                        waitUntilDone:YES];
//    // clean up
//    CGImageRelease(outputImage);
    CGContextRelease(imageContext);
//    free(imageBuffer);
#endif

    return info;
}


#pragma mark PVR
    
#ifdef __APPLE__

#define PVR_TEXTURE_FLAG_TYPE_MASK	0xff
static char gPVRTexIdentifier[] = "PVR!";

enum {
    kPVRTextureFlagTypePVRTC_2 = 24,
    kPVRTextureFlagTypePVRTC_4
};

typedef struct _PVRTexHeader {
    uint32_t headerLength;
    uint32_t height;
    uint32_t width;
    uint32_t numMipmaps;
    uint32_t flags;
    uint32_t dataLength;
    uint32_t bpp;
    uint32_t bitmaskRed;
    uint32_t bitmaskGreen;
    uint32_t bitmaskBlue;
    uint32_t bitmaskAlpha;
    uint32_t pvrTag;
    uint32_t numSurfs;
} PVRTexHeader;
#endif

MetaTexDataInfo* loadCompressedTexData(const std::string& path_) {
    MetaTexDataInfo* info = new MetaTexDataInfo;

#ifdef __APPLE__
    NSString* path = [NSString stringWithUTF8String:path_.c_str()];
	NSData *data = [NSData dataWithContentsOfFile:path];
	if (!data) {
		LOGD("Texture %s: failed to load PVR file!", path_.c_str());
		return NULL;
	}
	PVRTexHeader *header = (PVRTexHeader *)[data bytes];

	uint32_t pvrTag = CFSwapInt32LittleToHost(header->pvrTag);
	if (gPVRTexIdentifier[0] != ((pvrTag >>  0) & 0xff) ||
		gPVRTexIdentifier[1] != ((pvrTag >>  8) & 0xff) ||
		gPVRTexIdentifier[2] != ((pvrTag >> 16) & 0xff) ||
		gPVRTexIdentifier[3] != ((pvrTag >> 24) & 0xff)) {
		LOGD("Texture %s: bad format!", path_.c_str());
		return NULL;
	}

	uint32_t flags = CFSwapInt32LittleToHost(header->flags);
	uint32_t formatFlags = flags & PVR_TEXTURE_FLAG_TYPE_MASK;
	if (formatFlags != kPVRTextureFlagTypePVRTC_4 &
        formatFlags != kPVRTextureFlagTypePVRTC_2) {
        LOGD("Error: Texture %s format flags are wrong.\n", path_.c_str());
        return NULL;
    }

    info->internalFormat = (formatFlags == kPVRTextureFlagTypePVRTC_2) ?
                            GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG :
                            GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
    info->isCompressed = true;
    info->width = CFSwapInt32LittleToHost(header->width);
    info->height = CFSwapInt32LittleToHost(header->height);
    info->pixelFormat = PF_PVRTC_RGBA4;

//    BOOL hasAlpha = CFSwapInt32LittleToHost(header->bitmaskAlpha) ? YES : NO;
    uint32_t dataLength = CFSwapInt32LittleToHost(header->dataLength);
    byte* bytes = (byte*)[data bytes] + sizeof(PVRTexHeader);

    // Calculate the data size for each texture level and respect the minimum
    // number of blocks.
    uint32_t unpackWidth = info->width, unpackHeight = info->height, bpp = 4;
    uint32_t dataOffset = 0, dataSize = 0;
    uint32_t blockSize = 0, widthBlocks = 0, heightBlocks = 0;

    while (dataOffset < dataLength) {
        if (formatFlags == kPVRTextureFlagTypePVRTC_4) {
            blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
            widthBlocks = unpackWidth / 4;
            heightBlocks = unpackHeight / 4;
            bpp = 4;
        } else {
            blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
            widthBlocks = unpackWidth / 8;
            heightBlocks = unpackHeight / 4;
            bpp = 2;
        }
        
        // Clamp to minimum number of blocks
        if (widthBlocks < 2) {
            widthBlocks = 2;
        }
        if (heightBlocks < 2) {
            heightBlocks = 2;
        }
        dataSize = widthBlocks * heightBlocks * ((blockSize  * bpp) / 8);
        byte* data = new byte[dataSize];
        memcpy(data, bytes + dataOffset, dataSize);
        info->data.push_back(make_pair(data, dataSize));
        dataOffset += dataSize;

        unpackWidth = MAX(unpackWidth >> 1, 1);
        unpackHeight = MAX(unpackHeight >> 1, 1);
    }
#endif
    return info;
}


}  // end of namespace
