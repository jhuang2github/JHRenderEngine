//
//  Behavior.h
//
//  Created by Jingshu Huang on 9/6/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Behavior_h
#define _Behavior_h

#include <sstream>
#include "Observable.h"
#include "platformDef.h"


class BehaviorManager;
class SceneNode;

typedef unsigned long BehaviorID;    // Behavior instance ID. Starts from 1.
typedef unsigned long BehaviorType;  // Behavior class ID. Starts from 1.
typedef string BehaviorTypeName;     // Behavior class name.
typedef set<string> ObserverNameSet; // |set| instead of |hash_set| so events happen in order.


// Defines the interface of a behavior. A game entity has a list of behavior
// properties.
//
// Behaviors include but not limited to:
//  Run a script
//  Move, speak, animate
//  React as a rigid body
//  Emit Particles
//  Play located audio
//  Be packed up by the player
//  Be worn by the player
//  Explode
//  React to magnets
//  Be targeted by the player
//  Path finding and following
//  ...
class Behavior : public Observable {
public:
    Behavior() : mOwner(NULL) { }
    virtual ~Behavior();

    inline void setID(const BehaviorID& bid) { mID = bid; }
    inline BehaviorID& getID() { return mID; }

    inline void setBehaviorType(const BehaviorType& type) { mType = type; }
    inline BehaviorType getType(void) const { return mType; }

    inline void setNotificationID(const NotificationID& nid) { mNotifID = nid; }
    inline NotificationID getNotificationID(void) const { return mNotifID; }

    inline void setOwner(SceneNode* owner) { mOwner = owner; }
    inline SceneNode* getOwner(void) const { return mOwner; }

    // In addition to the setup of the properties, a client might want to set up
    // some additional field.
    virtual void additionalInit(void) {}

    inline void addObserverName(const string& observer) { mObserverNames.insert(observer); }
    virtual void setObservers(void);

    virtual bool act(void);

protected:
    BehaviorID mID;
    BehaviorType mType;
    NotificationID mNotifID;
    SceneNode* mOwner;  // Owner of the behavior. Does not own the data here.

    // A set of observer's names. This is just a temporary storage while parsing
    // scene description files.
    ObserverNameSet mObserverNames;
};

#endif
