//
//  line.h
//
//  Created by Jingshu Huang on 5/14/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __line__
#define __line__

#include "axisAlignedBox.h"
#include "MathUtility.h"
#include "plane.h"
#include "platformDef.h"
#include "shape.h"
#include "vector3.h"


// Actually this is a ray.
class Line : public Shape {
private:
    Vector3 mOrigin;
    Vector3 mDirection;
    float length;

public:
    Line() : mOrigin(Vector3::ZERO), mDirection(Vector3::UNIT_Z) { }
    Line(const Vector3& origin, const Vector3& direction) : mOrigin(origin), mDirection(direction) {}
    
    void setOrigin(const Vector3& origin) { mOrigin = origin; }
    const Vector3& getOrigin(void) const {return mOrigin;}
    
    void setDirection(const Vector3& dir) {mDirection = dir;}
    const Vector3& getDirection(void) const {return mDirection;}
    
    // Gets the position of a point t units along the Line.
    Vector3 getPoint(float t) const {
        return Vector3(mOrigin + (mDirection * t));
    }
    Vector3 operator*(float t) const {
        return getPoint(t);
    }
    
    /** Tests whether this Line intersects the given plane.
     @return A pair structure where the first element indicates whether
     an intersection occurs, and if true, the second element will
     indicate the distance along the Line at which it intersects.
     This can be converted to a point in space by calling getPoint().
     */
    std::pair<bool, float> intersects(const Plane& p) const {
        return MathUtility::intersects(*this, p);
    }
    /** Tests whether this Line intersects the given sphere.
     @return A pair structure where the first element indicates whether
     an intersection occurs, and if true, the second element will
     indicate the distance along the Line at which it intersects.
     This can be converted to a point in space by calling getPoint().
     */
    std::pair<bool, float> intersects(const Sphere& s) const {
        return MathUtility::intersects(*this, s);
    }
    /** Tests whether this Line intersects the given box.
     @return A pair structure where the first element indicates whether
     an intersection occurs, and if true, the second element will
     indicate the distance along the Line at which it intersects.
     This can be converted to a point in space by calling getPoint().
     */
    std::pair<bool, float> intersects(const AxisAlignedBox& box) const {
        return MathUtility::intersects(*this, box);
    }
};

#endif
