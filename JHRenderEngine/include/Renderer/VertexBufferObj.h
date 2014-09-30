//
//  VertexBufferObj.h
//
//  Created by Jingshu Huang on 12/20/12.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef VertexBufferObj_h
#define VertexBufferObj_h

#include "platformDef.h"
#include "platformGL.h"


class Mesh;
class SubMesh;


class VertexBufferObject {
public:
    VertexBufferObject() { mVboID = 0; }
    virtual ~VertexBufferObject() { mVboID = 0; }
    inline GLuint getVboID() { return mVboID; }

protected:
    virtual void cleanup();

protected:
    GLuint mVboID;
};


class VertexVBO : public VertexBufferObject {
public:
    VertexVBO();
    VertexVBO(Mesh* mesh, long mode=GL_STATIC_DRAW);
    ~VertexVBO();

    void sendData(Mesh* mesh, long mode=GL_STATIC_DRAW);
};


class IndexVBO : public VertexBufferObject {
public:
    IndexVBO();
    IndexVBO(SubMesh* subMesh, long mode=GL_STATIC_DRAW);
    ~IndexVBO();

    void sendData(SubMesh* subMesh, long mode=GL_STATIC_DRAW);
};

#endif
