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

#include "platformUtil.h"


double PlatformUtil::absoluteTimeGetCurrent(void) {
#ifdef __APPLE__
    //    if (cfAbsoluteTimeRefDateInvalid) {
    //        return CFAbsoluteTimeGetCurrent() + kCFAbsoluteTimeIntervalSince1970;
    //    } else {
    return CFAbsoluteTimeGetCurrent();
    //    }
#else
    return 1.0; //TODO(jhuang): fix this.
#endif
}


bool PlatformUtil::isZipped(const string& fileName) {
#ifdef __APPLE__
    return false;
#endif
    return true; // TODO(jhuang): fix this for android.
}
