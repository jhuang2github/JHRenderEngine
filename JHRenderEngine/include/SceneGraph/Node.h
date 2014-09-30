//
//  Node.h
//
//  Created by Jingshu Huang on 2/19/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Node_h
#define _Node_h

#include "Behavior.h"
#include "matrix4.h"
#include "Observer.h"
#include "quaternion.h"
#include "platformDef.h"
#include "vector2.h"
#include "vector3.h"


typedef string NodeType;
typedef unsigned long long int NodeID;

// An abstract class which scene node and bone inherit.
class Node : public Observer {
public:
    typedef map<NodeID, Node*> NodeMap;  // sorted by NodeID.

    Node(const NodeType& type, const string& nameSuffix);
    virtual ~Node();
    void init(const NodeID& nID, const NodeType& type, const string& nameSuffix);

    virtual void reset();
    virtual void deepCopyFrom(Node* other);
    virtual void nodeChildrenCopyImpl(Node* other) = 0;


    static string stringfyNodeID(const NodeID& nID);
    inline NodeID& getID() { return mNodeID; }
    inline void setID(const NodeID& nid) { mNodeID = nid; }
    
    inline void setNodeType(const NodeType& type) { mNodeType = type; }
    inline NodeType getNodeType(void) const { return mNodeType; }

    inline string& getNameSuffix() { return mNameSuffix; }
    inline string& getName() { return mName; }
    inline void setName(const NodeID& nID, const string& suffix) {
        mName = createName(nID, suffix);
    }
    inline string createName(const NodeID& nID, const string& suffix) {
        return stringfyNodeID(nID) + "_" + suffix;
    }

    inline void setParent(Node* parent);
    inline Node* getParent(void) { return mParent; }

    inline size_t numChildren() const { return mChildren.size(); }
    inline NodeMap& getChildren() { return mChildren; }
    virtual Node* createChild() {
        return createChild(Vector3::ZERO, Quaternion::IDENTITY, Vector3::UNIT_SCALE);
    }
    virtual Node* createChild(const Vector3& translate,
                              const Quaternion& rotate,
                              const Vector3& scale);
    virtual Node* createChildImpl() {
        assert(false && "Node::createChildImpl(): Child class should implment this method.");
        return NULL;  // Make compiler happy.
    }
    virtual void addChild(Node* child);
    virtual void removeChild(Node* child);
    virtual void removeAllChildren();

    // Transformations relative to the parent node.
    void translate(const Vector3&);
    void rotate(const Quaternion&);
    void scale(const Vector3&);

    // Transformations derived from all parents, in world space.
    inline const Quaternion& getDerivedRotate(void) const {
        return mTxf.getDerivedRotate(mParent);
    }
    inline const Vector3& getDerivedTranslate(void) const {
        return mTxf.getDerivedTranslate(mParent);
    }
    inline const Vector3& getDerivedScale(void) const {
        return mTxf.getDerivedScale(mParent);
    }
    // Gets the full transformation matrix for this node. This method returns
    // the full transformation matrix for this node, including the effect of any
    // parent node transformations. This should only be called by a SceneManager
    // which knows the derived transforms have been updated before calling this
    // method. Game app should just use the relative transforms.
    inline const Matrix4& getFullWorldTransform() const {
        return mTxf.getFullWorldTransform(mParent);
    }

//    virtual void accept(NodeVisitor& visitor) { visitor.visit(*this); }
    virtual void needUpdate(bool forceParentUpdate=false);
    virtual void update(const float secondsPassed);

    // Behavior related.
    virtual void attachBehavior(Behavior* behavior);
    virtual void detachBehavior(Behavior* behavior);
    virtual void detachBehavior(const BehaviorType& type);
    virtual Behavior* getBehavior(const BehaviorType& type);
    virtual bool preHandleTouch(void);
    virtual bool handleTouch(const BehaviorType& type, const Vector2& point);

    // Override.
    virtual void updateOnNotification(Observable* observable, Notification* msg=NULL) = 0;

protected:
    virtual void updateFromParent(void) const {
        mTxf.updateFromParent(mParent);
    }

    NodeID    mNodeID;
    NodeType  mNodeType;
    string    mNameSuffix;  // User-specified name in a scene file.
    string    mName;        // Name is |mNodeID| + |mNameSuffix|

    Node*  mParent;
    // Map from a child node Id to the child node. Does not own the data.
    // Nodes are sorted based on their Ids. Their Ids are generated automatically
    // when we parse scene description files.
    NodeMap mChildren;

    class Transform {
    private:
        Transform();
        ~Transform();
        void reset();
        void deepCopyFrom(const Transform& other);
        
        const Matrix4& getFullWorldTransform(const Node* parent) const;
        const Quaternion& getDerivedRotate(const Node* parent) const;
        const Vector3& getDerivedTranslate(const Node* parent) const;
        const Vector3& getDerivedScale(const Node* parent) const;
        
        void updateFromParent(const Node* parent) const;
        void needUpdate(const Node* parent, bool forceParentUpdate=false);
        void update(const Node* parent, const float secondsPassed);
        
    private:
        // Local relative transformations.
        Vector3 mTranslate;
        Vector3 mScale;
        Quaternion mRotate;
        
        // If the node inherit transformations from parents.
        bool mInheritRotate;
        bool mInheritScale;
        
        // Cached derived transformations with parents info.
        mutable bool mCachedTransformOutOfDate;
        mutable Matrix4 mCachedTransform;
        mutable Vector3 mDerivedTranslate;
        mutable Vector3 mDerivedScale;
        mutable Quaternion mDerivedRotate;
        
        mutable bool mNeedParentUpdate;
        
        friend class Node;
    };
    Transform mTxf;

    // Attached behaviors of the node. Does not own data. Data are owned by
    // behavior manager.
    // Map from behavior type to behavior instance.
    typedef unordered_map<BehaviorType, Behavior*> BehaviorMap;
    BehaviorMap mBehaviors;

private:
    static NodeID nextNodeNamePrefix;

    // Do not allow default constructor.
    Node() { assert(false); }

    friend class NodeIterator;
};

//class NodeVisitor {
//public:
//    virtual ~NodeVisitor() {}
//    virtual void visit(SceneNode& node) = 0;
//};

#endif
