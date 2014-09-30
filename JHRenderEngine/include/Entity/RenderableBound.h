//
//  RenderableBound.h
//
//  Created by Jingshu Huang on 8/23/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __RenderableBound__
#define __RenderableBound__

#include "Mesh.h"
#include "Renderable.h"
#include "RenderState.h"
#include "SubMesh.h"

class Bound;


class RenderableBound : public Renderable {
public:
    RenderableBound();
    ~RenderableBound();

    void setupMaterial(const string& name);
    void setupMeshWithBound(const string& meshName, Bound& bound);

    inline Mesh* getMesh() const { return mMesh; }
    inline VertexVBO* getVertexVbo() const { assert(mMesh); return mMesh->getVertexVbo(); }
    inline IndexVBO* getIndexVbo() const { assert(mSubMesh); return mSubMesh->getIndexVbo(); }
    inline int getNumVertices() const { assert(mSubMesh); return mMesh->getNumVertices(); }
    inline int getNumIndices() const { assert(mSubMesh); return mSubMesh->getNumIndices(); }
    inline int getIndexFormat() const { assert(mSubMesh); return mSubMesh->getIndexFormat(); }

    inline Material* getMaterial() const { return mMaterial; }
    inline GLenum getRenderMode() const { return GL_TRIANGLES; }
    inline RenderState* getRenderState() const { return &(RenderState::kDefaultRenderState); }

    inline const Matrix4& getWorldTransform() const { return Matrix4::IDENTITY; }

    static string kDefaultBoundMaterial;

protected:
    Mesh* mMesh;          // Does not own the data.
    SubMesh* mSubMesh;    // Does not own the data.
    Material* mMaterial;  // Does not own the data.
};

#endif
