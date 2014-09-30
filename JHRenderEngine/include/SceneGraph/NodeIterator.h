//
//  NodeIterator.h
//
//  Created by Jingshu Huang on 9/11/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _NodeIterator_h
#define _NodeIterator_h

#include <stack>

#include "Node.h"
#include "platformDef.h"


class NodeIterator {
public:
    NodeIterator(Node* node) { mCurNode = node; }
    virtual ~NodeIterator() {}

    virtual Node& operator * () {
        return *mCurNode;
    }
    
    virtual bool operator == (const NodeIterator& other) {
        return mCurNode == other.mCurNode;
    }
    
    virtual bool operator != (const NodeIterator& other) {
        return !(*this == other);
    }
    
    virtual NodeIterator& operator++() {
        return next();
    }

    virtual Node* current() {
        return mCurNode;
    }

    virtual NodeIterator& advance() {
        return next();
    }
    
    virtual NodeIterator& next() {
        addAllChildren(mCurNode);
        return advanceWithoutAddChildren();
    }
    
    // Check to see if we still have any unprocessed node.
    virtual bool hasNext() {
        return mCurNode != NULL || !mNodesToIterate.empty();
    }
    
    void addAllChildren(Node* parent) {
        if (!parent) {
            return;
        }
        
        Node::NodeMap::iterator iter = parent->mChildren.begin();
        while (iter != parent->mChildren.end()) {
            mNodesToIterate.push(iter->second);
            ++iter;
        }
    }
    
    NodeIterator& advanceWithoutAddChildren() {
        // Set the node on the top of the stack to be the current one.
        if (mNodesToIterate.size() <= 0) {
            mCurNode = NULL;
        } else {
            mCurNode = mNodesToIterate.top();
            mNodesToIterate.pop();
        }
        
        return *this;
    }
    
protected:
    Node* mCurNode;
    stack<Node*> mNodesToIterate;
};

#endif
