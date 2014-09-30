//
//  ZipArchive.h
//
//  Created by Jingshu Huang on 1/31/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _ZipArchive_h
#define _ZipArchive_h

#include "platformDef.h"


class ZipArchive {
public:
	static void *deflateData(void *data, uint dataSize, uint& zipSize);
	static void *inflateData(void *zipData, uint zipSize, uint& dataSize);
};

#endif
