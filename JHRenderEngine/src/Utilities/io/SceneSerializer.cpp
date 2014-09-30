//
//  SceneSerializer.cpp
//
//  Created by Jingshu Huang on 8/5/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include <cstdio>

#include "Behavior.h"
#include "BehaviorManager.h"
#include "Entity.h"
#include "DataStream.h"
#include "Renderer.h"
#include "RenderWindow.h"
#include "SceneMananger.h"
#include "SceneNode.h"
#include "SceneObj.h"
#include "SceneSerializer.h"
#include "SerializerManager.h"
#include "Viewport.h"


// Scene description file key words.
static const char* kClassKey = "class";
static const char* kNodeClassKey = "nodeClass";  // Scene node class
static const char* kObjClassKey = "objClass";    // Scene object class
static const char* kEntityGroupKey = "entity_group";
static const char* kEntityKey = "entity";
static const char* kCameraKey = "camera";
static const char* kHiddenKey = "hidden";
static const char* kNameKey = "name";
static const char* kTransformKey = "transform";
static const char* kPositionKey = "position";
static const char* kScaleKey = "scale";
static const char* kRotateKey = "rotate";
static const char* kLookAtKey = "lookAt";
static const char* kFieldOfView = "fieldOfView";
static const char* kProjectionKey = "projection";
static const char* kModelKey = "model";
static const char* kMaterialKey = "material";
static const char* kQueueKey = "queue";
// When using default material, allow scene file to overwrite the texture.
static const char* kTexturesKey = "textures";

// Allow an entity to override the render pass' render state.
static const char* kRenderStateKey = "render_state";
static const char* kDepthTestKey = "depth_test";
static const char* kDepthWriteKey = "depth_write";
static const char* kBlendKey = "blend";
static const char* kCullFaceKey = "cull_face";

// Behavior related.
static const char* kBehaviorKey = "behaviors";
static const char* kNotificationKey = "notification";
static const char* kObserversKey = "observers";

// Support include scene file hierarchy.
static const char* kIncludeKey = "include";

// Bound.
static const char* kShowBoundsKey = "show_bounds";


void SceneSerializer::importScene(DataStream& stream, SceneNode* node) {
    mSceneMgr = node->getSceneManager();
    assert(mSceneMgr != NULL);
    importSceneDOM(stream, node);
}


Viewport* SceneSerializer::getDefaultViewport() {
    return mSceneMgr->getRenderer()->getRenderTarget()->getDefaultViewport();
}


void SceneSerializer::importSceneDOM(DataStream & stream, SceneNode* node) {
    Document doc;
    doc.Parse<0>(stream.getData());
    assert(doc.IsObject() && "Error: Invalid scene description file found. Please check the correctness of the syntax.\n");
    handleNode(doc, node);
}

void SceneSerializer::handleNode(const Value& entries, SceneNode* node) {
    handleNodeProperties(entries, node);
    
    // Then recursively handle scene objects and scenen nodes.
    for (Value::ConstMemberIterator entryIter = entries.MemberBegin();
         entryIter != entries.MemberEnd(); ++entryIter) {
        const Value& entryKey = entryIter->name;
        const Value& entryValue = entryIter->value;
        assert(entryKey.IsString());
        
        string key = entryKey.GetString();
        if (key == kEntityKey) {
            handleEntity(entryValue, createChildSceneNode(node, entryValue));
        } else if (key == kEntityGroupKey) {
            handleNode(entryValue, createChildSceneNode(node, entryValue));
        } else if (key == kIncludeKey) {
            assert(entryValue.IsString());
            handleInclude(entryValue.GetString(), kDefaultSceneNodeClass,
                          createChildSceneNodeWithNameSuffix(node, ""));
        } else if (key == kCameraKey) {
            if (entryValue.IsObject()) {
                handleCamera(entryValue, createChildSceneNode(node, entryValue));
            } else if (entryValue.IsString()) {
                // This is not a camera definition, but a referance to a camera.
                string cameraName = entryValue.GetString();
                node->addVisibleCamera(cameraName);
                Camera* camera = mSceneMgr->getCamera(cameraName);
                assert(camera != NULL && "Error: camera is null. "
                       "Please define camera before using to it.");
                getDefaultViewport()->addCamera(camera);
                
            }
        } else {
            // Assume it is a simple node property and it has been handled.
        }
    }
}


void SceneSerializer::handleNodeProperties(const Value& prop, SceneNode* node) {
    string name = "";
    const Value& nameVal = prop[kNameKey];
    if (nameVal.IsString()) {
        name = nameVal.GetString();
        if (!name.empty()) {
            node->setName(node->getID(), name);
        }
    }

    bool hide = false;
    const Value& hiddenVal = prop[kHiddenKey];
    if (hiddenVal.IsBool()) {
        if (hiddenVal.GetBool()) {
            hide = true;
        }
    }
    node->setHidden(hide);
    
    bool showBound = false;
    const Value& showBoundVal = prop[kShowBoundsKey];
    if (showBoundVal.IsBool()) {
        showBound = showBoundVal.GetBool();
        node->setShowBound(showBound);
    }

    handleNodeTransformProperty(prop, node);
    handleNodeBehaviorsProperty(prop, node);
}

