//
//  SubEntity.h
//
//  Created by Jingshu Huang on 1/18/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _SubEntity_h
#define _SubEntity_h

#include "platformDef.h"
#include "Renderable.h"
#include "SubMesh.h"


class Entity;
class Material;


// Just like a mesh may contains multiple submeshes. An entity may contain
// multiple subentities, each of which has references to a submesh.
class SubEntity : public Renderable {
public:
    SubEntity(Entity* parent, SubMesh* subMesh);
    virtual ~SubEntity();

    virtual Mesh* getMesh() const;
    virtual VertexVBO* getVertexVbo() const;
    virtual IndexVBO* getIndexVbo() const;

    void setMaterial(const string& name);
    void setMaterial(Material* material);
    virtual Material* getMaterial() const { return mMaterial; }

    void setTextures(const vector<string>& textureNames);
    inline TexUnitToTextureMap* getTextureMap() const { return mTexUnitToTextureMap; }

    void setRenderState(const RenderState& state);
    inline RenderState* getRenderState() const { return mRenderState; }

    virtual int getNumIndices() const {
        if (mSubMesh) {
            return mSubMesh->getNumIndices();
        }
        return 0;
    }

    virtual int getNumVertices() const;
    virtual int getIndexFormat() const {
        return mSubMesh->getIndexFormat();
    }

    const Matrix4& getWorldTransform() const;

    virtual bool isVisible(void) const { return mVisible; }
    inline void setVisible(const bool v) { mVisible = v; }

private:
    Entity*   mParent;
    SubMesh*  mSubMesh;

    Material* mMaterial;
    // Renderable can override the textures defined in the material. It does not
    // own the texture data.
    TexUnitToTextureMap* mTexUnitToTextureMap;
    // Renderable can override the render state defined in a pass.
    RenderState* mRenderState;

    bool      mVisible;

//    void getWorldTransforms(Matrix4* txf) const;
//    ShadingTechnique* getShadingTechnique() const;
//    // The render queue to use when rendering this renderable
//    uint8 mRenderQueueID;
};

#endif
