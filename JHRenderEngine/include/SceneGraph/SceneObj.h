//
//  SceneObj.h
//
//  Created by Jingshu Huang on 1/18/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _SceneObj_h
#define _SceneObj_h

#include "Behavior.h"
#include "bound.h"
#include "matrix4.h"
#include "Observer.h"
#include "platformDef.h"
#include "SceneObjFactory.h"
#include "RenderQueue.h"


#define kStartKey  "start"
#define kTargetKey "target"

class SceneManager;
class SceneNode;


typedef string ESceneObjType;
typedef unsigned long long int SceneObjID;

// An abstract class.
// A scene object is produced by scene object factories. It can be attached to a
// scene node.
//
// Note instead of creating a scene obj factory for each game specific scene obj
// class, we just put the create method inside the sceneobj for simplicity.
class SceneObj : public SceneObjFactory {
public:
    SceneObj();
    SceneObj(const string& name, const ESceneObjType& type);
    SceneObj(const string& name, const ESceneObjType& type,
             SceneManager* sceneMgr, const ERenderQueueID& queueID);
    virtual ~SceneObj();

    virtual void reset();
    virtual void deepCopyFrom(SceneObj* other);

    // 1. Scene graph related.

    inline string getFullName() const {
        return stringfyObjID(mObjID) + "_" + mName;
    }
    inline const string& getName() const { return mName; }
    inline void setName(const string& name) { mName = name; }

    static string stringfyObjID(const SceneObjID& nId);
    inline SceneObjID& getID() { return mObjID; }
    inline void setID(const SceneObjID& oid) { mObjID = oid; }

    inline void setObjType(const ESceneObjType& type) { mObjType = type; }
    inline const ESceneObjType& getType() const { return mObjType; }

    inline void setSceneNode(SceneNode* node) { mSceneNode = node; }
    inline SceneNode* getSceneNode(void) { return mSceneNode; }

    inline void setSceneManager(SceneManager* mgr) { mSceneMgr = mgr; }
    inline SceneManager* getSceneManager(void) { return mSceneMgr; }


    // 2. Transformation and behavior related.

    // 2.1 transformation related.

    // 2.1.1 transformation in world space.
    virtual const Vector3& getFullWorldPosition(void);
    virtual const Vector3& getFullWorldScale(void);
    virtual const Quaternion& getFullWorldRotation(void);
    const Matrix4& getFullWorldTransform() const;

    // UNIT_Z is pointing backward.
    virtual Vector3 getForwardVector();
    virtual Vector3 getDirection() { return getForwardVector(); }
    virtual const Quaternion& getOrientation(void);
    virtual Vector3 getUpVector();
    virtual Vector3 getRightVector();

    // 2.1.2 transformation in local space.

    // For camera, the following methods need to be overridden and coords are in
    // screen space. For non-camera scene object, all the transformations are
    // the ones of the attached scene node, which are relative to its parent's node.
    virtual void translate(const Vector3& vec);
    virtual void translateLocal(const Vector3& vec);
    virtual void roll(const Radian& angle);
    virtual void yaw(const Radian& angle);
    virtual void pitch(const Radian& angle);
    virtual void rotate(const Vector3& axis, const Radian& angle);
    virtual void rotate(const Quaternion& quat);

    // 2.2 behavior related.
    SceneNode* getClosestReactiveParentNode(const BehaviorType& type);
    Behavior* getBehavior(const BehaviorType& type);

    // 3. Bound related.

    // Get the bound of the scene object in object space.
    virtual const Bound& getObjectBound(void) const { return mObjBound; }
    // Get the bound of the scene object in world space.
    virtual const Bound& getWorldBound(const bool needUpdate) const;


    // 4. Rendering related.
    inline void setRenderQueueID(const ERenderQueueID& qID) { mRenderQueueID = qID; }
    inline ERenderQueueID getRenderQueueID(void) { return mRenderQueueID; }
    RenderQueue* getRenderQueue() const;

    virtual bool isVisible() const { return true; };
    virtual void updateRenderQueue(RenderQueue* queue) = 0;
    virtual void updateOnNotificationImpl(Observable* observable,
                                          Notification* msg) {
        // Do nothing.
    }

protected:
    SceneObjID mObjID;
    string mName;                  // user specified name.
    ESceneObjType mObjType;
    
    ERenderQueueID mRenderQueueID; // The render queue the object will be in.
    SceneManager* mSceneMgr;
    SceneNode* mSceneNode;         // scene node the object is attached to.

    // Bound in object space. For entity, it contains all the meshes of all
    // child entities. For camera, it contains the frustum.
    mutable Bound mObjBound;

    // Cached world AABB of this object. It is computed from the all the node
    // transforms on the path to this node and the mesh bound the object refers
    // to (in an entity case).
    mutable Bound mWorldBound;

private:
    static SceneObjID nextObjNamePrefix;
};

#endif