void SceneSerializer::handleNodeTransformProperty(const Value &prop, SceneNode *node) {
    const Value& info = prop[kTransformKey];
    if (info.IsObject()) {
        const Value& translateVal = info[kPositionKey];
        if (translateVal.IsArray()) {
            node->translate(Vector3(translateVal[(SizeType)0].GetDouble(),
                                    translateVal[(SizeType)1].GetDouble(),
                                    translateVal[(SizeType)2].GetDouble()));
        }
        const Value& scaleVal = info[kScaleKey];
        if (scaleVal.IsArray()) {
            node->scale(Vector3(scaleVal[(SizeType)0].GetDouble(),
                                scaleVal[(SizeType)1].GetDouble(),
                                scaleVal[(SizeType)2].GetDouble()));
        }
        const Value& rotateVal = info[kRotateKey];
        if (rotateVal.IsArray()) {
            Vector3 rot = Vector3(deg2rad(rotateVal[(SizeType)0].GetDouble()),
                                  deg2rad(rotateVal[(SizeType)1].GetDouble()),
                                  deg2rad(rotateVal[(SizeType)2].GetDouble()));
            Quaternion quat(rot.x, rot.y, rot.z);
            node->rotate(quat);
        }
    }
}

void SceneSerializer::handleNodeBehaviorsProperty(const Value& prop, SceneNode* node) {
    const Value& behaviorsVal = prop[kBehaviorKey];
    if (!behaviorsVal.IsArray()) {
        return;
    }
    BehaviorManager* behaviorMgr = BehaviorManager::getInstance();

    SizeType count = behaviorsVal.Size();
    for (SizeType i = 0; i < count; ++i) {
        const Value& def = behaviorsVal[i];
        if (def.IsObject()) {  // behavior is defined in a dictionary.
            const Value& classNameObj = def[kClassKey];
            if (!classNameObj.IsString()) {
                continue;
            }
            const string& className = classNameObj.GetString();

            NotificationID notif = 0;
            const Value& notifVal = def[kNotificationKey];
            if (notifVal.IsInt()) {
                notif = notifVal.GetInt();
            }

            Behavior* behavior = behaviorMgr->createBehavior(className, node, notif);
            if (!behavior) {
                continue;
            }

            const Value& observersVal = def[kObserversKey];
            if (observersVal.IsArray()) {
                // We can only add observers after all the scene files are processed.
                // So here we just store the names of observers.
                int obCount = observersVal.Size();
                for (int k = 0; k < obCount; ++k) {
                    behavior->addObserverName(observersVal[k].GetString());
                }
            }
        }
    }
}


void SceneSerializer::handleEntity(const Value& entityInfo, SceneNode* node) {
    handleNodeProperties(entityInfo, node);
    handleEntityProperties(entityInfo, node);
}


void SceneSerializer::handleEntityProperties(const Value& info, SceneNode* node) {
    string name = "";
    if (info[kNameKey].IsString()) {
        name = info[kNameKey].GetString();
    }
    
    string entityClassName = kDefaultEntityClass;
    const Value& classVal = info[kObjClassKey];
    if (classVal.IsString()) {
        entityClassName = classVal.GetString();
    }
    
    string model = Mesh::kDefaultMeshName;
    if (info[kModelKey].IsString()) {
        model = info[kModelKey].GetString();
    }
    
    string material = Material::kDefaultMaterialName;
    vector<string> textures;
    if (info[kMaterialKey].IsString()) {
        material = info[kMaterialKey].GetString();
    } else if (info[kTexturesKey].IsArray()) {
        const Value& texes = info[kTexturesKey];
        SizeType count = texes.Size();
        for (SizeType i = 0; i < count; ++i) {
            textures.push_back(texes[i].GetString());
        }
    }
    
    ERenderQueueID queueId = QUEUE_5_MAIN;
    const Value& queueIdVal = info[kQueueKey];
    if (queueIdVal.IsString()) {
        string queueIdStr = queueIdVal.GetString();
        if (RenderQueue::kQueueNameToQueueIdMap.find(queueIdStr) !=
            RenderQueue::kQueueNameToQueueIdMap.end()) {
            queueId = RenderQueue::kQueueNameToQueueIdMap[queueIdStr];
        }
    }
    
    const Value& stateVal = info[kRenderStateKey];
    RenderState* state = NULL;
    if (stateVal.IsObject()) {
        state = new RenderState;
        const Value& depthTestVal = stateVal[kDepthTestKey];
        if (depthTestVal.IsBool()) {
            state->setEnableDepthTest(depthTestVal.GetBool());
        }
        const Value& depthWriteVal = stateVal[kDepthWriteKey];
        if (depthWriteVal.IsBool()) {
            state->setEnableDepthWrite(depthWriteVal.GetBool());
        }
        const Value& blendVal = stateVal[kBlendKey];
        if (blendVal.IsBool()) {
            state->setEnableBlend(blendVal.GetBool());
        }
        const Value& cullFaceVal = stateVal[kCullFaceKey];
        if (cullFaceVal.IsBool()) {
            state->setEnableCullFace(cullFaceVal.GetBool());
        }
    }
    
    Entity* ent = mSceneMgr->createEntity(name, entityClassName, model, material, queueId);
    if (textures.size() > 0) {
        ent->setTextures(textures);
    }
    if (state) {
        ent->setRenderState(*state);
        delete state;
    }
    
    node->attachObject(ent);
}


