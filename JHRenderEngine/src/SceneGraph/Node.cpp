//
//  Node.cpp
//
//  Created by Jingshu Huang on 2/19/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include <sstream>
#include "Node.h"
#include "stringUtility.h"


NodeID Node::nextNodeNamePrefix = 0;


string Node::stringfyNodeID(const NodeID& nId) {
    ostringstream stream;
    stream << nId;
    return stream.str();
}

Node::Node(const NodeType& type, const string& nameSuffix) {
    init(nextNodeNamePrefix++, type, nameSuffix);
}

void Node::init(const NodeID& nID, const NodeType& type, const string& nameSuffix) {
    mNodeID = nID;
    mName = stringfyNodeID(mNodeID);
    mNameSuffix = nameSuffix;
    if (!nameSuffix.empty()) {
        mName += "_" + nameSuffix;
    }
    mNodeType = type;
    reset();
    needUpdate();
}

Node::~Node() {
    removeAllChildren();
    if(mParent) {
        mParent->removeChild(this);
    }
}

// Note: we do not reset node name since nodes still exists in scene manager.
void Node::reset() {
    mParent = NULL;
    removeAllChildren();
    mBehaviors.clear();
    mTxf.reset();
}

void Node::deepCopyFrom(Node* other) {
    assert(other != NULL);
    mName = other->mName;
    mNodeType = other->mNodeType;
    mTxf.deepCopyFrom(other->mTxf);
    mParent = other->mParent;

    for (BehaviorMap::iterator iter = other->mBehaviors.begin();
         iter != other->mBehaviors.end(); ++iter) {
        mBehaviors.insert(BehaviorMap::value_type(iter->first, iter->second));
    }

    nodeChildrenCopyImpl(other);
}

void Node::setParent(Node* parent) {
    mParent = parent;
    needUpdate();
}

void Node::removeChild(Node* child) {
    if (!child) {
        return;
    }

    NodeMap::iterator iter = mChildren.find(child->getID());
    if (iter != mChildren.end() && iter->second == child) {
        mChildren.erase(iter);
        child->setParent(NULL);
    }
}

void Node::removeAllChildren() {
    for (NodeMap::iterator iter = mChildren.begin(); iter != mChildren.end();
         ++iter) {
        if (iter->second) {
            // Note we do not free the memory here. Just unset the parent-child
            // link. The memory is handled by SceneManager.
            iter->second->setParent(NULL);
        }
    }
    mChildren.clear();
}

Node* Node::createChild(const Vector3& translate,
                        const Quaternion& rotate,
                        const Vector3& scale) {
    Node* node = createChildImpl();
    node->translate(translate);
    node->rotate(rotate);
    node->scale(scale);
    addChild(node);
    return node;
}

void Node::addChild(Node* child) {
    assert(!child->mParent);    
    mChildren[child->getID()] = child;
    child->setParent(this);
}

void Node::translate(const Vector3& t) {
    mTxf.mTranslate += t;
}

void Node::rotate(const Quaternion& q) {
    Quaternion qnorm = q;
    qnorm.normalise();     // Normalise quaternion to avoid drift.
    mTxf.mRotate = qnorm * mTxf.mRotate;
    needUpdate();
}

void Node::scale(const Vector3& scale) {
    mTxf.mScale *= scale;
    needUpdate();
}

//void Node::rotate(const Vector3& axis, const Radian& angle) {
//    Quaternion q;
//    q.FromAngleAxis(angle,axis);
//    rotate(q, relativeTo);
//    needUpdate();
//}

void Node::needUpdate(bool forceParentUpdate) {
    mTxf.needUpdate(mParent, forceParentUpdate);
}

void Node::update(const float secondsPassed) {
    mTxf.update(mParent, secondsPassed);
    
    for (NodeMap::const_iterator iter = mChildren.begin();
         iter != mChildren.end(); ++iter) {
        Node* child = iter->second;
        child->update(secondsPassed);
    }
}


Node::Transform::Transform() {
    reset();
}

Node::Transform::~Transform() {
}

void Node::Transform::reset() {
    mTranslate = Vector3::ZERO;
    mRotate = Quaternion::IDENTITY;
    mScale = Vector3::UNIT_SCALE;
    
    mDerivedTranslate = Vector3::ZERO;
    mDerivedRotate = Quaternion::IDENTITY;
    mDerivedScale = Vector3::UNIT_SCALE;
    
    mCachedTransformOutOfDate = true;
    mNeedParentUpdate = true;
    mInheritRotate = true;
    mInheritScale = true;
}

void Node::Transform::deepCopyFrom(const Transform& other) {
    mTranslate = other.mTranslate;
    mScale = other.mScale;
    mRotate = other.mRotate;
    
    mInheritRotate = other.mInheritRotate;
    mInheritScale = other.mInheritScale;
    
    mCachedTransformOutOfDate = other.mCachedTransformOutOfDate;
    mCachedTransform = other.mCachedTransform;
    mDerivedTranslate = other.mDerivedTranslate;
    mDerivedScale = other.mDerivedScale;
    mDerivedRotate = other.mDerivedRotate;
    
    mCachedTransformOutOfDate = true;
    mNeedParentUpdate = true;
}

