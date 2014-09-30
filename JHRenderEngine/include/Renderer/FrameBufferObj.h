//
//  FrameBufferObj.h
//
//  Created by Jingshu Huang on 12/20/12.
//  Copyright (c) HuangImage 2012. All rights reserved.
//

#ifndef FrameBufferObj_h
#define FrameBufferObj_h

#include "RenderTarget.h"


class FrameBufferObj : public RenderTarget {
public:
    FrameBufferObj(int w, int h);
    ~FrameBufferObj();
};

#endif
