//
//  SceneSerializer.h
//
//  Created by Jingshu Huang on 8/5/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __SceneSerializer__
#define __SceneSerializer__

// For DOM-style json parsing
#include "rapidjson/include/document.h"
// For SAX-stuyle json parsing
#include "rapidjson/include/rapidjson.h"
#include "rapidjson/include/reader.h"
#include "Serializer.h"

using namespace rapidjson;


class Camera;
class DataStream;
class SceneManager;
class SceneNode;
class SceneObj;
class Viewport;


class SceneSerializer : public Serializer {
public:
    SceneSerializer() {
        mVersion = "v1_0";
    }
    virtual ~SceneSerializer() {
    }

    void importScene(DataStream&, SceneNode*);
    bool handleInclude (const string& fileName,
                        const string& nodeClassName,
                        SceneNode* parent);

private:
    void importSceneDOM(DataStream&, SceneNode*);
    void importSceneSAX(DataStream&, SceneNode*);

    void handleCamera(const Value& cameraInfo, SceneNode*);
    void handleEntity(const Value& entityInfo, SceneNode* node);
    void handleEntityProperties(const Value& info, SceneNode* node);

    void handleNode(const Value& entities, SceneNode* node);
    void handleNodeProperties(const Value& info, SceneNode* node);
    void handleNodeTransformProperty(const Value& info, SceneNode* node);
    void handleNodeBehaviorsProperty(const Value& info, SceneNode* node);

    SceneNode* createChildSceneNode(SceneNode* parent, const Value& value);
    SceneNode* createChildSceneNodeWithNameSuffix(SceneNode* parent, const string& nodeNameSuffix);
    SceneNode* createChildSceneNodeWithClass(SceneNode* parent,
        const string& nodeClassName, const string& nodeNameSuffix);

    Viewport* getDefaultViewport();

    typedef unordered_map<string, SceneNode*> SceneFileToSceneNodeMap;
    SceneFileToSceneNodeMap mSceneFileToSceneNodeMap;

    SceneManager* mSceneMgr;
};


#endif
