//
//  SubEntity.cpp
//
//  Created by Jingshu Huang on 1/18/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "Entity.h"
#include "Material.h"
#include "matrix4.h"
#include "Mesh.h"
#include "SubEntity.h"
#include "SubMesh.h"
#include "MaterialManager.h"


SubEntity::SubEntity(Entity* parent, SubMesh* subMesh) : Renderable(),
        mParent(parent), mSubMesh(subMesh) {
    mMaterial = NULL;
    mVisible = true;
    mTexUnitToTextureMap = NULL;
    mRenderState = NULL;
}

SubEntity::~SubEntity() {
    mParent = NULL;
    mMaterial = NULL;
    mSubMesh = NULL;

    if (mTexUnitToTextureMap) {
        // Does not own texture data. So just clear up the map.
        mTexUnitToTextureMap->clear();
    }
    mTexUnitToTextureMap = NULL;

    if (mRenderState) {
        delete mRenderState;
    }
    mRenderState = NULL;
}

Mesh* SubEntity::getMesh() const {
    if (!mParent) {
        return NULL;
    }

    return mParent->getMesh();
}

int SubEntity::getNumVertices() const {
    if (mParent) {
        return mParent->getNumVertices();
    }
    return 0;
}

VertexVBO* SubEntity::getVertexVbo() const {
    if (!mParent) {
        return NULL;
    }
    
    Mesh* mesh = mParent->getMesh();
    if (!mesh) {
        return NULL;
    }
    return mesh->getVertexVbo();
}

IndexVBO* SubEntity::getIndexVbo() const {
    if (!mSubMesh) {
        return NULL;
    }
    return mSubMesh->getIndexVbo();
}

void SubEntity::setMaterial(const string &name) {
    Material* material = MaterialManager::getInstance()->getAsset(name);
    if(!material) {
        material = MaterialManager::getInstance()->load(name);
    }
    assert(material != NULL);
    setMaterial(material);
}

void SubEntity::setMaterial(Material* material) {
    assert(material != NULL);
    mMaterial = material;
//    // Make sure the material is loaded.
//    mMaterial->load();
//    // tell parent to reconsider material vertex processing options
//    mParent->reevaluateVertexProcessing();
}

void SubEntity::setTextures(const vector<string>& textureNames) {
    if (!mTexUnitToTextureMap) {
        mTexUnitToTextureMap = new TexUnitToTextureMap;
    }

    int unit = 0;
    TextureManager* texMgr = TextureManager::getInstance();
    for (vector<string>::const_iterator iter = textureNames.begin();
         iter != textureNames.end(); ++iter) {
        Texture* texture = texMgr->getAsset(*iter);
        if (!texture) {
            texture = texMgr->load(*iter);
        }
        assert(texture != NULL);
        (*mTexUnitToTextureMap)[unit] = texture;
        unit++;
    }
}

void SubEntity::setRenderState(const RenderState& state) {
    if (!mRenderState) {
        mRenderState = new RenderState;
    }
    mRenderState->copyFrom(state);
}

const Matrix4& SubEntity::getWorldTransform() const {
    return mParent->getFullWorldTransform();
}
