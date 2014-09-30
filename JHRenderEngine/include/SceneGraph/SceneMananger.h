//
//  SceneMananger.h
//
//  Created by Jingshu Huang on 1/14/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _SceneMananger_h
#define _SceneMananger_h

#include <map>

#include "AssetManager.h"
#include "Mesh.h"
#include "Node.h"
#include "platformDef.h"
#include "RenderQueue.h"
#include "SceneNode.h"
#include "Singleton.h"
#include "vector2.h"


class Camera;
class Entity;
class GameControllerBase;
class Renderable;
class Renderer;
class RenderQueue;
class SceneNode;
class Viewport;


// |SceneObjBaseNameMap| maps from a scene object base name (no SceneObjID
// appended) to a SceneObjMap.
// |SceneObjMap| maps from a scene object full name (with SceneObjID) to the
// object handler.
typedef unordered_map<string, SceneObjectMap> SceneObjBaseNameMap;

typedef struct {
    SceneObjBaseNameMap sceneObjs;
} ThreadSafeSceneObjMap;

typedef map<NodeID, SceneNode*> SceneNodeMap;
typedef struct {
    // Root scene graph node. Note cameras are not in this scene graph although
    // they are in the sceneObjGroups.
    SceneNode* root;
} ThreadSafeSceneGraph;


// Scene manager handles:
// * create scene objects like entities, cameras, lights, particle systems, etc
// * organize and sort the scene objects.
// * support queries such as ray intersection, occlusion, culling, etc.
// * pass organized the data to a renderer.
// * culling is based on a hierarchy of node bounding boxes
class SceneManager : public Singleton<SceneManager> {
public:
    /* 1. Scene objects related. */
    
    // Scene objects creation / deletion.
    void registerSceneObjFactory(const string& sceneObjClassName,
                                 SceneObj* objFactory);
    SceneObj* getSceneObjFactory(const string& sceneObjClassName) const;

    SceneObj* createSceneObj(const string& objName,
                             const string& typeName,
                             const KeyValuePairStringMap* params);
    void deleteSceneObj(const string& objName,
                        const string& typeName);

    ThreadSafeSceneObjMap* getSceneObjMap(const string& typeName);

    Entity* createEntity(const string& entityName,
                         const string& entityClassName,
                         const string& meshName);
    Entity* createEntity(const string& entityName,
                         const string& entityClassName,
                         const string& meshName,
                         const ERenderQueueID& queueID);
    Entity* createEntity(const string& entityName,
                         const string& entityClassName,
                         const string& meshName,
                         const string& materialName,
                         const ERenderQueueID& queueID);
    Camera* createCamera(const string& cameraName,
                         const string& cameraClassName);
    Camera* getCamera(const string& cameraName);
    SceneNode* getCameraNode(const string& cameraName);
    Entity* getEntity(const string& objName,
                      const string& objClassName);
    Entity* getEntity(const string& objName,
                      const string& objClassName,
                      SceneNode* root);
    SceneObj* getSceneObj(const string& objName,
                          const string& objClassName);
    SceneObj* getSceneObj(const string& objName,
                          const string& objClassName,
                          SceneNode* root);

    // Get ray intersected closest scene object visible by the camera.
    SceneObj* getIntersectedSceneObj(Camera* cam, const Line& ray);


    /* 2. Scene node related. */
    void registerSceneNodeFactory(const string& sceneNodeClassName,
                                 SceneNode* nodeFactory);
    SceneNode* getSceneNodeFactory(const string& sceneNodeClassName) const;
    SceneNode* createChildSceneNode(const string& nodeClassName, SceneNode* parent,
                                    const string& nodeNameSuffix);
    SceneNode* createSceneNode(const string& nodeClassName, const string& nodeNameSuffix);
    SceneNode* getRootSceneNode();
    SceneNode* getSceneNode(const string& name);

    /* 3. Scene update and rendering related. */

    void updateScene(const float secondsPassed);
    void renderScene(Viewport* vp, Camera* cam);

    inline void setRenderer(Renderer* r) { mRenderer = r; }
    inline Renderer* getRenderer() { return mRenderer; }
    inline void setGameController(GameControllerBase* g) { mGameController = g; }
    inline GameControllerBase* getGameController(void) const { return mGameController; }

    SceneManager();
    ~SceneManager();

private:
    void clearSceneNodes();
    void clearSceneObjects();
    void clearSceneObjMap(SceneObjectMap& map);

private:
    // Convenient handlers to renderer and game controller. Do not own data.
    Renderer* mRenderer;
    GameControllerBase* mGameController;

    // 1. Scene node related.

    // Scene nodes in the scene graph keyed by their names.
    // Owns the SceneNode data.
    SceneNodeMap mSceneNodes;
    ThreadSafeSceneGraph mSceneGraph;
    // Map from a scene node class name to scene node factory handler.
    // Note we actually store a dummy scene node as the factory handler.
    typedef unordered_map<string, SceneNode*> SceneNodeFactoryMap;
    SceneNodeFactoryMap mSceneNodeFactoryMap;


    // 2. Scene object, including camera, related.

    // Scene objects grouped by their class name. Owns the data.
    typedef unordered_map<string, ThreadSafeSceneObjMap*> SceneObjGroupMap;
    SceneObjGroupMap mSceneObjGroups;
    // Map from a scene object class name to scene factory handler.
    // Note we actually store a dummy scene obj as the factory handler.
    typedef unordered_map<string, SceneObj*> SceneObjFactoryMap;
    SceneObjFactoryMap mSceneObjFactoryMap;

    // A convenient list for scene cameras. It does not own the data. The data
    // is owned by SceneObjGroups. The key is the scene object base name
    // (without sceneObjID).
    typedef unordered_map<string, Camera*> CameraMap;
    CameraMap mCameras;
};

#endif
