//
//  Camera.cpp
//
//  Created by Jingshu Huang on 12/20/12.
//  Copyright (c) HuangImage 2013. All rights reserved.
//
#include <math.h>

#include "Camera.h"
#include "line.h"
#include "Renderer.h"
#include "RenderWindow.h"
#include "SceneMananger.h"
#include "Viewport.h"


const string Camera::kDefaultCameraName = "proj_camera_0";
Vector3 Camera::kIsometricCameraPosition = Vector3(1.f, 1.f, 1.f);

bool Camera::sFactoryInitialized = registerFactory();


bool Camera::registerFactory() {
    SceneManager::getInstance()->registerSceneObjFactory(
        kDefaultCameraClass, new Camera("CameraFactory", kDefaultCameraClass));
    return true;
}

// Factory method.
SceneObj* Camera::create(SceneManager* sceneMgr,
                         const string& cameraName,
                         const KeyValuePairStringMap* params) {
    return new Camera(cameraName, kDefaultCameraClass, sceneMgr);
}

Camera::Camera(const string& name, const ESceneObjType& type) : SceneObj(name, type) {
    reset();
}

Camera::Camera(const string& name, const ESceneObjType& type, SceneManager* mgr) :
        SceneObj(name, type, mgr, QUEUE_5_MAIN) {
    reset();
    RenderWindow* target = mgr->getRenderer()->getRenderTarget();
    mFrustum.setAspect((float)target->getWidth() / (float)target->getHeight());
}

Camera::~Camera() {
}

void Camera::reset() {
    mNeedUpdateProjection = true;
    mNeedUpdateView = true;
    mNeedUpdateViewProjection = true;
    mNeedUpdateFrustum = true;
    mProjMatrix = Matrix4::IDENTITY;
    mViewMatrix = Matrix4::IDENTITY;
    mViewProjMatrix = Matrix4::IDENTITY;
}

void Camera::deepCopyFrom(SceneObj* other_) {
    //TODO(jhuang):
    assert(false && "Error: deep copy of camera has not been implemented (TODO)!");
//    Camera* other = static_cast<Camera*>(other_);
    SceneObj::deepCopyFrom(other_);
}

const Matrix4& Camera::getViewProjectionMatrix() {
    if (mNeedUpdateViewProjection) {
        mViewProjMatrix = mProjMatrix * mViewMatrix;
        mNeedUpdateFrustum = true;
    }
    return mViewProjMatrix;
}

Matrix4* Camera::getViewProjectionMatrixPtr() {
    if (mNeedUpdateViewProjection) {
        mViewProjMatrix = mProjMatrix * mViewMatrix;
        mNeedUpdateFrustum = true;
    }
    return &mViewProjMatrix;
}

void Camera::updateProjection() {
    if (!mNeedUpdateProjection) {
        return;
    }
    mFrustum.updateProjection(mProjMatrix);
    mNeedUpdateProjection = false;
    mNeedUpdateViewProjection = true;
}

void Camera::updateView() {
    if (!mNeedUpdateView) {
        return;
    }

    Matrix3 rot;
    mOrientation.ToRotationMatrix(rot);
    Matrix3 rotT = rot.Transpose();
    Vector3 trans = -rotT * mPosition;

    // View matrix (row-major) is:
    //  [ Lx  Ux  Dx  Tx  ]
    //  [ Ly  Uy  Dy  Ty  ]
    //  [ Lz  Uz  Dz  Tz  ]
    //  [ 0   0   0   1   ]
    //
    // Where T = -(Transposed(Rot) * Pos)
    mViewMatrix = Matrix4::IDENTITY;
    mViewMatrix = rotT; // fills upper 3x3
    mViewMatrix[0][3] = trans.x;
    mViewMatrix[1][3] = trans.y;
    mViewMatrix[2][3] = trans.z;

    mNeedUpdateView = false;
    mNeedUpdateViewProjection = true;
}

void Camera::lookAt(const Vector3& target) {
    setDirection(target - mPosition);
    mNeedUpdateView = true;
}

void Camera::translate(const Vector3& vec) {
    mPosition = mPosition + vec;
    mNeedUpdateView = true;
}

