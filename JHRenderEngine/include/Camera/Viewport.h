//
//  Viewport.h
//
//  Created by Jingshu Huang on 2/25/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Viewport_h
#define _Viewport_h

#include "rect.h"
#include "vector4.h"


class Camera;
class RenderTarget;
class SceneObj;


using namespace JHEngine;  // For Rect.

// Each viewport has a list of cameras as source and a target as a destination.
class Viewport {
public:
    typedef vector<Camera*> CameraList;

    Viewport();
    Viewport(RenderTarget* target, const Rect& vp, int ZOrder);
    ~Viewport();

    inline void addCamera(Camera* cam) { mCameras.push_back(cam); }
    inline Camera* getCameras(const int index) const { return mCameras[index]; }
    inline CameraList& getCameras() { return mCameras; }
    inline void setDefaultCamera(Camera* cam) { mDefaultCamera = cam; }
    inline Camera* getDefaultCamera() const { return mDefaultCamera; }

    inline const Rect& getViewportRect() { return mViewport; }
    inline RenderTarget* getRenderTarget(void) { return mTarget; }

    // |point| is in screen coords (pixel) with the origin at the top left corner.
    inline bool containsPoint(const Vector2& point) const {
        return mViewport.containsPoint(point);
    }

    SceneObj* getSelectedSceneObj(const Vector2& screenPoint);
    Vector2 getNormalizedScreenCoords(const Vector2& point);

    void update();
    void render();

private:
    RenderTarget* mTarget; // owner of the viewport

    // Viewport is in screen coords (pixel) with the origin at the top left corner.
    // Note this is different from the args in glViewport(x, y, w, h) where (x,y)
    // is at the bottom left corner.
    Rect mViewport;

    // Does not own the data. Data is owned by SceneManager. The order that the
    // cameras are added into the list is defined by scene description file.
    // Usually we should specify data from near to far.
    CameraList mCameras;
    Camera* mDefaultCamera;
};

#endif
