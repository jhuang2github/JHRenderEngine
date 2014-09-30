//
//  triangle.h
//
//  Created by Jingshu Huang on 5/14/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __triangle__
#define __triangle__

#include "shape.h"
#include "vector3.h"


class Triangle : public Shape {
public:
    Triangle() { vertices[0] = vertices[1] = vertices[2] = Vector3::ZERO; }
    ~Triangle();

private:
    Vector3 vertices[3];
};

#endif
