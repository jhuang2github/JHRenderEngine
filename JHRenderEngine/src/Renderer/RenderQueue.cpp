//
//  RenderQueue.cpp
//
//  Created by Jingshu Huang on 2/22/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "Material.h"
#include "MaterialManager.h"
#include "Renderable.h"
#include "RenderQueue.h"
#include "RenderQueueVisitor.h"
#include "SceneObj.h"
#include "ShadingTechnique.h"
#include "stringUtility.h"


QueueNameToQueueIdMap RenderQueue::kQueueNameToQueueIdMap =
        RenderQueue::createQueueNameToQueueIdMap();


QueueNameToQueueIdMap RenderQueue::createQueueNameToQueueIdMap() {
    QueueNameToQueueIdMap map;
    map["QUEUE_0"] =
    map["QUEUE_MIN"] = QUEUE_MIN;
    map["QUEUE_1"] =
    map["QUEUE_1_BACKGROUND"] = QUEUE_1_BACKGROUND;
    map["QUEUE_2"] = QUEUE_2;
    map["QUEUE_3"] = QUEUE_3;
    map["QUEUE_4"] = QUEUE_4;
    map["QUEUE_5"] =
    map["QUEUE_5_MAIN"] = QUEUE_5_MAIN;
    map["QUEUE_6"] = QUEUE_6;
    map["QUEUE_7"] = QUEUE_7;
    map["QUEUE_8"] = QUEUE_8;
    map["QUEUE_9"] = QUEUE_9;
    map["QUEUE_10"] =
    map["QUEUE_10_OVERLAY"] = QUEUE_10_OVERLAY;
    map["QUEUE_MAX"] = QUEUE_MAX;
    return map;
}


RenderQueue::RenderQueue() {
    mQueueID = QUEUE_5_MAIN;
}


RenderQueue::RenderQueue(Renderable* r) {
    mQueueID = QUEUE_5_MAIN;
    addRenderable(r);
}


RenderQueue::~RenderQueue(){
    for (PassToRenderableListMap::iterator iter = mPassToRenderables.begin();
         iter != mPassToRenderables.end(); ++iter) {
        RenderableList* renderableList = iter->second;
        if (renderableList) {
            renderableList->clear();  // Does not own the renderables.
            delete renderableList;
        }
    }
    mPassToRenderables.clear();
}


void RenderQueue::addRenderable(Renderable* r) {
    Material* material = r->getMaterial();
    if (!material) {
        material = MaterialManager::getInstance()->getDefaultMaterial();
    }
    assert(material != NULL);
    ShadingTechnique* pTech = material->getBestTechnique();
    assert(pTech != NULL);
    addRenderableByPass(r, pTech);
}


void RenderQueue::addRenderableByPass(Renderable* r, ShadingTechnique* pTech) {
    RenderPassList& passes = pTech->getPasses();
    for (RenderPassList::const_iterator iter = passes.begin(); iter != passes.end(); ++iter) {
        Pass* pass = *iter;
        PassToRenderableListMap::iterator passIter = mPassToRenderables.find(pass);
        if (passIter == mPassToRenderables.end()) {
            std::pair<PassToRenderableListMap::iterator, bool> retPair =
                mPassToRenderables.insert(
                PassToRenderableListMap::value_type(pass, new RenderableList));
            passIter = retPair.first;
        }

        assert(passIter->second != NULL);
        passIter->second->push_back(r);
    }
}


void RenderQueue::addVisibleObject(SceneObj* obj) {
    if (obj->isVisible()) {
        obj->updateRenderQueue(this);
    }
}

void RenderQueue::accept(RenderQueueVisitor& visitor) {
    for (PassToRenderableListMap::const_iterator iter = mPassToRenderables.begin();
         iter != mPassToRenderables.end(); ++iter) {
        RenderableList* renderables = iter->second;
        if (!renderables) {
            continue;
        }

        Pass* pass = iter->first;
        assert(pass != NULL);
        visitor.preVisit(pass);
        for (RenderableList::const_iterator rIter = renderables->begin();
             rIter != renderables->end(); ++rIter) {
            visitor.visit(*rIter);
        }
        visitor.postVisit(pass);
    }
}

