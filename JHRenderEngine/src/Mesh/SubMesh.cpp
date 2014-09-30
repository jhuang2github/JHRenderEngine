//
//  SubMesh.cpp
//
//  Created by Jingshu Huang on 1/18/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "Mesh.h"
#include "SubMesh.h"
#include "VertexBufferObj.h"


unordered_map<int, int> SubMesh::kIndexFormatSize = SubMesh::createIndexFormatSizeMap();


SubMesh::SubMesh(Mesh* _parent) {
    mParent = _parent;
    mIndexFormat = GL_UNSIGNED_SHORT;
    mIndices = NULL;
    mNumIndices = 0;
    mVbo = NULL;
}

SubMesh::~SubMesh() {
    mParent = NULL;
    if (mIndices) {
        delete reinterpret_cast<char*>(mIndices);
        mIndices = NULL;
    }
    mNumIndices = 0;

    if (mVbo) {
        delete mVbo;
    }
    mVbo = NULL;
}

unordered_map<int, int> SubMesh::createIndexFormatSizeMap() {
    unordered_map<int, int> sizeMap;
    sizeMap[GL_UNSIGNED_SHORT] = sizeof(ushort);
    sizeMap[GL_UNSIGNED_BYTE] = sizeof(byte);
    sizeMap[GL_UNSIGNED_INT] = sizeof(uint);
    return sizeMap;
}

void SubMesh::ensureIndexCapacity(size_t byteSize) {
    assert(mIndices == NULL);
    mIndices = malloc(byteSize);
}

int SubMesh::getIndicesSizeInByte() {
    return mNumIndices * kIndexFormatSize[mIndexFormat];
}

void SubMesh::onLoadComplete() {
    if (!mVbo) {
        mVbo = new IndexVBO(this);
    } else {
        mVbo->sendData(this);
    }
}
