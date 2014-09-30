//
//  platformUtil.h
//
//  Created by Jingshu Huang on 2/6/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __PLATFORMUTIL_H__
#define __PLATFORMUTIL_H__

#include "platformDef.h"


class PlatformUtil {
public:
    static bool isZipped(const string& fileName);
    static double absoluteTimeGetCurrent(void);
};

#endif
