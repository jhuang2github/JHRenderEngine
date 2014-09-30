//
//  bound.h
//
//  Created by Jingshu Huang on 8/21/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __bound_h_
#define __bound_h_


#include "axisAlignedBox.h"
#include "mathUtility.h"
#include "shape.h"
#include "sphere.h"

class Mesh;


// A container class that holds different types of bounds.
class Bound : public Shape {
public:
    inline void reset() {
        // mAABB = AxisAlignedBox::BOX_INVALID;
        mAABB = AxisAlignedBox(Vector3::MAX_VEC, Vector3::MIN_VEC);
        mSphere = Sphere::SPHERE_NULL;
    }

    inline Sphere& getBoundSphere(void) {
        setSphereFromAABB();
        return mSphere;
    }

    inline AxisAlignedBox& getAABB(void) { return mAABB; }
    inline void setObjectBound(const AxisAlignedBox& box) {  setAABB(box); }
    inline void setAABB(const AxisAlignedBox& box) { mAABB = box; }

    void merge(const Bound& other) {
        merge(other.mAABB);
    }

    void merge(const AxisAlignedBox& other) {
        mAABB.merge(other);
    }

    // Override.
    void loadIntoMesh(Mesh& mesh) {
        mAABB.loadIntoMesh(mesh);
        //mSphere.loadIntoMesh(mesh);
    }

    // Override.
    void updateMesh(Mesh& mesh) {
        mAABB.updateMesh(mesh);
        //mSphere.updateMesh(mesh);
    }

private:
    void setSphereFromAABB() {
        mSphere.setRadius((mAABB.getMaximum() - mAABB.getCenter()).length());
        mSphere.setCenter(mAABB.getCenter());
    }

    AxisAlignedBox mAABB;
    // Centered at the center of bounding box.
    // TODO(jhuang): check if this is correct.
    // Or shall we center at the world origin?
    Sphere mSphere;
};

#endif
