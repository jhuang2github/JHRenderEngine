//
//  PVRTexture.cpp
//
//  Created by Jingshu Huang on 5/6/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "PVRTexture.h"


#define PVR_TEXTURE_FLAG_TYPE_MASK	0xff
//static char gPVRTexIdentifier[] = "PVR!";

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
