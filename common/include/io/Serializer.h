//
//  Serializer.h
//
//  Created by Jingshu Huang on 12/20/12.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Serializer_h_
#define _Serializer_h_

#include "platformDef.h"


class Serializer {
public:
	virtual ~Serializer() {}

protected:
    string mVersion;
};

#endif
