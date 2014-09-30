//
//  GLES2Renderer.h
//
//  Created by Jingshu Huang on 1/22/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _GLES2Renderer_h
#define _GLES2Renderer_h

#include "matrix4.h"
#include "platformDef.h"
#include "Renderer.h"
#include "RenderWindow.h"


class Pass;
class RenderQueue;

// Abstract class.
class GLES2Renderer : public Renderer {
public:
    GLES2Renderer();
    virtual ~GLES2Renderer() = 0;

    virtual void render(const float secondsPassed);
    virtual void render(Viewport* vp, Camera* cam);  // render a scene

protected:
    void preVisit(Pass* p);
    void visit(Renderable* r);  // render one renderable instance.
    void postVisit(Pass* p);

private:
    void setViewport(Viewport* vp);

    void beginFrame();
    void endFrame();
    void processRenderQueues();

    void setPass(const Pass* pass);
    void renderSingleObject(Renderable* rend, const Pass* pass);

private:
    // Debug use only.
    void debugTransformations();
    void debugDraw();

    // Local vars.
    Pass*   mCurPass;
};

#endif
