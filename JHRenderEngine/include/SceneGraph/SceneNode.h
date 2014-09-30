//
//  SceneNode.h
//
//  Created by Jingshu Huang on 1/21/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _SceneNode_h
#define _SceneNode_h

#include "bound.h"
#include "Camera.h"
#include "Node.h"

typedef unordered_map<string, SceneObj*> SceneObjectMap;

class RenderableBound;
class RenderQueue;
class SceneManager;
class SceneObj;

#define kDefaultSceneNodeClass "SceneNode"


// A node in scene graph.
class SceneNode : public Node {
public:
    SceneNode(SceneManager* manager, const NodeType& type,
              const string& nodeNameSuffix);
    ~SceneNode();

    // Factory method.
    virtual SceneNode* create(SceneManager* mgr, const string& nodeNameSuffix);
    virtual SceneNode* createChild();
    virtual SceneNode* createChild(const string& nodeClass, const string& nameSuffix);


    // Override.
    virtual void deepCopyFrom(Node* other);
    virtual void nodeChildrenCopyImpl(Node* other);

    virtual void attachedObjectsCopyImpl(SceneNode* other);

    inline void toggleVisibility(void) { mHidden = !mHidden; }
    inline void setHidden(const bool hide) { mHidden = hide; }
    inline bool isHidden(void) { return mHidden; }

//    virtual bool isRenderableNode(void) { return mNodeType != CameraType; }

    inline void setShowBound(const bool show) { mShowBound = show; }
    inline bool getShowBound(void) { return mShowBound; }

    inline Bound& getWorldBound(void) { return mWorldBound; }
    inline AxisAlignedBox& getWorldBoundAABB(void) { return mWorldBound.getAABB(); }

    // Override. Reset/unload the current scene.
    void reset();

    inline SceneManager* getSceneManager() const { return mSceneMgr; }
    inline SceneObjectMap& getAttachedObjects() { return mObjects; }
    inline SceneObj* getFirstAttachedObject() {
        return mObjects.size() > 0 ? mObjects.begin()->second : NULL;
    }
    bool load(const string& fileName);
    bool load();

    virtual ushort numAttachedObjects() const;
    virtual SceneObj* getAttachedObject(const string& name);
    virtual void attachObject(SceneObj* obj);
    virtual void detachObject(const string& name);

    inline void addVisibleCamera(const string& cameraName) {
        mVisibleCameras.insert(cameraName);
    }
    bool isVisible(Camera* cam);
    void findVisibleObjects(Camera* cam);

    // Override.
    virtual void update(const float secondsPassed);
    virtual void updateOnNotification(Observable* observable, Notification* msg);

protected:
    // Override.
    void updateFromParent(void) const;
    void updateBounds(void);

    // Debug use. Add bound to render queue for drawing.
    void addBoundToRenderQueue();

    Bound mWorldBound;        // Bound in world space.
    RenderableBound* mRenderableWorldBound;
    bool  mShowBound;

    string getBoundMeshName();

protected:
    SceneManager* mSceneMgr;  // The node creator.
    string mPath;             // Full path to scene description file.
    bool  mHidden;

    // Map from a scene object name (without sceneObjID) to the scene object.
    // In one scene node, there should NOT be scene objects that have the same
    // name. |mObjects| does not own the data.
    SceneObjectMap mObjects;

    // A set that stores the names of cameras that can see the node (and all the
    // child nodes unless defined otherwise by the child node). This info is
    // defined through scene description file, and independent from the bounding
    // box visibility test. When the set is empty, it means the node is
    // visible by all the cameras defined or defined by its parent nodes (if
    // any of its parent node has this field set).
    typedef unordered_set<string> VisibleCameraSet;
    VisibleCameraSet mVisibleCameras;

    friend class SceneNodeIterator;
};


#endif