// Get transform in world space.
const Matrix4& Node::Transform::getFullWorldTransform(const Node* parent) const {
    if (mCachedTransformOutOfDate) {
        mCachedTransform.makeTransform(getDerivedTranslate(parent),
                                       getDerivedScale(parent),
                                       getDerivedRotate(parent));
        mCachedTransformOutOfDate = false;
    }
    return mCachedTransform;
}

// Derived rotate with all parents' rotations in world space.
const Quaternion& Node::Transform::getDerivedRotate(const Node* parent) const {
    if (mNeedParentUpdate) {
        updateFromParent(parent);
    }
    return mDerivedRotate;
}

// Derived translate with all parents' translations in world space.
const Vector3& Node::Transform::getDerivedTranslate(const Node* parent) const {
    if (mNeedParentUpdate) {
        updateFromParent(parent);
    }
    return mDerivedTranslate;
}

// Derived scale with all parents' scales in world space.
const Vector3& Node::Transform::getDerivedScale(const Node* parent) const {
    if (mNeedParentUpdate) {
        updateFromParent(parent);
    }
    return mDerivedScale;
}

void Node::Transform::needUpdate(const Node* parent, bool forceParentUpdate) {
    mNeedParentUpdate = true;
//    mNeedChildUpdate = true;
    mCachedTransformOutOfDate = true;
    
    // Make sure we're not root and parent hasn't been notified before
//    if (mParent && (!mParentNotified || forceParentUpdate)) {
//        mParent->requestUpdate(this, forceParentUpdate);
//        mParentNotified = true ;
//    }
//
//    // all children will be updated
//    mChildrenToUpdate.clear();
}

// Pull transform info from parent node and use it to update the current node.
void Node::Transform::updateFromParent(const Node* parent) const {
    if (parent) {
        const Quaternion& parentRotate = parent->getDerivedRotate();
        if (mInheritRotate) {
            mDerivedRotate = parentRotate * mRotate;
        } else {
            mDerivedRotate = mRotate;
        }

        const Vector3& parentScale = parent->getDerivedScale();
        if (mInheritScale) {
            mDerivedScale = parentScale * mScale;
        } else {
            mDerivedScale = mScale;
        }

        mDerivedTranslate = parentRotate * (parentScale * mTranslate);
        mDerivedTranslate += parent->getDerivedTranslate();

    } else {
        // Root node, no parent
        mDerivedRotate = mRotate;
        mDerivedTranslate = mTranslate;
        mDerivedScale = mScale;
    }

    mCachedTransformOutOfDate = true;
    mNeedParentUpdate = false;
}

//void Node::processQueuedUpdates() {
//    for (QueuedUpdates::iterator i = msQueuedUpdates.begin();
//         i != msQueuedUpdates.end(); ++i) {
//        Node* n = *i;
//        n->mQueuedForUpdate = false;
//        n->needUpdate(true);
//    }
//    msQueuedUpdates.clear();
//}

void Node::Transform::update(const Node* parent, const float secondsPassed) {
    if (mNeedParentUpdate) {
        updateFromParent(parent);
    }
}

#pragma mark - behaviors

void Node::attachBehavior(Behavior* behavior) {
    mBehaviors[behavior->getType()] = behavior;
}

void Node::detachBehavior(Behavior* behavior) {
    detachBehavior(behavior->getType());
}

void Node::detachBehavior(const BehaviorType& type) {
    BehaviorMap::iterator iter = mBehaviors.find(type);
    if (iter != mBehaviors.end()) {
        mBehaviors.erase(iter);
    } else {
        LOGD("[SceneNode detachBehavior] Error: cannot find the behavior type %d.", (int)type);
    }
}

Behavior* Node::getBehavior(const BehaviorType& type) {
    BehaviorMap::iterator iter = mBehaviors.find(type);
    if (iter != mBehaviors.end()) {
        return iter->second;
    }
    return NULL;
}

// Process some info before handle touch up behavior.
bool Node::preHandleTouch() {
    // do nothing.
    return true;
}

// |point| is in screen coords (pixel) with the origin at the top left corner.
bool Node::handleTouch(const BehaviorType& type, const Vector2& point) {
    LOGD("[Node handleTouch] triggered. Scene node = %s.\n", mName.c_str());
    Behavior* behavior = getBehavior(type);
    if (behavior) {
        LOGD("[Node handleTouch] Behavior = %lu acts.\n", behavior->getID());
        behavior->act();

        // Get the coords of the point in a coords with the origin at bottom left.
//        Vector2 newPoint = point;
//        int screenH = mSceneMgr->getRenderer()->getScreenHeight();
//        newPoint.y = screenH - newPoint.y;
        return true;
    }
    return false;
}


