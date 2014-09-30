//
//  sphere.cpp
//
//  Created by Jingshu Huang on 5/14/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "sphere.h"


Sphere Sphere::SPHERE_NULL = Sphere(Vector3::ZERO, 0.f);
Sphere SPHERE_UNIT = Sphere(Vector3::ZERO, 1.f);


void Sphere::merge(const Sphere& oth) {
    Vector3 diff =  oth.getCenter() - mCenter;
    float lengthSq = diff.squaredLength();
    float radiusDiff = oth.getRadius() - mRadius;
    
    // Early-out
    if (MathUtility::Sqr(radiusDiff) >= lengthSq) {
        if (radiusDiff > 0.0f) { // One fully contains the other
            mCenter = oth.getCenter();
            mRadius = oth.getRadius();
        }
        return;
    }
    
    float length = MathUtility::Sqrt(lengthSq);

    Vector3 newCenter;
    float newRadius;
    if ((length + oth.getRadius()) > mRadius) {
        float t = (length + radiusDiff) / (2.0f * length);
        newCenter = mCenter + diff * t;
    }
    // otherwise, we keep our existing center
    newRadius = 0.5f * (length + mRadius + oth.getRadius());

    mCenter = newCenter;
    mRadius = newRadius;
}
