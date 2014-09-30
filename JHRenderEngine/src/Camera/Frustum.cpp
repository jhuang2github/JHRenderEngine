//
//  Frustum.cpp
//
//  Created by Jingshu Huang on 1/22/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "axisAlignedBox.h"
#include "Frustum.h"
#include "platformDef.h"
#include "sphere.h"


Frustum::Frustum(const EProjectionType& type) {
    setProjectionType(type);
    mAspect = 1.f;
    mFovy = 45.f;
}

Frustum::~Frustum() {
}

void Frustum::setProjectionType(const EProjectionType& type) {
    mProjType = type;
    if (PERSPECTIVE == type) {
        mNear = 0.01f;
        mFar = 1000.f;
    } else {
        // For an identity projection matrix. (near = 1, far = -1).
        // Everything projects to the near plane in an ortho proj.
        mNear = 1.f;
        mFar = 1000.f;
    }
}

void Frustum::updateProjection(Matrix4& projMatrix) {
    if (mProjType == PERSPECTIVE) {
        updatePerspectiveProj(projMatrix);
    } else if (mProjType == ORTHOGRAPHIC) {
        updateOrthographicProj(projMatrix);
    }
}

void Frustum::calcProjectionParams(float& left, float& right,
                                   float& bottom, float& top) {
    float ymax = mNear * tanf(mFovy * kRadianPerDegree);
    float xmax = ymax * mAspect;
    left = -xmax;
    right = xmax;
    top = -ymax;
    bottom = ymax;
}

// Similar to glFrustum.
// Creates a row-major perspective projection matrix,
// which depth range [-1,1], right-handed.
//
// [ A   0   C   0  ]
// [ 0   B   D   0  ]
// [ 0   0   q   qn ]
// [ 0   0   -1  0  ]
//
// A = 2 * near / (right - left)
// B = 2 * near / (top - bottom)
// C = (right + left) / (right - left)
// D = (top + bottom) / (top - bottom)
// q = - (far + near) / (far - near)
// qn = - 2 * (far * near) / (far - near)
void Frustum::updatePerspectiveProj(Matrix4& projMatrix) {
    float left, right, bottom, top;
    calcProjectionParams(left, right, top, bottom);
    
    float inv_w = 1 / (right - left);
    float inv_h = 1 / (top - bottom);
    float inv_d = 1 / (mFar - mNear);
    
    float A = 2 * mNear * inv_w;
    float B = 2 * mNear * inv_h;
    float C = (right + left) * inv_w;
    float D = (top + bottom) * inv_h;
    float q = - (mFar + mNear) * inv_d;
    float qn = -2 * (mFar * mNear) * inv_d;
    
    projMatrix = Matrix4::ZERO;
    projMatrix[0][0] = A;
    projMatrix[0][2] = C;
    projMatrix[1][1] = B;
    projMatrix[1][2] = D;
    projMatrix[2][2] = q;
    projMatrix[2][3] = qn;
    projMatrix[3][2] = -1;
}

// Similar to glOrtho.
// Creates a row-major orthographic projection matrix,
// which depth range [-1,1], right-handed rules
//
// [ A   0   0   C  ]
// [ 0   B   0   D  ]
// [ 0   0   q   qn ]
// [ 0   0   0   1  ]
//
// A = 2 * / (right - left)
// B = 2 * / (top - bottom)
// C = - (right + left) / (right - left)
// D = - (top + bottom) / (top - bottom)
// q = - 2 / (far - near)
// qn = - (far + near) / (far - near)
//
// To get identity projection matrix:
// glOrtho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
//   left = -1
//   right = 1
//   top = -1
//   bottom = 1
//   near = -1
//   far = 1
void Frustum::updateOrthographicProj(Matrix4& projMatrix) {
    float left, right, bottom, top;
    calcProjectionParams(left, right, top, bottom);
    
    float inv_w = 1 / (right - left);
    float inv_h = 1 / (top - bottom);
    float inv_d = 1 / (mFar - mNear);
    
    float A = 2 * inv_w;
    float B = 2 * inv_h;
    float C = - (right + left) * inv_w;
    float D = - (top + bottom) * inv_h;
    float q = - 2 * inv_d;
    float qn = - (mFar + mNear)  * inv_d;
    
    projMatrix = Matrix4::ZERO;
    projMatrix[0][0] = A;
    projMatrix[0][3] = C;
    projMatrix[1][1] = B;
    projMatrix[1][3] = D;
    projMatrix[2][2] = q;
    projMatrix[2][3] = qn;
    projMatrix[3][3] = 1;
}


