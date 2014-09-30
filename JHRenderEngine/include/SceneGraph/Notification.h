//
//  Notification.h
//
//  Created by Jingshu Huang on 9/12/13.
//  Copyright (c). All rights reserved.
//

#ifndef _Notification_h
#define _Notification_h

#include "platformDef.h"

typedef int NotificationID;


class Notification {
public:
    Notification(const NotificationID& msgID) : mID(msgID), mData(NULL) {}
    Notification(const NotificationID& msgID, void* data) : mID(msgID), mData(data) {}

    inline const NotificationID getID(void) const { return mID; }
    inline void* getData(void) { return mData; }

protected:
    NotificationID mID;
    void* mData;      // Does not own the data.
};

#endif
