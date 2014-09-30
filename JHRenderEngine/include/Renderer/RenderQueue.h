//
//  RenderQueue.h
//
//  Created by Jingshu Huang on 2/22/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _RenderQueue_h
#define _RenderQueue_h

#include "Pass.h"
#include "platformDef.h"


/*
1)Background - this render queue is rendered before any others. It is used for
 skyboxes and the like.
2) Geometry (main, default) - this is used for most objects. Opaque geometry
 uses this queue.
3) AlphaTest - alpha tested geometry uses this queue. It's a separate queue from
 Geometry one since it's more efficient to render alpha-tested objects after all
 solid ones are drawn.
4) Transparent - this render queue is rendered after Geometry and AlphaTest, in
 back-to-front order. Anything alpha-blended (i.e. shaders that don't write to
 depth buffer) should go here (glass, particle effects).
5) Overlay - this render queue is meant for overlay effects. Anything rendered
 last should go here (e.g. lens flares).
*/
typedef enum {
    QUEUE_MIN = 0,
    QUEUE_1_BACKGROUND = 10,
    QUEUE_2 = 20,
    QUEUE_3 = 30,
    QUEUE_4 = 40,
    QUEUE_5_MAIN = 50,  // The default render queue
    QUEUE_6 = 60,
    QUEUE_7 = 70,       // alpha test
    QUEUE_8 = 80,       // transparent
    QUEUE_9 = 90,
    QUEUE_10_OVERLAY = 100,
    QUEUE_MAX = 110
} ERenderQueueID;

typedef unordered_map<string, ERenderQueueID> QueueNameToQueueIdMap;

class Renderable;
class RenderQueueVisitor;
class SceneObj;


class RenderQueue {

    // Comparator to order pass group. For ordering, see Pass::setHash().
    struct PassGroupLess {
        bool operator()(const Pass* a, const Pass* b) const {
            // Sort by pass hash.
            uint hasha = a->getHash();
            uint hashb = b->getHash();
            return hasha <= hashb;
        }
    };

    typedef vector<Renderable*> RenderableList;
    typedef vector<Pass*> RenderablePassList;

public:
    RenderQueue();
    RenderQueue(Renderable* r);
    ~RenderQueue();

    void addRenderable(Renderable* r);
    void addVisibleObject(SceneObj* obj);
    void accept(RenderQueueVisitor& visitor);

    static QueueNameToQueueIdMap kQueueNameToQueueIdMap;

private:
    void addRenderableByPass(Renderable* rend, ShadingTechnique* pTech);

private:
    ERenderQueueID mQueueID;

    // Scene objects are grouped by material in render queues to minimize gl
    // state changes.
    typedef map<Pass*, RenderableList*, PassGroupLess> PassToRenderableListMap;
    PassToRenderableListMap mPassToRenderables;  // Do not own Passes or Renderables.


    static QueueNameToQueueIdMap createQueueNameToQueueIdMap();
};

#endif
