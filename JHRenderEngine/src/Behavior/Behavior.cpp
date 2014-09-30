//
//  Behavior.cpp
//
//  Created by Jingshu Huang on 9/6/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//


#include "Behavior.h"
#include "SceneMananger.h"


Behavior::~Behavior() {
    mOwner = NULL;
    mObserverNames.clear();
}


void Behavior::setObservers(void) {
    SceneManager* sceneMgr = SceneManager::getInstance();
    for (ObserverNameSet::const_iterator iter = mObserverNames.begin();
         iter != mObserverNames.end(); ++iter) {
        const string& name = *iter;
        SceneNode* node = sceneMgr->getSceneNode(name);
        if (!node) {
            LOGD("[Behavior setObservers]: Error: while registering behaviors, "
                 "fail to find the behavior %s.", name.c_str());
        } else {
            addObserver(node);
        }
    }
}


// Default act does nothing except sending out a notification.
bool Behavior::act(void) {
    notifyObservers(mNotifID, mOwner);
    return true;
}
