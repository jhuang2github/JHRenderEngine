//
//  Frustum.h
//
//  Created by Jingshu Huang on 1/22/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Frustum_h
#define _Frustum_h

#include "matrix4.h"
#include "plane.h"


class AxisAlignedBox;
class Plane;
class Sphere;


typedef enum {
    ORTHOGRAPHIC,
    PERSPECTIVE
} EProjectionType;


typedef enum {
    PLANE_NEAR   = 0,
    PLANE_FAR    = 1,
    PLANE_LEFT   = 2,
    PLANE_RIGHT  = 3,
    PLANE_TOP    = 4,
    PLANE_BOTTOM = 5
} EFrustumPlane;


class Frustum {
public:
    Frustum(const EProjectionType& type=ORTHOGRAPHIC);
    ~Frustum();

    void setProjectionType(const EProjectionType& type);
    inline void setNearPlane(const float near) { mNear = near; }
    inline void setFarPlane(const float far) { mFar = far; }
    inline void setFovy(const float fov) { mFovy = fov; }
    inline void setAspect(const float aspect) { mAspect = aspect; }

    inline float getNearPlane() { return mNear; }
    inline float getFarPlane() { return mFar; }
    inline float getFovy() { return mFovy; }
    inline float getAspect() { return mAspect; }
    const Plane* getFrustumPlanes() const { return mFrustumPlanes; }
    const Plane& getFrustumPlane(const EFrustumPlane& plane) const {
        return mFrustumPlanes[plane];
    }

    void updateProjection(Matrix4& projMatrix);
    void updateFrustumPlanes(const Matrix4& viewProjMatrix);

    bool isVisible(const AxisAlignedBox& bound) const;
    bool isVisible(const Vector3& vert) const;
    bool isVisible(const Sphere& sphere) const;

private:
    void calcProjectionParams(float& left, float& right, float& bottom, float& top);
    void updatePerspectiveProj(Matrix4& mat);
    void updateOrthographicProj(Matrix4& mat);

private:
    EProjectionType mProjType;
    Plane mFrustumPlanes[6];

    float mNear;
    float mFar;
    float mAspect;   // aspect = width / height
    float mFovy;     // in degrees
};

#endif
