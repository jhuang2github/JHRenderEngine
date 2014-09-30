//
//  vectors.cpp
//
//  Created by Jingshu Huang on 1/15/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include <cfloat>

#include "vector3.h"
#include "quaternion.h"

template<> const Vector3 Vector3::ZERO(0, 0, 0);
template<> const Vector3 Vector3::UNIT_X(1, 0, 0);
template<> const Vector3 Vector3::UNIT_Y(0, 1, 0);
template<> const Vector3 Vector3::UNIT_Z(0, 0, 1);
template<> const Vector3 Vector3::UNIT_SCALE(1, 1, 1);
template<> const Vector3 Vector3::MAX_VEC(FLT_MAX, FLT_MAX, FLT_MAX);
template<> const Vector3 Vector3::MIN_VEC(-FLT_MAX, -FLT_MAX, -FLT_MAX);


/* Gets the shortest arc quaternion to rotate this vector to the
 destination vector.
 
 If you call this with a dest vector that is close to the inverse
 of this vector, we will rotate 180 degrees around the 'fallbackAxis'
 (if specified, or a generated axis if not) since in this case
 ANY axis of rotation is valid.
 */
template<> Quaternion Vector3::getRotationTo(const Vector3T& dest,
                         const Vector3T& fallbackAxis) const
{
    // Based on Stan Melax's article in Game Programming Gems
    Quaternion q;
    // Copy, since cannot modify local
    Vector3T v0 = *this;
    Vector3T v1 = dest;
    v0.normalise();
    v1.normalise();
    
    float d = v0.dotProduct(v1);
    // If dot == 1, vectors are the same
    if (d >= 1.f) {
        return Quaternion::IDENTITY;
    }
    if (d < (1e-6f - 1.f)) {
        if (fallbackAxis != Vector3T::ZERO) {
            // rotate 180 degrees about the fallback axis
            q.FromAngleAxis(M_PI, fallbackAxis);
        } else {
            // Generate an axis
            Vector3T axis = Vector3T::UNIT_X.crossProduct(*this);
            if (axis.isZeroLength()) { // pick another if colinear
                axis = Vector3T::UNIT_Y.crossProduct(*this);
            }
            axis.normalise();
            q.FromAngleAxis(M_PI, axis);
        }
    } else {
        float s = sqrtf((1.f + d) * 2.f);
        float invs = 1.f / s;
        Vector3T c = v0.crossProduct(v1);
        q.x = c.x * invs;
        q.y = c.y * invs;
        q.z = c.z * invs;
        q.w = s * 0.5f;
        q.normalise();
    }
    return q;
}
