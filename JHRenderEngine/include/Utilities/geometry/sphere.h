//
//  sphere.h
//
//  Created by Jingshu Huang on 5/14/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __sphere__
#define __sphere__

#include "axisAlignedBox.h"
#include "mathUtility.h"
#include "shape.h"
#include "vector3.h"


// A sphere: x^2 + y^2 + z^2 = r^2 (for sphere's centered on the origin).
class Sphere : public Shape {
public:
    Sphere() : mRadius(0.f), mCenter(Vector3::ZERO) {}
    Sphere(const Vector3& center, float radius) : mRadius(radius), mCenter(center) {}
    
    float getRadius(void) const { return mRadius; }
    void setRadius(float radius) { mRadius = radius; }
    
    const Vector3& getCenter(void) const { return mCenter; }
    void setCenter(const Vector3& center) { mCenter = center; }
    // Sphere intersects a sphere.
    bool intersects(const Sphere& s) const {
        return (s.mCenter - mCenter).squaredLength() <= MathUtility::Sqr(s.mRadius + mRadius);
    }
    // Sphere intersects an AABB.
    bool intersects(const AxisAlignedBox& box) const {
        return MathUtility::intersects(*this, box);
    }
    // sphere intersects a plane.
    bool intersects(const Plane& plane) const {
        return MathUtility::intersects(*this, plane);
    }
    // sphere intersects a point. */
    bool intersects(const Vector3& v) const {
        return ((v - mCenter).squaredLength() <= MathUtility::Sqr(mRadius));
    }
    // Merges another Sphere into the current sphere.
    void merge(const Sphere& oth);

    static Sphere SPHERE_NULL;
    static Sphere SPHERE_UNIT;

private:
    float mRadius;
    Vector3 mCenter;

};

#endif
