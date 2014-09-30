//
//  SubMesh.h
//
//  Created by Jingshu Huang on 1/18/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _SubMesh_h
#define _SubMesh_h

#include "platformDef.h"
#include "platformGL.h"

class IndexVBO;
class Mesh;

class SubMesh {
public:
    SubMesh(Mesh* _parent);
    ~SubMesh();

    inline void setIndexFormat(GLenum format) { mIndexFormat = format; }
    inline GLenum getIndexFormat() { return mIndexFormat; }
    inline void* getIndices() { return mIndices; }
    inline void setNumIndices(const int num) { mNumIndices = num; }
    inline int getNumIndices() const { return mNumIndices; }
    inline IndexVBO* getIndexVbo() { return mVbo; }
    int getIndicesSizeInByte();

    void ensureIndexCapacity(size_t byteSize);
    void onLoadComplete();

    static unordered_map<int, int> kIndexFormatSize;

private:
    static unordered_map<int, int> createIndexFormatSizeMap();

private:
    Mesh*          mParent;
    GLenum         mIndexFormat;
    void*          mIndices;  // Index stream. Owns the data.
    int            mNumIndices;

    IndexVBO*      mVbo;
};

#endif
