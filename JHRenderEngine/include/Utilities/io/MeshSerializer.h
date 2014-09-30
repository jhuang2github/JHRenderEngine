//
//  MeshSerializer.h
//
//  Created by Jingshu Huang on 1/17/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _MeshSerializer_h
#define _MeshSerializer_h

#include "platformDef.h"
#include "Serializer.h"


class DataStream;
class Mesh;


class MeshSerializer : public Serializer {
public:
    virtual ~MeshSerializer() {}
    virtual void importMesh(DataStream&, Mesh*) = 0;
};


#pragma mesh serializer implementations

class ObjSerializer : public MeshSerializer {
public:
	ObjSerializer() {
        mVersion = "v1_0";
    }
    ~ObjSerializer() {
    }

    void importMesh(DataStream&, Mesh*);
};

#endif
