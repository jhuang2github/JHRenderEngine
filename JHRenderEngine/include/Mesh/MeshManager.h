//
//  MeshManager.h
//
//  Created by Jingshu Huang on 1/18/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _MeshManager_h
#define _MeshManager_h

#include "AssetManager.h"
#include "Mesh.h"
#include "platformDef.h"
#include "Singleton.h"

class Shape;


class MeshManager : public Singleton<MeshManager>, public AssetManager<string, Mesh> {
public:
    Mesh* loadFromShape(const string& meshName, Shape& shape);
};

#endif
