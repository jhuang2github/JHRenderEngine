//
//  Entity.h
//
//  Created by Jingshu Huang on 1/18/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Entity_h
#define _Entity_h

#include "Material.h"
#include "platformDef.h"
#include "SceneObj.h"

class Entity;
class Mesh;
class SceneManager;
class SubEntity;


#define kDefaultEntityClass "Entity"


// An entity is an instance of a mesh. It can be attached to a scene node.
class Entity : public SceneObj {
public:
    Entity(const string& name, const ESceneObjType& objType, Mesh* _mesh,
           SceneManager* sceneMgr, const ERenderQueueID& queueID);
    ~Entity();


    // Override factory method.
    virtual SceneObj* create(SceneManager* sceneMgr,
                             const string& objName,
                             const KeyValuePairStringMap* params = 0);
    virtual void deepCopyFrom(SceneObj* other);

    void setMaterial(const string& name);
    void setMaterial(Material* material);
    void setTextures(const vector<string>& textures);
    void setRenderState(const RenderState& state);

    inline Mesh* getMesh() const { return mMesh; }
    int getNumVertices() const;

    // Override.
    virtual const Bound& getObjectBound(void) const;

    // Override.
    virtual void updateRenderQueue(RenderQueue* queue);

    static string kRenderQueueIDKey;
    static string kMeshKey;

protected:
    Entity(const string& name, const ESceneObjType& type);
    void parseParams(const KeyValuePairStringMap* params,
                     Mesh** mesh,
                     ERenderQueueID& queueID);

private:
    static bool sFactoryInitialized;
    static bool registerFactory();

    Entity() { assert(false && "Entity::Entity default constructor is called."); }
    void buildSubEntities();

    typedef vector<SubEntity*> SubEntityList;

    Mesh* mMesh;
    SubEntityList mSubEntities;  // Owns the data.

    // TODO(jhuang): may need to define an AnimationState set to encapsulate
    // animation state info from motionbillboard primitive here.
};

#endif
