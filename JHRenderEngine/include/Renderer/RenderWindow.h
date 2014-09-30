//
//  RenderWindow.h
//
//  Created by Jingshu Huang on 2/25/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _RenderWindow_h
#define _RenderWindow_h

#include "RenderTarget.h"
#include "vector2.h"


class SceneObj;


class RenderWindow : public RenderTarget {
public:
    RenderWindow() : RenderTarget() {}
    ~RenderWindow() {}

    virtual void update(const float);
    virtual void render(const float);

    SceneObj* getSelectedSceneObj(const Vector2& screenPoint);
};

#endif