bool SceneSerializer::handleInclude(const string &fileName,
                                    const string& nodeClassName,
                                    SceneNode *parent) {
    bool loadNewFile = false;

    // Recursively load sub scene file.
    SceneFileToSceneNodeMap::iterator iter = mSceneFileToSceneNodeMap.find(fileName);
    SceneNode* child = createChildSceneNodeWithClass(parent, nodeClassName, "");
    if (iter == mSceneFileToSceneNodeMap.end()) {
        mSceneFileToSceneNodeMap[fileName] = child;
        child->load(fileName);
        loadNewFile = true;
    } else {
        // We have already loaded the file. Check if it is actually
        // a scene file or camera definition file.
        if (SerializerManager::getInstance()->isSceneFile(fileName)) {
            // TODO(jhuang): fix the deep copy code.
            //child->deepCopyFrom(iter->second);
            child->load(fileName);
        }
    }

    return loadNewFile;
}


void SceneSerializer::handleCamera(const Value &cameraInfo, SceneNode* node) {
    string cameraName = "";
    const Value& cameraNameVal = cameraInfo[kNameKey];
    if (cameraNameVal.IsString()) {
        cameraName = cameraNameVal.GetString();
    }
    assert(!cameraName.empty() && "Error: camera name is not set in the scene file.");

    string cameraClassName = kDefaultCameraClass;
    const Value& cameraClass = cameraInfo[kObjClassKey];
    if (cameraClass.IsString()) {
        cameraClassName = cameraClass.GetString();
    }
    Camera* camera = mSceneMgr->createCamera(cameraName, cameraClassName);
    assert(camera != NULL);

    // We set the default camera only when it is actually being used in scene file.
    if (cameraName == Camera::kDefaultCameraName) {
        RenderWindow* target = mSceneMgr->getRenderer()->getRenderTarget();
        target->getDefaultViewport()->setDefaultCamera(camera);
    }

    SceneNode* cameraNode = createChildSceneNode(node, cameraInfo);
    if (!cameraName.empty()) {
        cameraNode->setName(cameraNode->getID(), cameraName);
    }
    cameraNode->setHidden(true);  // camera is not renderable.
    cameraNode->attachObject(camera);

    const Value& positionVal = cameraInfo[kPositionKey];
    assert(positionVal.IsArray() && positionVal.Size() == 3);
    camera->setPosition(Vector3(positionVal[(SizeType)0].GetDouble(),
                                positionVal[(SizeType)1].GetDouble(),
                                positionVal[(SizeType)2].GetDouble()));
    const Value& lookAtVal = cameraInfo[kLookAtKey];
    assert(lookAtVal.IsArray() && lookAtVal.Size() == 3);
    camera->lookAt(Vector3(lookAtVal[(SizeType)0].GetDouble(),
                           lookAtVal[(SizeType)1].GetDouble(),
                           lookAtVal[(SizeType)2].GetDouble()));

    const Value& fovVal = cameraInfo[kFieldOfView];
    if (fovVal.IsNumber()) {
        camera->setFieldOfView((float)fovVal.GetDouble());
    }
    EProjectionType projType = (EProjectionType)cameraInfo[kProjectionKey].GetInt();
    camera->setProjectionType(projType);

    handleNodeBehaviorsProperty(cameraInfo, cameraNode);
}


// Create a concrete scene node type based on the class info specified in |value|.
SceneNode* SceneSerializer::createChildSceneNode(SceneNode* parent, const Value& value){
    string nodeClassName = kDefaultSceneNodeClass;
    const Value& nodeClass = value[kNodeClassKey];
    if (nodeClass.IsString()) {
        nodeClassName = nodeClass.GetString();
    }

    string nodeNameSuffix = "";
    const Value& nameVal = value[kNameKey];
    if (nameVal.IsString()) {
        nodeNameSuffix = nameVal.GetString();
    }

    return mSceneMgr->createChildSceneNode(nodeClassName, parent, nodeNameSuffix);
}


SceneNode* SceneSerializer::createChildSceneNodeWithNameSuffix(SceneNode* parent, const string& nodeNameSuffix){
    return mSceneMgr->createChildSceneNode(kDefaultSceneNodeClass, parent, nodeNameSuffix);
}


SceneNode* SceneSerializer::createChildSceneNodeWithClass(SceneNode* parent, const string& nodeClassName, const string& nodeNameSuffix) {
    return mSceneMgr->createChildSceneNode(nodeClassName, parent, nodeNameSuffix);
}


#pragma handle json in SAX style

// TODO(jhuang):
void SceneSerializer::importSceneSAX(DataStream & stream, SceneNode* node) {
}

