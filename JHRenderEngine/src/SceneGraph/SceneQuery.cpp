//
//  SceneQuery.cpp
//
//  Created by Jingshu Huang on 9/4/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "SceneMananger.h"
#include "SceneNodeIterator.h"
#include "SceneQuery.h"


RaySceneQueryResult& RaySceneQuery::execute(void) {
    mResult.clear();

    // TODO(jhuang): refactor this so that callback style execute can re-use.
    SceneNodeIterator iter(SceneManager::getInstance()->getRootSceneNode());
    while(iter.hasNext()) {
        SceneNode& node = *iter;
        if (node.isVisible(mCurCamera)) {
            std::pair<bool, float> result = mRay.intersects(node.getWorldBoundAABB());
            if (result.first) { // intersect.
                if (node.numChildren() == 0 && node.numAttachedObjects() > 0) {
                    // A leaf node, meaning it has only a scene object attached to it.
                    // TODO(jhuang): here we only use the first scene obj attached to
                    // the node. Probably should put all the objects in the result.
                    RaySceneQueryResultEntry entry;
                    entry.distance = result.second;
                    entry.sceneObj = node.getAttachedObjects().begin()->second;
                    entry.shape = NULL;
                    mResult.push_back(entry);
                }

//                if (!observer->onQueryResult(a, result.second)) {
//                    return;
//                }

                iter.advance();
            } else {
                // Does not intersect. We skip all the children nodes.
                iter.advanceWithoutAddChildren();
            }
        } else {
            iter.advanceWithoutAddChildren();
        }
    }

    if (mNeedSortedResult) {
        if (mMaxNumResultEntries > 0 && mMaxNumResultEntries < mResult.size()) {
            // Partially sort the N smallest elements, discard others.
            std::partial_sort(mResult.begin(), mResult.begin() + mMaxNumResultEntries, mResult.end());
            mResult.resize(mMaxNumResultEntries);
        } else {
            // Sort entire result array
            std::sort(mResult.begin(), mResult.end());
        }
    }

    return mResult;
}


// Callback style execution.
void RaySceneQuery::execute(SceneQueryObserver *observer) {
    //TODO(jhuang):
}

//RaySceneQueryResult& RaySceneQuery::execute(void)
//{
//    // Clear without freeing the vector buffer
//    mResult.clear();
//    
//    // Call callback version with self as listener
//    this->execute(this);
//    
//    if (mSortByDistance)
//    {
//        if (mMaxResults != 0 && mMaxResults < mResult.size())
//        {
//            // Partially sort the N smallest elements, discard others
//            std::partial_sort(mResult.begin(), mResult.begin()+mMaxResults, mResult.end());
//            mResult.resize(mMaxResults);
//        }
//        else
//        {
//            // Sort entire result array
//            std::sort(mResult.begin(), mResult.end());
//        }
//    }
//    
//    return mResult;
//}
//
//void RaySceneQuery::clearResults(void)
//{
//    // C++ idiom to free vector buffer: swap with empty vector
//    RaySceneQueryResult().swap(mResult);
//}
////-----------------------------------------------------------------------
//bool RaySceneQuery::queryResult(MovableObject* obj, Real distance)
//{
//    // Add to internal list
//    RaySceneQueryResultEntry dets;
//    dets.distance = distance;
//    dets.movable = obj;
//    dets.worldFragment = NULL;
//    mResult.push_back(dets);
//    // Continue
//    return true;
//}
////-----------------------------------------------------------------------
//bool RaySceneQuery::queryResult(SceneQuery::WorldFragment* fragment, Real distance)
//{
//    // Add to internal list
//    RaySceneQueryResultEntry dets;
//    dets.distance = distance;
//    dets.movable = NULL;
//    dets.worldFragment = fragment;
//    mResult.push_back(dets);
//    // Continue
//    return true;
//}
