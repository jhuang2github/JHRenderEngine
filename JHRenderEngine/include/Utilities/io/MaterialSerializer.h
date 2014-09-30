//
//  MaterialSerializer.h
//
//  Created by Jingshu Huang on 2/8/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _MaterialSerializer_h
#define _MaterialSerializer_h

#include "Serializer.h"


class DataStream;
class Material;


class MaterialSerializer : public Serializer {
public:
    virtual ~MaterialSerializer() {}
    virtual void importMaterial(DataStream&, Material*) = 0;
};


#pragma material serializer implementations

class MatSerializer : public MaterialSerializer {
public:
	MatSerializer() {
        mVersion = "v1_0";
    }
    ~MatSerializer() {
    }
    
    void importMaterial(DataStream&, Material*);

private:
    void importMaterialDOM(DataStream& stream, Material* material);
    void importMaterialSAX(DataStream& stream, Material* material);
};

#endif
