//
//  Entity.cpp
//
//  Created by Jingshu Huang on 1/18/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "Entity.h"
#include "Mesh.h"
#include "MeshManager.h"
#include "RenderQueue.h"
#include "SceneMananger.h"
#include "SubEntity.h"

string Entity::kRenderQueueIDKey = "renderQueueID";
string Entity::kMeshKey = "mesh";

bool Entity::sFactoryInitialized = registerFactory();


bool Entity::registerFactory() {
    SceneManager::getInstance()->registerSceneObjFactory(kDefaultEntityClass,
            new Entity("EntityFactory", kDefaultEntityClass));
    return true;
}

// Factory method.
SceneObj* Entity::create(SceneManager* sceneMgr,
                         const string& entityName,
                         const KeyValuePairStringMap* params) {
    Mesh* mesh = NULL;
    ERenderQueueID queueID = QUEUE_5_MAIN;
    parseParams(params, &mesh, queueID);
    return new Entity(entityName, kDefaultEntityClass, mesh, sceneMgr, queueID);
}

Entity::Entity(const string& name, const ESceneObjType& type) : SceneObj(name, type) {
    mMesh = NULL;
}

Entity::Entity(const string& name, const ESceneObjType& objType, Mesh* mesh,
               SceneManager* sceneMgr, const ERenderQueueID& queueID) :
    SceneObj(name, objType, sceneMgr, queueID) {
    mMesh = mesh;
    if (mMesh) {
        buildSubEntities();
    }
}

Entity::~Entity() {
    for (vector<SubEntity*>::iterator iter = mSubEntities.begin();
         iter != mSubEntities.end(); ++iter) {
        delete *iter;
    }
    mSubEntities.clear();
}

void Entity::parseParams(const KeyValuePairStringMap* params,
                         Mesh** mesh, ERenderQueueID& queueID) {
    if(!params || !mesh) {
        return;
    }

    KeyValuePairStringMap::const_iterator iter = params->find(Entity::kMeshKey);
    if (iter != params->end()) {
        // Load the mesh.
        *mesh = MeshManager::getInstance()->load(iter->second);
    }
    iter = params->find(Entity::kRenderQueueIDKey);
    if (iter != params->end()) {
        stringstream stream(iter->second);
        uint qID = queueID;
        stream >> qID;
        queueID = (ERenderQueueID)qID;
    }

    if (!*mesh) {
        LOGD("EntityFactory::create(): Need to specify mesh when constructing an entity.");
    }
}

void Entity::deepCopyFrom(SceneObj* other_) {
    mMesh = (static_cast<Entity*>(other_))->mMesh;
    buildSubEntities();
    SceneObj::deepCopyFrom(other_);
}

void Entity::buildSubEntities() {
    assert(mMesh != NULL);
    SubMeshList& subMeshes = mMesh->getSubMeshes();
    for (SubMeshList::const_iterator iter = subMeshes.begin();
         iter != subMeshes.end(); ++iter) {
        SubMesh* subMesh = *iter;
        SubEntity* subEntity = new SubEntity(this, subMesh);
        mSubEntities.push_back(subEntity);
    }
}

void Entity::setMaterial(const string &name) {
    for (SubEntityList::const_iterator iter = mSubEntities.begin();
         iter != mSubEntities.end(); ++iter) {
        (*iter)->setMaterial(name);
    }
}

void Entity::setMaterial(Material* material) {
    for (SubEntityList::const_iterator iter = mSubEntities.begin();
         iter != mSubEntities.end(); ++iter) {
        (*iter)->setMaterial(material);
    }
}

void Entity::setTextures(const vector<string> &textureNames) {
    assert(mMesh != NULL);
    for (SubEntityList::const_iterator iter = mSubEntities.begin();
         iter != mSubEntities.end(); ++iter) {
        (*iter)->setTextures(textureNames);
    }
}

void Entity::setRenderState(const RenderState &state) {
    for (SubEntityList::const_iterator iter = mSubEntities.begin();
         iter != mSubEntities.end(); ++iter) {
        (*iter)->setRenderState(state);
    }
}

int Entity::getNumVertices() const {
    if (mMesh) {
        return mMesh->getNumVertices();
    }
    return 0;
}

// Get an entity bound from a mesh.
const Bound& Entity::getObjectBound(void) const {
    if (mMesh->isLoaded()) {
        mObjBound = mMesh->getObjectBound();  // get a mesh local bounds.
    } else {
        mObjBound.reset();
    }

    return mObjBound;
}

void Entity::updateRenderQueue(RenderQueue* queue) {
    for (SubEntityList::const_iterator iter = mSubEntities.begin();
         iter != mSubEntities.end(); ++iter) {
        SubEntity* sub = *iter;
        if (sub->isVisible()) {
            queue->addRenderable(sub);
        }
    }
}
