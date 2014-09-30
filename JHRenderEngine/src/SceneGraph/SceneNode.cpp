//
//  SceneNode.cpp
//
//  Created by Jingshu Huang on 1/21/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "bound.h"
#include "Camera.h"
#include "DataStream.h"
#include "platformUtil.h"
#include "platformPathUtil.h"
#include "quaternion.h"
#include "RenderableBound.h"
#include "Renderer.h"
#include "RenderQueue.h"
#include "SceneMananger.h"
#include "SceneNode.h"
#include "SceneObj.h"
#include "SceneSerializer.h"
#include "SerializerManager.h"
#include "vector3.h"


// Factory method.
SceneNode* SceneNode::create(SceneManager* mgr, const string& nodeNameSuffix) {
    SceneNode* node = new SceneNode(mgr, kDefaultSceneNodeClass, nodeNameSuffix);
    node->translate(Vector3::ZERO);
    node->rotate(Quaternion::IDENTITY);
    node->scale(Vector3::UNIT_SCALE);
    return node;
}

SceneNode::SceneNode(SceneManager* manager, const NodeType& type,
                     const string& nodeNameSuffix)
    : Node(type, nodeNameSuffix) {
    mSceneMgr = manager;
    mObjects.clear();
    mPath.clear();
    mVisibleCameras.clear();
    mHidden = false;
    mShowBound = false;
    mRenderableWorldBound = NULL;
}

SceneNode::~SceneNode() {
    mSceneMgr = NULL;
    if (mRenderableWorldBound) {
        delete mRenderableWorldBound;
    }
    mRenderableWorldBound = NULL;

    reset();
}

void SceneNode::reset() {
    mObjects.clear();
    mPath.clear();
    mVisibleCameras.clear();
    mHidden = false;
    mShowBound = false;
    Node::reset();
}

void SceneNode::deepCopyFrom(Node* other_) {
    assert(other_ != NULL);
    Node::deepCopyFrom(other_);

    SceneNode* other = static_cast<SceneNode*>(other_);

    mWorldBound = other->mWorldBound;
    mRenderableWorldBound = other->mRenderableWorldBound;
    mShowBound = other->mShowBound;

    mSceneMgr = other->mSceneMgr;
    mPath = other->mPath;
    mHidden = other->mHidden;
    mNodeType = other->mNodeType;

    // Assuming the visible camera is defined inside the scene file so we can
    // safely copy it over.
    mVisibleCameras = other->mVisibleCameras;

    attachedObjectsCopyImpl(other);
}

SceneNode* SceneNode::createChild() {
    return createChild(kDefaultSceneNodeClass, "");
}

SceneNode* SceneNode::createChild(const string& nodeClass, const string& nameSuffix) {
    return mSceneMgr->createChildSceneNode(nodeClass, this, nameSuffix);
}

void SceneNode::nodeChildrenCopyImpl(Node* other) {
    NodeMap& otherChildren = other->getChildren();
    for(NodeMap::const_iterator iter = otherChildren.begin(); iter != otherChildren.end();
        ++iter) {
        SceneNode* child = mSceneMgr->createChildSceneNode(iter->second->getNodeType(), this, "");
        child->deepCopyFrom(iter->second);
    }
}

void SceneNode::attachedObjectsCopyImpl(SceneNode* other) {
    SceneObjectMap& otherObjs = other->getAttachedObjects();
    for (SceneObjectMap::const_iterator iter = otherObjs.begin(); iter != otherObjs.end();
         ++iter) {
        string name = iter->second->getName();
        SceneObj* obj = mSceneMgr->createSceneObj(name,
                                                  iter->second->getType(),
                                                  NULL);
        obj->deepCopyFrom(iter->second);
        assert(obj->getName() == name);
        assert(getAttachedObject(name) == NULL);
        mObjects[name] = obj;
    }
}

bool SceneNode::load(const string& fileName) {
    // First unload the previous scene.
    if (!mPath.empty()) {
        reset();
    }

    mPath = PlatformPathUtil::assetPath + "/" + PlatformPathUtil::sceneSubPath + "/" + fileName;
    LOGD("tid=%d [SceneNode load] load scene = %s", getThreadID(), fileName.c_str());
    return load();
}

