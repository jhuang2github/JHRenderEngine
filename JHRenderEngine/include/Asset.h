//
//  Asset.h
//
//  Created by Jingshu Huang on 2/11/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Asset_h
#define _Asset_h


class Asset {
public:
    virtual ~Asset() {}
    virtual bool load() = 0;
};

#endif
