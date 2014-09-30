//
//  Renderer.h
//
//  Created by Jingshu Huang on 1/22/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Renderer_h
#define _Renderer_h

#include "RenderWindow.h"
#include "RenderQueue.h"
#include "RenderQueueVisitor.h"


class MaterialManager;
class SceneManager;


// Abstract class.
class Renderer : public RenderQueueVisitor {
public:
    Renderer() {
        assert(false && "Error: default constructor is not supported.");
    }

    Renderer(SceneManager* sceneMgr, MaterialManager* materialMgr) {
        mWindow = new RenderWindow;
        mSceneMgr = sceneMgr;
        mMaterialMgr = materialMgr;
        mRenderQueues.clear();
        mCurCamera = NULL;
    }

    virtual ~Renderer() {
        clearRenderQueues();
        if (mWindow) {
            delete mWindow;
        }
        mWindow = NULL;
        mSceneMgr = NULL;
        mMaterialMgr = NULL;
    }

    // Render target related.
    RenderWindow* getRenderTarget() { return mWindow; }

    virtual void setRenderTargetDimension(const int w, const int h) {
        mWindow->setDimension(w, h);
    }

    inline int getScreenWidth() const { return mWindow->getWidth(); }
    inline int getScreenHeight() const { return mWindow->getHeight(); }
    inline Vector2 getScreenSize(void) {
        return Vector2(getScreenWidth(), getScreenHeight());
    }

    inline SceneManager* getSceneManager(void) { return mSceneMgr; }

    inline void setCamera(Camera* cam) { mCurCamera = cam; }
    inline Camera* getCamera(void) { return mCurCamera; }

    // Render queue related.
    virtual void clearRenderQueues() {
        for (RenderQueueMap::iterator iter = mRenderQueues.begin();
             iter != mRenderQueues.end(); ++iter) {
            RenderQueue* queue = iter->second;
            if (queue) {
                delete queue;
            }
        }
        mRenderQueues.clear();
    }

    virtual RenderQueue* getRenderQueue(const ERenderQueueID& qID) {
        if (mRenderQueues.find(qID) == mRenderQueues.end() || !mRenderQueues[qID]) {
            mRenderQueues[qID] = new RenderQueue;
        }
        return mRenderQueues[qID];
    }

    virtual bool isShaderSupported() { return true; }
    virtual void render(Viewport* vp, Camera* cam) = 0;
    virtual void render(const float secondsPassed) = 0;


protected:
    RenderWindow*   mWindow;
    SceneManager*    mSceneMgr;
    MaterialManager* mMaterialMgr;

    // Map from render queue ID to render queue. These render queues are
    // sorted based on their queue ID.
    typedef map<uint, RenderQueue*> RenderQueueMap;
    RenderQueueMap mRenderQueues;  // One map per viewport per camera

    Camera* mCurCamera;
};

#endif