// Translate in camera's local axes coordinates.
void Camera::translateLocal(const Vector3& vec) {
    Vector3 trans = mOrientation * vec;
    mPosition = mPosition + trans;
    mNeedUpdateView = true;
}

// Rotate around local Z axis
void Camera::roll(const Radian& angle) {
    rotate(-getForwardVector(), angle);
    mNeedUpdateView = true;
}

// Rotate around local Y axis
void Camera::yaw(const Radian& angle) {
    rotate(getUpVector(), angle);
    mNeedUpdateView = true;
}

// Rotate around local X axis
void Camera::pitch(const Radian& angle) {
    rotate(getRightVector(), angle);
    mNeedUpdateView = true;
}

void Camera::rotate(const Vector3& axis, const Radian& angle) {
    Quaternion q(angle, axis);
    rotate(q);
    mNeedUpdateView = true;
}

void Camera::rotate(const Quaternion& quat) {
    // Normalise the quat to avoid cumulative problems with precision (since
    // computers have fixed precisions.
    Quaternion q = quat;
    q.normalise();
    mOrientation = q * mOrientation;
    mNeedUpdateView = true;
}

void Camera::setFieldOfView(const float fovy) {
    mFrustum.setFovy(fovy);
}

void Camera::setProjectionType(const EProjectionType& type) {
    mFrustum.setProjectionType(type);
    mNeedUpdateProjection = true;
}

void Camera::setPosition(const Vector3& vec) {
    mPosition = vec;
    mNeedUpdateView = true;
}

void Camera::setDirection(const Vector3& vec) {
    setForwardVector(vec);
    mNeedUpdateView = true;
}

void Camera::setForwardVector(const Vector3& vec) {
    if (vec == Vector3::ZERO) {
        return;
    }

    // Camera points down -Z of local axes. So reverse direction of direction
    // vector before determining local Z
    Vector3 newZvec = -vec;
    newZvec.normalise();
    // Get axes from current quaternion.
    Vector3 axes[3];
    mOrientation.ToAxes(axes);  // axes[2] is the old z vec.
    // Derive shortest arc to new direction
    Quaternion rotQuat = axes[2].getRotationTo(newZvec, axes[1]);

    mOrientation = rotQuat * mOrientation;
    mNeedUpdateView = true;
}

bool Camera::isVisible(const AxisAlignedBox& bound) {
    // TODO(jhuang):
//    // First make sure the frustum planes are updated.
//    const Matrix4& viewProj = getViewProjectionMatrix();
//    if (mNeedUpdateFrustum) {
//        mFrustum.updateFrustumPlanes(viewProj);
//    }
//    return mFrustum.isVisible(bound);
    return true;
}

void Camera::update(Viewport* vp) {
    if (mNeedUpdateProjection) {
        updateProjection();
    }
    if (mNeedUpdateView) {
        updateView();
    }
}

void Camera::render(Viewport* vp) {
    assert(mSceneMgr != NULL);
    mSceneMgr->renderScene(vp, this);
}


#pragma handle selection


// Normalized device screen point has coords range xy: (-1, 1).
SceneObj* Camera::getSelectedSceneObj(const Vector2& normalizedScreenPoint) {
    Line ray = getWorldSpaceRayFromNormalizedScreenPointToViewport(normalizedScreenPoint);
    return mSceneMgr->getIntersectedSceneObj(this, ray);
}


// Get a world space ray that start at the world space position of the given
// normalized screen coords and towards viewport.
// |point| is normalized device coords (xy (-1, 1)).
Line Camera::getWorldSpaceRayFromNormalizedScreenPointToViewport(const Vector2& point) {
    Matrix4 inverseVP = (getProjectionMatrix() * getViewMatrix()).inverse();

    // Form normalized device 3D coords.
    Vector3 nearPoint(point.x, point.y, -1.f);
    // Use midPoint rather than far point to avoid issues with infinite projection
    Vector3 midPoint (point.x, point.y,  0.0f);

    // Get ray origin and ray target on near plane in world space
    Vector3 rayOrigin = inverseVP * nearPoint;
    Vector3 rayTarget = inverseVP * midPoint;

    Vector3 rayDirection = rayTarget - rayOrigin;
    rayDirection.normalise();

    return Line(rayOrigin, rayDirection);
}
