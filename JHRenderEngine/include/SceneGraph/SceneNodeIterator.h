//
//  SceneNodeIterator.h
//
//  Created by Jingshu Huang on 9/11/13.
//  Copyright (c) All rights reserved.
//

#ifndef _SceneNodeIterator_h
#define _SceneNodeIterator_h

#include "NodeIterator.h"
#include "SceneNode.h"


class SceneNodeIterator : public NodeIterator {
public:
    SceneNodeIterator(SceneNode* node) : NodeIterator(node) {}
    ~SceneNodeIterator() {}
    
    SceneNode& operator * () {
        return *(static_cast<SceneNode*>(mCurNode));
    }

    virtual SceneNode* current() {
        return static_cast<SceneNode*>(mCurNode);
    }
};

#endif
