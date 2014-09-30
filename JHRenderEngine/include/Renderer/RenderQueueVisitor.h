//
//  RenderQueueVisitor.h
//
//  Created by Jingshu Huang on 11/12/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//
//

#ifndef _RenderQueueVisitor_h
#define _RenderQueueVisitor_h


class Pass;
class Renderable;


// A visitor to renderables in a render queue.
class RenderQueueVisitor {
public:
    virtual ~RenderQueueVisitor() {}
    
protected:
    virtual void preVisit(Pass* p) {}
    virtual void visit(Renderable* r) = 0;
    virtual void postVisit(Pass* p) {}
    
    friend class RenderQueue;
};

#endif
