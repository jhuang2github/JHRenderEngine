//
//  SceneObjFactory.h
//
//  Created by Jingshu Huang on 1/18/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _SceneObjFactory_h
#define _SceneObjFactory_h

#include "platformDef.h"
#include "Singleton.h"

class SceneObj;
class SceneManager;


// An interface for scene object factories.
class SceneObjFactory {
public:
    virtual SceneObj* create(SceneManager* sceneMgr,
                             const string& objName,
                             const KeyValuePairStringMap* params = 0) = 0;
};


template<typename T>
class SceneObjFactoryImplT : public SceneObjFactory,
                             public Singleton<SceneObjFactoryImplT<T> > {
public:
    SceneObj* create(SceneManager* sceneMgr,
                     const string& objName,
                     const KeyValuePairStringMap* params = 0);
};

#endif
