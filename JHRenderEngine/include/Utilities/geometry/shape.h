//
//  shape.h
//
//  Created by Jingshu Huang on 8/27/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __shape__
#define __shape__


class Mesh;


class Shape {
public:
    Shape() {}
    virtual ~Shape() {}

    virtual void loadIntoMesh(Mesh& mesh) {}
    virtual void updateMesh(Mesh& mesh) {}
};

#endif
