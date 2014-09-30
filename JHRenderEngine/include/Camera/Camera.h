//
//  Camera.h
//
//  Created by Jingshu Huang on 12/20/12.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Camera_h
#define _Camera_h

#include "axisAlignedBox.h"
#include "Frustum.h"
#include "platformDef.h"
#include "SceneObj.h"
#include "matrix4.h"
#include "vector3.h"

#define kDefaultCameraClass "Camera"

class Camera;
class SceneManager;
class Viewport;


// Note: camera's transformation is relative to the scene node it is attached to
// since it is also a scene object.
class Camera : public SceneObj {
public:
    Camera(const string& name, const ESceneObjType& type, SceneManager* mgr);
    virtual ~Camera();

    // Override factory method.
    virtual SceneObj* create(SceneManager* sceneMgr,
                             const string& objName,
                             const KeyValuePairStringMap* params = 0);
    virtual void deepCopyFrom(SceneObj* other);
    void reset();

    // Getters and setters.
    inline void setSceneManager(SceneManager* mgr) { mSceneMgr = mgr; }
    void setProjectionType(const EProjectionType& type);
    void setPosition(const Vector3& vec);
    void setForwardVector(const Vector3& vec);
    void setDirection(const Vector3& vec);
    void setFieldOfView(const float fovy);

    // Override. In world space.
    inline const Vector3& getFullWorldPosition(void) { return mPosition; }
    inline const Vector3& getFullWorldScale(void) { return Vector3::UNIT_SCALE; }
    inline const Quaternion& getFullWorldRotation(void) { return mOrientation; }
    // UNIT_Z is pointing backward.
    inline Vector3 getForwardVector() { return mOrientation * -Vector3::UNIT_Z; }
    inline const Quaternion& getOrientation(void) { return mOrientation; }
    inline Vector3 getUpVector() { return mOrientation * Vector3::UNIT_Y; }
    inline Vector3 getRightVector() { return mOrientation * Vector3::UNIT_X; }

    void lookAt(const Vector3& targetPoint);
    void translate(const Vector3& vec);
    // Override. In local (screen) space.
    void translateLocal(const Vector3& vec);
    void roll(const Radian& angle);
    void yaw(const Radian& angle);
    void pitch(const Radian& angle);
    void rotate(const Vector3& axis, const Radian& angle);
    void rotate(const Quaternion& quat);

    inline const Matrix4& getProjectionMatrix() const { return mProjMatrix; }
    inline Matrix4* getProjectionMatrixPtr() { return &mProjMatrix; }
    inline const Matrix4& getViewMatrix() const { return mViewMatrix; }
    inline Matrix4* getViewMatrixPtr() { return &mViewMatrix; }
    const Matrix4& getViewProjectionMatrix();
    Matrix4* getViewProjectionMatrixPtr();
    inline const Frustum& getFrustum() const { return mFrustum; }

    // If an AABB is visible by the camera.
    bool isVisible(const AxisAlignedBox& bound);

    void render(Viewport* vp);
    void update(Viewport* vp);
    virtual void updateRenderQueue(RenderQueue* queue) {}

    SceneObj* getSelectedSceneObj(const Vector2& normalizedScreenPoint);
    Line getWorldSpaceRayFromNormalizedScreenPointToViewport(const Vector2& point);

    static Vector3 kIsometricCameraPosition;

    static const string kDefaultCameraName;

protected:
    Camera(const string& name, const ESceneObjType& type);

    void updateProjection();
    void updateView();

    // Right-hand coord. Z pointing out of screen. User specified position in
    // world space.
    Vector3 mPosition;
    Quaternion mOrientation;

    Matrix4 mProjMatrix;     // a row-major matrix. Need transpose when sending to GPU
    Matrix4 mViewMatrix;     // a row-major matrix. Need transpose when sending to GPU
    Matrix4 mViewProjMatrix; // a row-major matrix. Need transpose when sending to GPU
    Frustum mFrustum;

    bool mNeedUpdateProjection;
    bool mNeedUpdateView;
    bool mNeedUpdateViewProjection;
    bool mNeedUpdateFrustum;

private:
    static bool sFactoryInitialized;
    static bool registerFactory();

    Camera() { assert(false && "Camera::Camera default constructor is called."); }
};

#endif
