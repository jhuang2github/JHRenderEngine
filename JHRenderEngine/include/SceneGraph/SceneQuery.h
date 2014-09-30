//
//  SceneQuery.h
//
//  Created by Jingshu Huang on 9/4/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __SceneQuery__
#define __SceneQuery__

#include "line.h"

class SceneObj;
class SceneManager;
class Shape;


// This optional class allows you to receive per-result callbacks from
// SceneQuery executions instead of a single set of consolidated results.
// This interface should be overridden with subclasses.
class SceneQueryObserver {
public:
    virtual ~SceneQueryObserver() {}
    /** Called when a SceneObject is returned by a query.
     The implementor should return 'true' to continue returning objects,
     or 'false' to abandon any further results from this query.
     */
    virtual bool onQueryResult(SceneObj* object) = 0;
    /** Called when a WorldFragment is returned by a query.
     The implementor should return 'true' to continue returning objects,
     or 'false' to abandon any further results from this query.
     */
    virtual bool onQueryResult(Shape* shape) = 0;
};


class SceneQuery {
public:
    SceneQuery(const SceneManager& sceneMgr) {}
    virtual ~SceneQuery() {}
    virtual void execute(SceneQueryObserver* observer) = 0;
};


#pragma ray query

class RaySceneQueryObserver : public SceneQueryObserver {
public:
    virtual ~RaySceneQueryObserver() {}
    virtual bool onQueryResult(SceneObj* obj, float distance) = 0;
    virtual bool onQueryResult(Shape* shape, float distance) = 0;
};


struct RaySceneQueryResultEntry {
public:
    float distance;
    SceneObj* sceneObj;
    Shape* shape;

public:
    RaySceneQueryResultEntry() { clear(); }
    ~RaySceneQueryResultEntry() { clear(); }

    void clear() {
        distance = 0.f;
        sceneObj = NULL;
        shape = NULL;
    }

    // Comparison operator for sorting.
    bool operator < (const RaySceneQueryResultEntry& rhs) const {
        assert(sceneObj != NULL && rhs.sceneObj != NULL);

        // First sort by render queue drawing order.
        // Then sort by distance from the camera.
        // Last sort by object ID. The bigger the ID, the later it shows up
        // in a scene description file, and the further it is from camera.
        ERenderQueueID thisQueue = sceneObj->getRenderQueueID();
        ERenderQueueID otherQueue = rhs.sceneObj->getRenderQueueID();
        if (thisQueue == otherQueue) {
            if (distance == rhs.distance) {
                return sceneObj->getID() < rhs.sceneObj->getID();
            }
            return distance < rhs.distance;
        }

        // The higher the queue ID, the closer to the camera.
        return thisQueue > otherQueue;
    }
};

typedef vector<RaySceneQueryResultEntry> RaySceneQueryResult;


class RaySceneQuery : public SceneQuery {
public:
    RaySceneQuery(const SceneManager& mgr, const Line& ray, Camera* cam) : SceneQuery(mgr) {
        mRay = ray;
        mCurCamera = cam;
        mMaxNumResultEntries = 1;
        mNeedSortedResult = true;
    }
    RaySceneQuery(const SceneManager& mgr) : SceneQuery(mgr) {
        mMaxNumResultEntries = 1;
        mNeedSortedResult = true;
    }
    ~RaySceneQuery() {}

    inline void setRay(const Line& ray) { mRay = ray; }
    inline const Line& getRay(void) const { return mRay; }
    inline void setMaxNumResultEntries(const int count) { mMaxNumResultEntries = count; }
    inline void setNeedSortedResult(const bool needSort) { mNeedSortedResult = needSort; }
    inline void setCamera(Camera* cam) { mCurCamera = cam; }

    RaySceneQueryResult& execute(void);
    void execute(SceneQueryObserver* observer); // Callback style execution.

protected:
    Line mRay;
    RaySceneQueryResult mResult;
    bool mNeedSortedResult;
    int mMaxNumResultEntries;

    // TODO(jhuang): may not belong here.
    // Only check the scene objects visible in the camera.
    Camera* mCurCamera;
};

#endif
