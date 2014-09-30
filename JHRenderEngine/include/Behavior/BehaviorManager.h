//
//  BehaviorManager.h
//
//  Created by Jingshu Huang on 9/6/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __BehaviorManager__
#define __BehaviorManager__

#include "AssetManager.h"
#include "Behavior.h"
#include "platformDef.h"
#include "Singleton.h"


#define kInvalidNotification  0


typedef Behavior* (*BehaviorFactoryPtr)(const BehaviorID& bid,
                                        BehaviorType const& type,
                                        SceneNode* owner,
                                        const int notifID);


class BehaviorManager : public Singleton<BehaviorManager>,
                        public AssetManager<string, Behavior*> {
public:
    void registerBehaviorFactory(const BehaviorTypeName&, BehaviorFactoryPtr);
    void registerBehaviorObservers(void);

    Behavior* createBehavior(const BehaviorTypeName& behaviorClassName,
                             SceneNode* owner,
                             const int notif=kInvalidNotification);
    void deleteBehavior(const BehaviorID& behaviorID);

    BehaviorType getBehaviorType(const BehaviorTypeName& className,
                                 bool addIfNotExist=false);
    BehaviorTypeName getBehaviorTypeName(const BehaviorType& type);

private:
    ~BehaviorManager() {
        clearBehaviors();
    }

    void clearBehaviors();

    BehaviorFactoryPtr getBehaviorFactory(const BehaviorTypeName& className);
    BehaviorFactoryPtr getBehaviorFactory(const BehaviorType& type);

private:
    static BehaviorID   sNextBehaviorID;
    static BehaviorType sNextBehaviorType;

    // Map from a behavior class type ID to behavior creation function pointer.
    typedef unordered_map<BehaviorType, BehaviorFactoryPtr> BehaviorFactoryMap;
    BehaviorFactoryMap mBehaviorFactorys;

    // Map from a behaivor class type ID to a behavior class name.
    typedef unordered_map<BehaviorType, BehaviorTypeName> BehaviorTypeMap;
    BehaviorTypeMap mBehaviorTypes;
    // Map from a behavior class name to a behavior class type ID.
    typedef unordered_map<BehaviorTypeName, BehaviorType> BehaviorNameMap;
    BehaviorNameMap mBehaviorNames;

    // Map from a behavior ID to a behavior instance. Owns the data.
    typedef unordered_map<BehaviorID, Behavior*> BehaviorMap;
    BehaviorMap mBehaviors;
};

#endif
