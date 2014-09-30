//
//  Renderable.h
//
//  Created by Jingshu Huang on 3/1/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Renderable_h
#define _Renderable_h

#include "Material.h"
#include "matrix4.h"
#include "TextureManager.h"


class IndexVBO;
class Material;
class Mesh;
class RenderState;
class VertexVBO;


// Abstracts renderables are queued in render queue. They are grouped by
// material. Classes implementing this interface must be based on a single
// material, a single world matrix (or a collection of world matrices which are
// blended by weights), and must be renderable via a single render operation.
// Deciding whether to put these objects in the rendering pipeline is done from
// the more specific classes e.g. sub-entities.
class Renderable {
public:
    Renderable() {}
    virtual ~Renderable() {}

    virtual Mesh* getMesh() const = 0;
    virtual VertexVBO* getVertexVbo() const = 0;
    virtual IndexVBO* getIndexVbo() const = 0;
    virtual int getNumVertices() const = 0;
    virtual int getNumIndices() const = 0;
    virtual int getIndexFormat() const = 0;

    virtual Material* getMaterial() const = 0;
    // Renderable textures can override the textures specified in the material.
    virtual TexUnitToTextureMap* getTextureMap() const { return NULL; }
    virtual GLenum getRenderMode() const { return GL_TRIANGLES; }
    virtual RenderState* getRenderState() const { return NULL; }

    virtual ShadingTechnique* getShadingTechnique(void) const {
        return getMaterial()->getBestTechnique();
    }

    virtual const Matrix4& getWorldTransform() const = 0;
};

#endif
