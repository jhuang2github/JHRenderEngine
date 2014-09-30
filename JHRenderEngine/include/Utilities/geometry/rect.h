//
//  rect.h
//
//  Created by Jingshu Huang on 3/14/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __rect__
#define __rect__

#include "shape.h"
#include "vector2.h"


namespace JHEngine {

// Rect has origin at the top left corner.
class Rect : public Shape {
public:
    Rect() : x(0), y(0), width(0), height(0) {}
    Rect(float x_, float y_, float w_, float h_) : x(x_), y(y_), width(w_), height(h_) {}
    ~Rect() {}

    // |point| is in screen coords (pixel) with the origin at the top left corner.
    inline bool containsPoint(const Vector2& point) const {
        return point.x >= x && point.x <= x + width &&
               point.y >= y && point.y <= y + height;
    }

public:
    float x, y;
    float width, height;
};

}

#endif