// Given a viewprojection viewProj, upate frustum planes.
void Frustum::updateFrustumPlanes(const Matrix4& viewProj) {
    Vector3 normalLeft(viewProj[3][0] + viewProj[0][0],
                       viewProj[3][1] + viewProj[0][1],
                       viewProj[3][2] + viewProj[0][2]);
    float len = normalLeft.length();
    Plane& planeLeft = mFrustumPlanes[PLANE_LEFT];
    planeLeft.setNormal(normalLeft);
    planeLeft.setDistance(viewProj[3][3] + viewProj[0][3] / len);

    Vector3 normalRight(viewProj[3][0] - viewProj[0][0],
                        viewProj[3][1] - viewProj[0][1],
                        viewProj[3][2] - viewProj[0][2]);
    len = normalRight.length();
    Plane& planeRight = mFrustumPlanes[PLANE_RIGHT];
    planeRight.setNormal(normalRight);
    planeRight.setDistance(viewProj[3][3] - viewProj[0][3] / len);

    Vector3 normalTop(viewProj[3][0] - viewProj[1][0],
                      viewProj[3][1] - viewProj[1][1],
                      viewProj[3][2] - viewProj[1][2]);
    len = normalTop.length();
    Plane& planeTop = mFrustumPlanes[PLANE_TOP];
    planeTop.setNormal(normalTop);
    planeTop.setDistance(viewProj[3][3] - viewProj[1][3] / len);

    Vector3 normalBottom(viewProj[3][0] + viewProj[1][0],
                         viewProj[3][1] + viewProj[1][1],
                         viewProj[3][2] + viewProj[1][2]);
    len = normalBottom.length();
    Plane& planeBottom = mFrustumPlanes[PLANE_BOTTOM];
    planeBottom.setNormal(normalBottom);
    planeBottom.setDistance(viewProj[3][3] + viewProj[1][3] / len);

    Vector3 normalNear(viewProj[3][0] + viewProj[2][0],
                       viewProj[3][1] + viewProj[2][1],
                       viewProj[3][2] + viewProj[2][2]);
    len = normalNear.length();
    Plane& planeNear = mFrustumPlanes[PLANE_NEAR];
    planeNear.setNormal(normalNear);
    planeNear.setDistance(viewProj[3][3] + viewProj[2][3] / len);

    Vector3 normalFar(viewProj[3][0] - viewProj[2][0],
                      viewProj[3][1] - viewProj[2][1],
                      viewProj[3][2] - viewProj[2][2]);
    len = normalFar.length();
    Plane& planeFar = mFrustumPlanes[PLANE_FAR];
    planeFar.setNormal(normalFar);
    planeFar.setDistance(viewProj[3][3] - viewProj[2][3] / len);
}


// Check if an AABB is inside the frustum.
// Note when calling this function, we should have already updated the frustum
// planes by calling updateFrustumPlanes(viewProj).
bool Frustum::isVisible(const AxisAlignedBox& bound) const {
    if (bound.isNull()) {
        return false;
    }
    if (bound.isInfinite()) {
        return true;
    }
    //updateFrustumPlanes();

    Vector3 centre = bound.getCenter();
    Vector3 halfSize = bound.getHalfSize();

    // For each plane, see if all points are on the negative side.
    // If so, object is not visible
    for (int plane = 0; plane < 6; ++plane) {
        Plane::Side side = mFrustumPlanes[plane].getSide(centre, halfSize);
        if (side == Plane::NEGATIVE_SIDE) {
            return false;
        }
    }

    return true;
}


// Check if a vertex is inside the frustum.
// Note when calling this function, we should have already updated the frustum
// planes by calling updateFrustumPlanes(viewProj).
bool Frustum::isVisible(const Vector3& vert) const {
    // For each plane, see if all points are on the negative side
    // If so, the object is not visible.
    for (int plane = 0; plane < 6; ++plane) {
        if (mFrustumPlanes[plane].getSide(vert) == Plane::NEGATIVE_SIDE) {
            return false;
        }
    }
    return true;
}


// Check if a sphere is inside the frustum.
// Note when calling this function, we should have already updated the frustum
// planes by calling updateFrustumPlanes(viewProj).
bool Frustum::isVisible(const Sphere& sphere) const {
    // For each plane, see if sphere is on negative side
    // If so, object is not visible
    for (int plane = 0; plane < 6; ++plane) {
        // If the distance from sphere center to plane is negative, and 'more negative'
        // than the radius of the sphere, sphere is outside frustum
        if (mFrustumPlanes[plane].getDistance(sphere.getCenter()) < -sphere.getRadius()) {
            return false;
        }
    }

    return true;
}

