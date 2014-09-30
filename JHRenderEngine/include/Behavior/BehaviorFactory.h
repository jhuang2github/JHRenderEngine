//
//  BehaviorFactory.h
//
//  Created by Jingshu Huang on 9/6/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __BehaviorFactory__
#define __BehaviorFactory__

#include "BehaviorManager.h"
#include "platformDef.h"


template<typename T>
Behavior* createBehavior(BehaviorID const& bid,
                         BehaviorType const& type,
                         SceneNode* owner,
                         const int notif) {
    T* newInst = new T;
    newInst->setID(bid);
    newInst->setBehaviorType(type);
    newInst->setOwner(owner);
    newInst->setNotificationID(notif);
    newInst->additionalInit();
    return newInst;
}


// A template for all the game behavior factory classes.
// A game specific behavior should register itself with the behavior factory by:
//    1. declare "static BehaviorFactory<MyClass> sBehaviorFactory;" in header
//    2. "BehaviorFactory<MyClass> BehaviorFactory::sBehaviorFactory("MyClass");"
//       in implementation.
// Also, make sure the class name matches the name used in the .scene data file.
template<typename T>
class BehaviorFactoryT {
public:
    BehaviorFactoryT(string const& behaviorClassName) {
        BehaviorManager::getInstance()->registerBehaviorFactory(
                behaviorClassName, &createBehavior<T>);
    }
};

// TODO(jhuang): define macro for the function registeration.
//#define REGISTER_DEC_TYPE(NAME) \
//static DerivedRegister<NAME> reg
//
//#define REGISTER_DEF_TYPE(NAME) \
//DerivedRegister<NAME> NAME::reg(#NAME)

#endif
