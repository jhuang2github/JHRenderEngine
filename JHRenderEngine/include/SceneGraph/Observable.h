//
//  Observable.h
//
//  Created by Jingshu Huang on 9/12/13.
//  Copyright (c). All rights reserved.
//

#ifndef __Observable__
#define __Observable__

#include <set>

#include "Notification.h"
#include "Observer.h"
#include "platformDef.h"


// Set instead of hash_set. Child class of observables might want to sort the
// observers so events happen in a certain sequence.
typedef set<Observer*> ObserverSet;


class Observable {
public:
    Observable() {}
    virtual ~Observable() {}

    inline int numObservers(void) { return mObservers.size(); }

    void addObserver(Observer* observer) {
        assert(observer != NULL && "Error: observer is null.\n");
        ObserverSet::iterator iter = mObservers.find(observer);
        if (iter == mObservers.end()) {
            mObservers.insert(observer);
        }
    }

    void removeObserver(Observer* observer) {
        assert(observer != NULL && "Error: observer is null.\n");
        ObserverSet::iterator iter = mObservers.find(observer);
        if (iter != mObservers.end()) {
            mObservers.erase(iter);
        }
    }

    void notifyObservers(const NotificationID& name) {
        Notification msg(name);
        notifyObservers(&msg);
    }

    void notifyObservers(const NotificationID& name, void* data) {
        Notification msg(name, data);
        notifyObservers(&msg);
    }

    virtual void notifyObservers(Notification* notif) {
        // Copy the observer out so we do not block the thread by "update".
        ObserverSet copy;
        copy = mObservers;
        for (ObserverSet::const_iterator iter = copy.begin();
             iter != copy.end(); ++iter) {
            (*iter)->updateOnNotification(this, notif);
        }
    }

protected:
    ObserverSet mObservers;
};

#endif
