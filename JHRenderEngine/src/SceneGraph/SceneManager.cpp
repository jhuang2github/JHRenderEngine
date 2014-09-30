//
//  SceneManager.cpp
//
//  Created by Jingshu Huang on 1/16/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include <sstream>

#include "BehaviorManager.h"
#include "Camera.h"
#include "Entity.h"
#include "Node.h"
#include "Renderable.h"
#include "Renderer.h"
#include "RenderQueue.h"
#include "SceneNode.h"
#include "SceneNodeIterator.h"
#include "SceneObj.h"
#include "SceneMananger.h"
#include "SceneQuery.h"
#include "Viewport.h"


const static string kCameraKey = "camera";


SceneManager::SceneManager() {
    mRenderer = NULL;
    mSceneGraph.root = NULL;
    registerSceneNodeFactory(kDefaultSceneNodeClass,
                             new SceneNode(this, kDefaultSceneNodeClass, ""));
}

SceneManager::~SceneManager() {
    clearSceneObjects();
    clearSceneNodes();
    mCameras.clear();
}


#pragma mark - Scene object related

void SceneManager::clearSceneObjects() {
    for (SceneObjGroupMap::iterator iter = mSceneObjGroups.begin();
         iter != mSceneObjGroups.end(); ++iter) {
        ThreadSafeSceneObjMap* objMap = iter->second;
        if (objMap) {
            for (SceneObjBaseNameMap::iterator k = objMap->sceneObjs.begin();
                 k != objMap->sceneObjs.end(); ++k) {
                // If it is a camera, first clean it up in the mCameras map.
                // CameraMap uses scene obj base name as key.
                CameraMap::iterator camIter = mCameras.find(k->first);
                if (camIter != mCameras.end()) {
                    mCameras.erase(camIter);
                }

                clearSceneObjMap(k->second);
            }
            objMap->sceneObjs.clear();
        }
    }
    mSceneObjGroups.clear();
}

void SceneManager::clearSceneObjMap(SceneObjectMap& map) {
    for (SceneObjectMap::iterator it = map.begin(); it != map.end(); ++it) {
        if (it->second) {
            delete it->second;
        }
    }
    map.clear();
}

void SceneManager::registerSceneObjFactory(const string& sceneObjClassName,
                                           SceneObj* objFactory) {
    SceneObjFactoryMap::iterator iter = mSceneObjFactoryMap.find(sceneObjClassName);
    if (iter != mSceneObjFactoryMap.end()) {
        LOGD("[SceneManager registerSceneObjFactory] "
             "Error: factory for %s already registered.\n",
             sceneObjClassName.c_str());
        return;
    }
    mSceneObjFactoryMap[sceneObjClassName] = objFactory;
}

SceneObj* SceneManager::getSceneObjFactory(const string& sceneObjName) const {
    SceneObjFactoryMap::const_iterator iter = mSceneObjFactoryMap.find(sceneObjName);
    if (iter != mSceneObjFactoryMap.end()) {
        return iter->second;
    }
    return NULL;
}

Entity* SceneManager::createEntity(const string& entityName,
                                   const string& entityClassName,
                                   const string& meshName) {
	KeyValuePairStringMap params;
	params[Entity::kMeshKey] = meshName;
	return static_cast<Entity*>(createSceneObj(entityName,
                                               entityClassName,
                                               &params));
}

Entity* SceneManager::createEntity(const string& entityName,
                                   const string& entityClassName,
                                   const string& meshName,
                                   const string& materialName,
                                   const ERenderQueueID& queueID) {
	Entity* entity = createEntity(entityName, entityClassName, meshName, queueID);
    entity->setMaterial(materialName);
    return entity;
}

Entity* SceneManager::createEntity(const string& entityName,
                                   const string& entityClassName,
                                   const string& meshName,
                                   const ERenderQueueID& queueID) {
	KeyValuePairStringMap params;
	params[Entity::kMeshKey] = meshName;

    std::stringstream stream;
    stream << queueID;
    params[Entity::kRenderQueueIDKey] = stream.str();

	return static_cast<Entity*>(createSceneObj(entityName,
                                               entityClassName,
                                               &params));
}

