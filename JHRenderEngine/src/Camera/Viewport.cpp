//
//  Viewport.cpp
//
//  Created by Jingshu Huang on 2/25/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "Camera.h"
#include "RenderTarget.h"
#include "Viewport.h"


Viewport::Viewport() {
    mTarget = NULL;
    mDefaultCamera = NULL;
}

Viewport::Viewport(RenderTarget* target, const Rect& vp, int ZOrder) {
    mTarget = target;
    mViewport = vp;
    // We set up default camera later (when we parse the scene description file).
    mDefaultCamera = NULL;
}

Viewport::~Viewport() {
    mCameras.clear();
    mDefaultCamera = NULL;
    mTarget = NULL;
}

void Viewport::update() {
    for (CameraList::const_iterator iter = mCameras.begin();
         iter != mCameras.end(); ++iter) {
        (*iter)->update(this);
    }
}

void Viewport::render() {
    // Draw from back to front.
    for (CameraList::reverse_iterator iter = mCameras.rbegin();
         iter != mCameras.rend(); ++iter) {
        (*iter)->render(this);
    }
}

// Given |point| in viewport coords (pixel) with the origin at the top left
// corner, compute the normalized device coords xyz: (-1, 1).
//
// TODO(jhuang): we need to convert screen point to our oriented viewport.
//    Real tX = screenX; Real a = getOrientationMode() * Math::HALF_PI;
//    screenX = Math::Cos(a) * (tX-0.5f) + Math::Sin(a) * (screenY-0.5f) + 0.5f;
//    screenY = Math::Sin(a) * (tX-0.5f) + Math::Cos(a) * (screenY-0.5f) + 0.5f;
//    if ((int)getOrientationMode()&1) screenY = 1.f - screenY;
//
Vector2 Viewport::getNormalizedScreenCoords(const Vector2& point) {
    Vector2 normalized = (point - Vector2(mViewport.x, mViewport.y)) /
                          Vector2(mViewport.width, mViewport.height);

    float nx = (2.f * normalized.x) - 1.f;
    float ny = 1.f - (2.f * normalized.y);
    return Vector2(nx, ny);
}

// |screenPoint| is in screen coords (pixel) with the origin at the top left corner.
SceneObj* Viewport::getSelectedSceneObj(const Vector2 &screenPoint) {
    Vector2 normalized = getNormalizedScreenCoords(screenPoint);
    // Check from front to back.
    for (vector<Camera*>::const_iterator iter = mCameras.begin();
         iter != mCameras.end(); ++iter) {
        SceneObj* selected = (*iter)->getSelectedSceneObj(normalized);
        if (selected) {
            return selected;
        }
    }
    return NULL;
}