bool SceneNode::load() {
    SceneSerializer* serializer = SerializerManager::getInstance()->getSceneSerializer(mPath);
    if (PlatformUtil::isZipped(mPath)) {
        ZipFileDataStream data(mPath);
        serializer->importScene(data, this);
    } else {
        FileDataStream data(mPath);
        serializer->importScene(data, this);
    }
    return true;
}

void SceneNode::attachObject(SceneObj* obj) {
    mObjects[obj->getName()] = obj;
    obj->setSceneNode(this);
    needUpdate();
}

ushort SceneNode::numAttachedObjects() const {
    return mObjects.size();
}

SceneObj* SceneNode::getAttachedObject(const string& name) {
    SceneObjectMap::iterator iter = mObjects.find(name);
    if (iter != mObjects.end()) {
        return iter->second;
    }
    return NULL;
}

void SceneNode::detachObject(const string& name) {
    SceneObjectMap::iterator iter = mObjects.find(name);
    if (iter != mObjects.end()) {
        iter->second->setSceneNode(NULL);
        mObjects.erase(iter);
        needUpdate();
    }
}

void SceneNode::updateOnNotification(Observable* observable, Notification* msg) {
    for (SceneObjectMap::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it) {
        if (it->second) {
            it->second->updateOnNotificationImpl(observable, msg);
        }
    }
}

void SceneNode::update(const float secondsPassed) {
    Node::update(secondsPassed);
    updateBounds();
}

void SceneNode::updateFromParent(void) const {
    Node::updateFromParent();
}

void SceneNode::updateBounds(void) {
    mWorldBound.reset();

    // Update bounds from attached scene objects. Merge world bounds of each scene object.
    for (SceneObjectMap::iterator iter = mObjects.begin();
         iter != mObjects.end(); ++iter) {
        mWorldBound.merge(iter->second->getWorldBound(true));
    }

    // Merge with the children nodes' world bounds.
    for (NodeMap::iterator iter = mChildren.begin();
         iter != mChildren.end(); ++iter) {
        mWorldBound.merge(static_cast<SceneNode*>(iter->second)->mWorldBound);
    }
}

void SceneNode::addBoundToRenderQueue() {
    if (!mRenderableWorldBound) {
        mRenderableWorldBound = new RenderableBound;
    }

    mRenderableWorldBound->setupMeshWithBound(getBoundMeshName(), mWorldBound);
    RenderQueue* queue = mSceneMgr->getRenderer()->getRenderQueue(QUEUE_10_OVERLAY);
    assert(queue != NULL && "[SceneNode addBoundToRenderQueue] render queue should not be empty.");
    queue->addRenderable(mRenderableWorldBound);
}

string SceneNode::getBoundMeshName() {
    ostringstream os;
    os << "SceneNode_" << mNodeID << "_Bound_Mesh";
    return os.str();
}

bool SceneNode::isVisible(Camera* cam) {
    if (isHidden()) {
        return false;
    }

    // Check if the visiblity camera set is set. If it is set, check if the
    // camera is in this visible set or not. If it is not set, this node inherits
    // the visiblity camera set from its parent. If it is root node and the set
    // is empty, the node can be seen by all cameras.
    if (!mVisibleCameras.empty() &&
        mVisibleCameras.find(cam->getName()) == mVisibleCameras.end()) {
        return false;
    }
    
    // Check if the node is visible.
    return cam->isVisible(mWorldBound.getAABB());
}

void SceneNode::findVisibleObjects(Camera* cam) {
    if (isHidden() || !isVisible(cam)) {
        return;
    }

    // Add all entities attached to this node to their render queues.
    for (SceneObjectMap::const_iterator iter = mObjects.begin();
         iter != mObjects.end(); ++iter) {
        SceneObj* obj = iter->second;
        obj->getRenderQueue()->addVisibleObject(obj);
    }

    // Check child nodes.
    for (NodeMap::const_iterator child = mChildren.begin();
         child != mChildren.end(); ++child) {
        static_cast<SceneNode*>(child->second)->findVisibleObjects(cam);
    }

    // Check if we need to add the bound renderable associated with the node.
    if (mShowBound) {
        addBoundToRenderQueue();
    }
}


