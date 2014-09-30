//
//  BehaviorManager.cpp
//
//  Created by Jingshu Huang on 9/6/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "Behavior.h"
#include "BehaviorManager.h"
#include "SceneNode.h"
#include "SceneObj.h"


const static BehaviorType kInvalidBehaviorType = 0;

BehaviorType BehaviorManager::sNextBehaviorType = 1;
BehaviorID BehaviorManager::sNextBehaviorID = 1;


void BehaviorManager::clearBehaviors() {
    for (BehaviorMap::iterator k = mBehaviors.begin();
         k != mBehaviors.end(); ++k) {
        if (k->second) {
            delete k->second;
        }
    }
    mBehaviors.clear();
}


BehaviorType BehaviorManager::getBehaviorType(
        const BehaviorTypeName& className, bool addIfNotExist) {
    BehaviorType type = kInvalidBehaviorType;
    BehaviorNameMap::iterator it = mBehaviorNames.find(className);
    if (it == mBehaviorNames.end()) {
        if (addIfNotExist) {
            type = sNextBehaviorType++;
            mBehaviorNames.insert(BehaviorNameMap::value_type(className, type));
            mBehaviorTypes.insert(BehaviorTypeMap::value_type(type, className));
        }
    } else {
        type = it->second;
    }
    return type;
}


BehaviorTypeName BehaviorManager::getBehaviorTypeName(const BehaviorType& type) {
    BehaviorTypeMap::iterator it = mBehaviorTypes.find(type);
    if (it == mBehaviorTypes.end()) {
        return "";
    }
    return it->second;
}


void BehaviorManager::registerBehaviorFactory(const BehaviorTypeName& className,
                                              BehaviorFactoryPtr ptr) {
    const BehaviorType type = getBehaviorType(className, true);
    mBehaviorFactorys.insert(BehaviorFactoryMap::value_type(type, ptr));
    LOGD("Behavior Factory %s (ID=%d) registered.", className.c_str(), (int)type);
}


BehaviorFactoryPtr BehaviorManager::getBehaviorFactory(
        const BehaviorTypeName& className) {
    assert(!mBehaviorFactorys.empty());
    const BehaviorType type = getBehaviorType(className, false);
    BehaviorFactoryPtr ptr = getBehaviorFactory(type);
    if (!ptr) {
        LOGD("[BehaviorManager getBehaviorFactory] Error: Could not find the behavior creation method %s. Did you forget to register your behavior class?", className.c_str());
    }
    return ptr;
}


BehaviorFactoryPtr BehaviorManager::getBehaviorFactory(const BehaviorType& type) {
    assert(!mBehaviorFactorys.empty());
    if (type != kInvalidBehaviorType) {
        BehaviorFactoryMap::iterator iter = mBehaviorFactorys.find(type);
        if (iter != mBehaviorFactorys.end()) {
            return iter->second;
        }
    }
    LOGD("[BehaviorManager getBehaviorFactory] Error: Could not find the behavior creation method for behavior type %d.", (int)type);
    return NULL;
}


Behavior* BehaviorManager::createBehavior(const BehaviorTypeName& className,
                                          SceneNode* owner,
                                          const int notifID) {
    const BehaviorType type = getBehaviorType(className, false);
    BehaviorFactoryPtr methodPtr = getBehaviorFactory(type);
    if (!methodPtr) {
        LOGD("[BehaviorManager createBehavior] Error: cannot create behavior.");
        return NULL;
    }

    BehaviorID bid = sNextBehaviorID++;
    Behavior* behavior = (*methodPtr)(bid, type, owner, notifID);
    assert(behavior != NULL && "Error: cannot create behavior. Behavior is null.");
    mBehaviors[bid] = behavior;
    if (owner) {
        owner->attachBehavior(behavior);
    }
    return behavior;
}


void BehaviorManager::deleteBehavior(const BehaviorID& bid) {
    BehaviorMap::iterator iter = mBehaviors.find(bid);
    if (iter != mBehaviors.end()) {
//        BehaviorFactory::getInstance()->destroy(iter->second);
        delete iter->second;
        mBehaviors.erase(iter);
    }
}

void BehaviorManager::registerBehaviorObservers(void) {
    for (BehaviorMap::iterator k = mBehaviors.begin();
         k != mBehaviors.end(); ++k) {
        if (k->second) {
            k->second->setObservers();
        }
    }
}
