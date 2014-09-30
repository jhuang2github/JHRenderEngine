//
//  RenderableBound.cpp
//
//  Created by Jingshu Huang on 8/23/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "bound.h"
#include "MaterialManager.h"
#include "Mesh.h"
#include "MeshManager.h"
#include "RenderableBound.h"


//string RenderableBound::kDefaultBoundMaterial = "default.material";
string RenderableBound::kDefaultBoundMaterial = "bound.material";;


RenderableBound::RenderableBound() : Renderable() {
    mMesh = NULL;
    mSubMesh = NULL;
    mMaterial = NULL;
    mMaterial = NULL;
    setupMaterial(kDefaultBoundMaterial);
}

RenderableBound::~RenderableBound() {
    // Mesh data is owned by MeshManager.
    mMesh = NULL;
    mSubMesh = NULL;
    mMaterial = NULL;
}

void RenderableBound::setupMaterial(const string& name) {
    mMaterial = MaterialManager::getInstance()->getAsset(name);
    if(!mMaterial) {
        mMaterial = MaterialManager::getInstance()->load(name);
    }
    assert(mMaterial != NULL);
}

// TODO(jhuang). BUG(jhuang): this has memory issue.
void RenderableBound::setupMeshWithBound(const string& meshName, Bound& bound) {
    mMesh = MeshManager::getInstance()->getAsset(meshName);
    if (!mMesh) {
        mMesh = MeshManager::getInstance()->loadFromShape(meshName, bound);
        SubMeshList& submeshes = mMesh->getSubMeshes();
        assert(submeshes.size() == 1);
        mSubMesh = submeshes[0];
        assert(mSubMesh != NULL && mSubMesh->getNumIndices() == 36);
    } else {
        bound.updateMesh(*mMesh);
        assert(mSubMesh->getNumIndices() == 36);
    }
}
