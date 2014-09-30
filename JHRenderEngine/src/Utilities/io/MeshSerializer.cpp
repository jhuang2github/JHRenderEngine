//
//  MeshSerializer.cpp
//
//  Created by Jingshu Huang on 1/17/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include <cfloat>
#include <cstdio>
#include <string.h>
#include <fcntl.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#include "bound.h"
#include "DataStream.h"
#include "Mesh.h"
#include "MeshSerializer.h"
#include "SubMesh.h"
#include "vector3.h"


void ObjSerializer::importMesh(DataStream& stream, Mesh* mesh) {
    // TODO(jhuang):
}
