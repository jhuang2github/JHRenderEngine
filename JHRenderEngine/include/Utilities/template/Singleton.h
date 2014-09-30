//
//  Singleton.h
//
//  Created by Jingshu Huang on 1/9/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Singleton_h_
#define _Singleton_h_


template <typename S>
class Singleton {
public:
    static S* getInstance() {
        static S* instance = 0;
        if (!instance) {
            instance = new S;
        }
        return instance;
    }
};

#endif