Camera* SceneManager::createCamera(const string& cameraName,
                                   const string& cameraClassName) {
    CameraMap::iterator iter = mCameras.find(cameraName);
    if (iter != mCameras.end()) {
        LOGE("ERROR: Found two cameras that have the same name %s. "
             "Skip the creation of the second camera.", cameraName.c_str());
        return iter->second;
    }

	Camera* camera = static_cast<Camera*>(createSceneObj(cameraName,
                                                         cameraClassName,
                                                         NULL));
    camera->setSceneManager(this);
    mCameras[camera->getName()] = camera;
    return camera;
}

SceneObj* SceneManager::createSceneObj(const string& objName,
                                       const string& objClassName,
                                       const KeyValuePairStringMap* params) {
	SceneObj* factory = getSceneObjFactory(objClassName);
    assert(factory != NULL);
	ThreadSafeSceneObjMap* objMap = getSceneObjMap(objClassName);
    assert(objMap != NULL);

    SceneObj* obj = NULL;
    obj = factory->create(this, objName, params);
    assert(obj != NULL);
    obj->setSceneManager(this);

    SceneObjBaseNameMap::iterator iter = objMap->sceneObjs.find(objName);
    if (iter == objMap->sceneObjs.end()) {
        SceneObjectMap map;
        map.insert(SceneObjectMap::value_type(obj->getFullName(), obj));
        objMap->sceneObjs.insert(SceneObjBaseNameMap::value_type(objName, map));
    } else {
        iter->second.insert(SceneObjectMap::value_type(obj->getFullName(), obj));
    }

    return obj;
}

void SceneManager::deleteSceneObj(const string& objName,
                                  const string& objClassName) {
	ThreadSafeSceneObjMap* objMap = getSceneObjMap(objClassName);
    assert(objMap != NULL);

    SceneObjBaseNameMap::iterator iter = objMap->sceneObjs.find(objName);
    if (iter != objMap->sceneObjs.end()) {
        // If it is a camera, first clean it up in the mCameras map.
        // CameraMap uses scene obj base name as key.
        CameraMap::iterator camIter = mCameras.find(iter->first);
        if (camIter != mCameras.end()) {
            mCameras.erase(camIter);
        }

        // Remove the instance.
//			getSceneObjFactory(objClassName)->destroy(iter->second);

        SceneObjectMap& map = iter->second;
        if (map.size() > 1) {
            LOGD("Warning: [SceneManager deleteSceneObj]: multiple scene obj "
                 "with the name %s will be deleted.\n", objName.c_str());
        }
        clearSceneObjMap(iter->second);
        objMap->sceneObjs.erase(iter);
    }
}

Camera* SceneManager::getCamera(const string& name) {
    CameraMap::iterator iter = mCameras.find(name);
    if (iter != mCameras.end()) {
        return iter->second;
    }
    return NULL;
}

SceneNode* SceneManager::getCameraNode(const string& cameraName) {
    Camera* cam = getCamera(cameraName);
    return cam ? cam->getSceneNode() : NULL;
}

Entity* SceneManager::getEntity(const string& objName,
                                const string& objClassName) {
    return static_cast<Entity*>(getSceneObj(objName, objClassName));
}

Entity* SceneManager::getEntity(const string& objName,
                                 const string& objClassName,
                                 SceneNode* root) {
    return static_cast<Entity*>(getSceneObj(objName, objClassName, root));
}

SceneObj* SceneManager::getSceneObj(const string& objName,
                                    const string& objClassName) {
	ThreadSafeSceneObjMap* objMap = getSceneObjMap(objClassName);
    assert(objMap != NULL);
    SceneObjBaseNameMap::iterator iter = objMap->sceneObjs.find(objName);
    if (iter != objMap->sceneObjs.end()) {
        SceneObjectMap& map = iter->second;
        int count = map.size();
        assert(count > 0);
        if (count > 1) {
            LOGD("[SceneManager getEntity]: Warning: found multiple entities "
                 "with the base name %s.\n", objName.c_str());
        }
        return map.begin()->second;
    }

    return NULL;
}

SceneObj* SceneManager::getSceneObj(const string& objName,
                                    const string& objClassName,
                                    SceneNode* start) {
    SceneNodeIterator iter(start);
    while(iter.hasNext()) {
        SceneNode& node = *iter;
        SceneObjectMap& map = node.getAttachedObjects();
        SceneObjectMap::iterator it = map.find(objName);
        if (it != map.end()) {
            return it->second;
        }
        iter.advance();
    }
    return NULL;
}

