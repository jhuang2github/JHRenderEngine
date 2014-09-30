//
//  MeshManager.cpp
//
//  Created by Jingshu Huang on 1/20/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "MeshManager.h"
#include "shape.h"


Mesh* MeshManager::loadFromShape(const string &meshName, Shape& shape) {
    Mesh* t = NULL;
    AssetMapIterator iter = assets.find(meshName);
    if (iter != assets.end()) {
        return iter->second;
    }

    t = new Mesh(meshName);
    t->loadFromShape(shape);
    assets[meshName] = t;
    return t;
}
