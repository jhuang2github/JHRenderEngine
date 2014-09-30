//
//  VertexBufferObj.cpp
//
//  Created by Jingshu Huang on 12/20/12.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "GLUtil.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "VertexBufferObj.h"


void VertexBufferObject::cleanup() {
    if (mVboID) {
		glDeleteBuffers(1, &mVboID);
    }
	mVboID = 0;
}


#pragma vertex vbo

VertexVBO::VertexVBO() {
    GL_genBuffers(1, &mVboID);
	assert(mVboID != 0);
    GL_bindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexVBO::VertexVBO(Mesh* mesh, long mode) {
    GL_genBuffers(1, &mVboID);
	assert(mVboID != 0);
    sendData(mesh, mode);
}

VertexVBO::~VertexVBO() {
    cleanup();
}

void VertexVBO::sendData(Mesh* mesh, long mode) {
	assert(mVboID != 0);
    GL_bindBuffer(GL_ARRAY_BUFFER, mVboID);
    GL_bufferData(GL_ARRAY_BUFFER, mesh->getVerticesSizeInByte(),
                 mesh->getVertices(), mode);
    GL_bindBuffer(GL_ARRAY_BUFFER, 0);
}

#pragma Index vbo

IndexVBO::IndexVBO() {
    GL_genBuffers(1, &mVboID);
	assert(mVboID != 0);
	GL_bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexVBO::IndexVBO(SubMesh* subMesh, long mode) {
    GL_genBuffers(1, &mVboID);
	assert(mVboID != 0);
    sendData(subMesh, mode);
}

IndexVBO::~IndexVBO() {
    cleanup();
}

void IndexVBO::sendData(SubMesh* subMesh, long mode) {
	assert(mVboID != 0);
    GL_bindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboID);
    GL_bufferData(GL_ELEMENT_ARRAY_BUFFER, subMesh->getIndicesSizeInByte(),
                 subMesh->getIndices(), mode);
	GL_bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
