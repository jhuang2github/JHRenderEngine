//
//  RenderWindow.cpp
//
//  Created by Jingshu Huang on 2/25/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "RenderWindow.h"
#include "Viewport.h"


// Note each render queue queues up renderables for a given camera.
// This filling of render queues could be parallelly processed with thread
// pool workers.
/*
 // now fill our render views with visible objects in a
 // threaded environment.
 for (int i = 0; i < currentViews.size(); i++) {
    Thread pool.QueueWork(procThreadedFillRenderView, currentViews[i]);
 }
 Thread pool.waitForWorkToFinish();
 
 ///
 procThreadedFillRenderView(void *DataPacket) {
    RenderView *currView = (RenderView *) DataPacket;
    List<RenderObject *> objects = gObjectManager.giveFrustumCollision(currView->frustum);
    for (int q = 0; q < objects.size(); q++) {
        RenderCommand *RC = new RenderCommand();
        Objects[q]->fillCommandBuffer(RC);
        currentViews[i].RenderCommands.add(RC);
    }
 }
 */
void RenderWindow::update(const float secondsPassed) {
    // Go through viewports in depth order and update. Mostly update cameras
    // associated with each viewport.
    for (ViewportMap::const_iterator iter = mViewports.begin();
         iter != mViewports.end(); ++iter) {
        if (iter->second) {
            iter->second->update();
        }
    }
}

/*
void renderControl::serializeRenderViews(List<RenderView *> Views) {
 for (int viewType = 0; viewType < Count; viewType++) {
    for (int i = 0; i < views.size(); i++) {
        if (Views[i].mViewType != viewType)
            continue;
 
        BindRenderTarget(Views[i]->renderTarget, Views[i]->DepthTarget);
 
        if (Views[i]->clearTargets) {
            ClearTarget(Views[i]->clearFlags,
            Views[i]->clearColor, Views[i]->clearDepths);
        }
 
        for (int k = 0; k < Views[i]->commands.size(); k++)
            executeDrawCommand(Views[i]->commands[k]);
    }
 
 }
}
 */
void RenderWindow::render(const float secondsPassed) {
    clearFrameBuffer();

    // Go through viewports in depth order and render.
    for (ViewportMap::const_iterator iter = mViewports.begin();
         iter != mViewports.end(); ++iter) {
        if (iter->second) {
            iter->second->render();
        }
    }
}


// |point| is in screen coords (pixel) with the origin at the top left corner.
SceneObj* RenderWindow::getSelectedSceneObj(const Vector2& point) {
//    // Change the screen point to viewport point (with the origin at the lower
//    // left corner.
//    Vector2 point = screenPoint;
//    point.y = getHeight() - screenPoint.y;

    // Go through viewports in reverse depth order (front to back).
    for (ViewportMap::reverse_iterator iter = mViewports.rbegin();
         iter != mViewports.rend(); ++iter) {
        Viewport* vp = iter->second;
        if (vp && vp->containsPoint(point)) {
            SceneObj* selectedObj = vp->getSelectedSceneObj(point);
            if (selectedObj) {
                return selectedObj;
            }
        }
    }
    return NULL;
}
