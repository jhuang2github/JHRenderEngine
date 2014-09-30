//
//  Observer.h
//
//  Created by Jingshu Huang on 9/12/13.
//  Copyright (c) . All rights reserved.
//

#ifndef __Observer__
#define __Observer__

class Notification;
class Observable;


class Observer {
public:
    virtual ~Observer() {}

    virtual void updateOnNotification(Observable* observable, Notification* msg) = 0;
};

#endif
