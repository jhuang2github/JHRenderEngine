//
//  plane.cpp
//
//  Created by Jingshu Huang on 5/13/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "axisAlignedBox.h"
#include "plane.h"

Plane::Plane() {
    setDistance(0);
    setNormal(Vector3(0, 0, 1));
}

Plane::Plane(const Vector3& normal, const float distance) {
    setDistance(distance);
    setNormal(normal);
}

Plane::~Plane() {
}

float Plane::getDistance(const Vector3& point) const {
    return mNormal.dotProduct(point) + mDistance;
}

Plane::Side Plane::getSide(const Vector3& rkPoint) const {
    float fDistance = getDistance(rkPoint);
    if (fDistance < 0.f) {
        return Plane::NEGATIVE_SIDE;
    }
    if (fDistance > 0.f) {
        return Plane::POSITIVE_SIDE;
    }
    return Plane::NO_SIDE;
}

Plane::Side Plane::getSide(const AxisAlignedBox& box) const {
    if (box.isNull()) {
        return NO_SIDE;
    }
    if (box.isInfinite()) {
        return BOTH_SIDE;
    }
    return getSide(box.getCenter(), box.getHalfSize());
}

Plane::Side Plane::getSide (const Vector3& center, const Vector3& halfSize) const {
    float dist = getDistance(center);
    float maxAbsDist = mNormal.absDotProduct(halfSize);
    if (dist < -maxAbsDist) {
        return Plane::NEGATIVE_SIDE;
    }
    if (dist > +maxAbsDist) {
        return Plane::POSITIVE_SIDE;
    }
    return Plane::BOTH_SIDE;  // intersect.
}
