//
//  SceneObjFactory.cpp
//
//  Created by Jingshu Huang on 1/18/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include <sstream>

#include "Entity.h"
#include "Camera.h"
#include "MeshManager.h"
#include "SceneMananger.h"
#include "SceneObj.h"
#include "SceneObjFactory.h"


//template<> string EntityFactory::kFactoryType = "Entity";
//template<> string CameraFactory::kFactoryType = "Camera";
//
//
//template<> SceneObj* EntityFactory::create(SceneManager* sceneMgr,
//                                           const string& entityName,
//                                           const KeyValuePairStringMap* params) {
//    Mesh* mesh = NULL;
//    uint queueID = QUEUE_5_MAIN;
//    if (params) {
//        KeyValuePairStringMap::const_iterator iter = params->find(kMeshKey);
//        if (iter != params->end()) {
//            // Load the mesh.
//            mesh = MeshManager::getInstance()->load(iter->second);
//        }
//        iter = params->find(kRenderQueueIDKey);
//        if (iter != params->end()) {
//            stringstream stream(iter->second);
//            stream >> queueID;
//        }
//    }
//
//    if (!mesh) {
//        LOGD("EntityFactory::create(): Need to specify mesh when constructing an entity.");
//    }
//
//    return new Entity(entityName, mesh, sceneMgr, this, static_cast<ERenderQueueID>(queueID));
//}
//
//template<> void EntityFactory::destroy(SceneObj* obj) {
//    Entity* ent = static_cast<Entity*>(obj);
//    if (ent) {
//        delete ent;
//    }
//}
//
//
//
//template<> SceneObj* CameraFactory::create(SceneManager* sceneMgr,
//                                           const string& cameraName,
//                                           const KeyValuePairStringMap* params) {
//    return new Camera(cameraName, sceneMgr, this);
//}
//
//template<> void CameraFactory::destroy(SceneObj* obj) {
//    Camera* cam = static_cast<Camera*>(obj);
//    if (cam) {
//        delete cam;
//    }
//}