ThreadSafeSceneObjMap* SceneManager::getSceneObjMap(const string& objClassName) {
    SceneObjGroupMap::const_iterator iter = mSceneObjGroups.find(objClassName);
    if (iter != mSceneObjGroups.end()) {
        return iter->second;
    }
    ThreadSafeSceneObjMap* objMap = new ThreadSafeSceneObjMap;
    mSceneObjGroups[objClassName] = objMap;
    return objMap;
}

SceneObj* SceneManager::getIntersectedSceneObj(Camera* cam, const Line& ray) {
    RaySceneQuery rayQuery(*this, ray, cam);
    RaySceneQueryResult& result = rayQuery.execute();
    if (result.size() > 0) {
        return result[0].sceneObj;
    }
    return NULL;
}


#pragma mark - Scene node related

void SceneManager::clearSceneNodes() {
	getRootSceneNode()->removeAllChildren();
	for (SceneNodeMap::iterator iter = mSceneNodes.begin();
         iter != mSceneNodes.end(); ++iter) {
		delete iter->second;
	}
	mSceneNodes.clear();
    
    if (mSceneGraph.root) {
        delete mSceneGraph.root;
        mSceneGraph.root = NULL;
    }
}

void SceneManager::registerSceneNodeFactory(const string& sceneNodeClassName,
                                            SceneNode* nodeFactory) {
    if (getSceneNodeFactory(sceneNodeClassName)) {
        LOGD("[SceneManager registerSceneNodeFactory] Error: factory for %s already registered.\n",
             sceneNodeClassName.c_str());
        return;
    }
    mSceneNodeFactoryMap[sceneNodeClassName] = nodeFactory;
}

SceneNode* SceneManager::getSceneNodeFactory(const string& sceneNodeClassName) const {
    SceneNodeFactoryMap::const_iterator iter = mSceneNodeFactoryMap.find(sceneNodeClassName);
    if (iter != mSceneNodeFactoryMap.end()) {
        return iter->second;
    }
    return NULL;
}

SceneNode* SceneManager::createChildSceneNode(const string& nodeClassName, SceneNode* parent,
                                              const string& nodeNameSuffix) {
    SceneNode* node = createSceneNode(nodeClassName, nodeNameSuffix);
    parent->addChild(node);
    return node;
}

SceneNode* SceneManager::createSceneNode(const string& nodeClassName,
                                         const string& nodeNameSuffix) {
    SceneNode* factory = getSceneNodeFactory(nodeClassName);
    assert(factory != NULL && "Error in finding the factory to create scene node.");
    SceneNode* node = factory->create(this, nodeNameSuffix);
    NodeID nodeId = node->getID();
    assert(mSceneNodes.find(nodeId) == mSceneNodes.end());
    mSceneNodes[nodeId] = node;
    return node;
}

SceneNode* SceneManager::getRootSceneNode() {
	if (!mSceneGraph.root) {
		mSceneGraph.root = createSceneNode(kDefaultSceneNodeClass, "");
	}
    return mSceneGraph.root;
}

SceneNode* SceneManager::getSceneNode(const string& name) {
    // Early termination for camera node.
    SceneNode* node = getCameraNode(name);
    if (node) {
        return node;
    }

    SceneNodeIterator iter(getRootSceneNode());
    while(iter.hasNext()) {
        SceneNode& node = *iter;
        const string& nodeName = node.getName();
        const NodeID& nodeID = node.getID();
        if (nodeName == node.createName(nodeID, name)) {
            return iter.current();
        }
        iter.advance();
    }
    return NULL;
}


#pragma mark - Update and render related

// Update scene graph.
void SceneManager::updateScene(const float secondsPassed) {
    getRootSceneNode()->update(secondsPassed);
}

void SceneManager::renderScene(Viewport* vp, Camera* cam) {
    assert(mRenderer != NULL);
    mRenderer->clearRenderQueues();

    // Find visible scene objects and insert them into the render queues.
    getRootSceneNode()->findVisibleObjects(cam);

    assert(mRenderer != NULL);
    mRenderer->render(vp, cam);
}
