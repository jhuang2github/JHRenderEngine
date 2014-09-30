//
//  axisAlignedBox.cpp
//
//  Created by Jingshu Huang on 5/14/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "axisAlignedBox.h"
#include "Mesh.h"
#include "SubMesh.h"

const AxisAlignedBox AxisAlignedBox::BOX_INVALID = AxisAlignedBox(Vector3::MAX_VEC, Vector3::MIN_VEC);
const AxisAlignedBox AxisAlignedBox::BOX_NULL = AxisAlignedBox(Vector3::ZERO, Vector3::ZERO);
const AxisAlignedBox AxisAlignedBox::BOX_INFINITE = AxisAlignedBox(Vector3::MIN_VEC, Vector3::MAX_VEC);
const AxisAlignedBox AxisAlignedBox::BOX_ORIGIN_CENTER_UNIT = AxisAlignedBox(Vector3(-0.5f, -0.5, -0.5f), Vector3(0.5f, 0.5f, 0.5f));
const AxisAlignedBox AxisAlignedBox::BOX_ORIGIN_START_UNIT = AxisAlignedBox(Vector3::ZERO, Vector3::UNIT_SCALE);


const std::vector<Vector3>& AxisAlignedBox::getCorners(void) {
    updateCorners();
    return mCorners;
}

Vector3 AxisAlignedBox::getCenter(void) const {
    return Vector3((mMaximum.x + mMinimum.x) * 0.5f,
                   (mMaximum.y + mMinimum.y) * 0.5f,
                   (mMaximum.z + mMinimum.z) * 0.5f);
}

Vector3 AxisAlignedBox::getCorner(EBoxCorner cornerToGet) const {
    switch(cornerToGet) {
        case FAR_LEFT_BOTTOM:
            return mMinimum;
        case FAR_LEFT_TOP:
            return Vector3(mMinimum.x, mMaximum.y, mMinimum.z);
        case FAR_RIGHT_TOP:
            return Vector3(mMaximum.x, mMaximum.y, mMinimum.z);
        case FAR_RIGHT_BOTTOM:
            return Vector3(mMaximum.x, mMinimum.y, mMinimum.z);
        case NEAR_RIGHT_BOTTOM:
            return Vector3(mMaximum.x, mMinimum.y, mMaximum.z);
        case NEAR_LEFT_BOTTOM:
            return Vector3(mMinimum.x, mMinimum.y, mMaximum.z);
        case NEAR_LEFT_TOP:
            return Vector3(mMinimum.x, mMaximum.y, mMaximum.z);
        case NEAR_RIGHT_TOP:
            return mMaximum;
        default:
            return Vector3();
    }
}

void AxisAlignedBox::updateCorners() {
    if (mCorners.size() < 8) {
        mCorners.reserve(8);
    }
    
    mCorners[0] = mMinimum;
    mCorners[1].x = mMinimum.x; mCorners[1].y = mMaximum.y; mCorners[1].z = mMinimum.z;
    mCorners[2].x = mMaximum.x; mCorners[2].y = mMaximum.y; mCorners[2].z = mMinimum.z;
    mCorners[3].x = mMaximum.x; mCorners[3].y = mMinimum.y; mCorners[3].z = mMinimum.z;
    
    mCorners[4] = mMaximum;
    mCorners[5].x = mMinimum.x; mCorners[5].y = mMaximum.y; mCorners[5].z = mMaximum.z;
    mCorners[6].x = mMinimum.x; mCorners[6].y = mMinimum.y; mCorners[6].z = mMaximum.z;
    mCorners[7].x = mMaximum.x; mCorners[7].y = mMinimum.y; mCorners[7].z = mMaximum.z;
}

/// Calculate the volume of this box
float AxisAlignedBox::volume(void) const {
    Vector3 diff = mMaximum - mMinimum;
    return diff.x * diff.y * diff.z;
}

/** Scales the AABB by the vector given. */
void AxisAlignedBox::scale(const Vector3& s) {
    // assumes centered on origin
    Vector3 min = mMinimum * s;
    Vector3 max = mMaximum * s;
    setMinMax(min, max);
}

// Merges the passed in box into the current box. The result is the
// box which encompasses both.
void AxisAlignedBox::merge(const AxisAlignedBox& rhs) {
    mMinimum.makeFloor(rhs.mMinimum);
    mMaximum.makeCeil(rhs.mMaximum);
}

// Extends the box to encompass the specified point if needed.
void AxisAlignedBox::merge(const Vector3& point) {
    mMaximum.makeCeil(point);
    mMinimum.makeFloor(point);
}

void AxisAlignedBox::transformAffine(const Matrix4& m) {
    assert(m.isAffine());
    
    Vector3 centre = getCenter();
    Vector3 halfSize = getHalfSize();
    Vector3 newCentre = m.transformAffine(centre);
    Vector3 newHalfSize(
                        fabs(m[0][0]) * halfSize.x + fabs(m[0][1]) * halfSize.y + fabs(m[0][2]) * halfSize.z,
                        fabs(m[1][0]) * halfSize.x + fabs(m[1][1]) * halfSize.y + fabs(m[1][2]) * halfSize.z,
                        fabs(m[2][0]) * halfSize.x + fabs(m[2][1]) * halfSize.y + fabs(m[2][2]) * halfSize.z);
    
    setMinMax(newCentre - newHalfSize, newCentre + newHalfSize);
}

void AxisAlignedBox::transform(const Matrix4& matrix) {
    Vector3 oldMin, oldMax, currentCorner;
    
    // Getting the old values so that we can use the existing merge method.
    oldMin = mMinimum;
    oldMax = mMaximum;
    
    // We sequentially compute the corners in the following order :
    // 0, 6, 5, 1, 2, 4, 7, 3
    // This sequence allows us to only change one member at a time to get at
    // all corners.
    
    // For each one, we transform it using the matrix
    // Which gives the resulting point and merge the resulting point.
    
    // First corner
    // min min min
    currentCorner = oldMin;
    merge( matrix * currentCorner );
    
    // min,min,max
    currentCorner.z = oldMax.z;
    merge( matrix * currentCorner );
    
    // min max max
    currentCorner.y = oldMax.y;
    merge( matrix * currentCorner );
    
    // min max min
    currentCorner.z = oldMin.z;
    merge( matrix * currentCorner );
    
    // max max min
    currentCorner.x = oldMax.x;
    merge( matrix * currentCorner );
    
    // max max max
    currentCorner.z = oldMax.z;
    merge( matrix * currentCorner );
    
    // max min max
    currentCorner.y = oldMin.y;
    merge( matrix * currentCorner );
    
    // max min min
    currentCorner.z = oldMin.z;
    merge( matrix * currentCorner );
}

bool AxisAlignedBox::intersects(const AxisAlignedBox& b2) const {
    // Use up to 6 separating planes
    if (mMaximum.x < b2.mMinimum.x)
        return false;
    if (mMaximum.y < b2.mMinimum.y)
        return false;
    if (mMaximum.z < b2.mMinimum.z)
        return false;
    
    if (mMinimum.x > b2.mMaximum.x)
        return false;
    if (mMinimum.y > b2.mMaximum.y)
        return false;
    if (mMinimum.z > b2.mMaximum.z)
        return false;
    
    // otherwise, must be intersecting
    return true;
}

AxisAlignedBox AxisAlignedBox::intersection(const AxisAlignedBox& b2) const {
    Vector3 intMin = mMinimum;
    Vector3 intMax = mMaximum;
    
    intMin.makeCeil(b2.getMinimum());
    intMax.makeFloor(b2.getMaximum());
    
    // Check intersection isn't null
    if (intMin.x < intMax.x &&
        intMin.y < intMax.y &&
        intMin.z < intMax.z) {
        return AxisAlignedBox(intMin, intMax);
    }
    
    return AxisAlignedBox();
}

bool AxisAlignedBox::contains(const AxisAlignedBox& other) const {
    return this->mMinimum.x <= other.mMinimum.x &&
    this->mMinimum.y <= other.mMinimum.y &&
    this->mMinimum.z <= other.mMinimum.z &&
    other.mMaximum.x <= this->mMaximum.x &&
    other.mMaximum.y <= this->mMaximum.y &&
    other.mMaximum.z <= this->mMaximum.z;
}

/** Tests whether the vector point is within this box. */
bool AxisAlignedBox::contains(const Vector3& v) const {
    return(v.x >= mMinimum.x  &&  v.x <= mMaximum.x  &&
           v.y >= mMinimum.y  &&  v.y <= mMaximum.y  &&
           v.z >= mMinimum.z  &&  v.z <= mMaximum.z);
}

float AxisAlignedBox::minDistance(const Vector3& v) const {
    if (this->contains(v)) {
        return 0;
    } else {
        float maxDist = std::numeric_limits<float>::min();
        
        if (v.x < mMinimum.x)
            maxDist = std::max(maxDist, mMinimum.x - v.x);
        if (v.y < mMinimum.y)
            maxDist = std::max(maxDist, mMinimum.y - v.y);
        if (v.z < mMinimum.z)
            maxDist = std::max(maxDist, mMinimum.z - v.z);
        
        if (v.x > mMaximum.x)
            maxDist = std::max(maxDist, v.x - mMaximum.x);
        if (v.y > mMaximum.y)
            maxDist = std::max(maxDist, v.y - mMaximum.y);
        if (v.z > mMaximum.z)
            maxDist = std::max(maxDist, v.z - mMaximum.z);
        
        return maxDist;
    }
}

void AxisAlignedBox::loadIntoMesh(Mesh &mesh) {
    static const int numKeyFrames = 1;
    static const int numVerts = 8;

    // Mesh vertices.
    mesh.setNumKeyFrames(numKeyFrames);
    mesh.setVertexFormat(VT_Pos);
    mesh.setNumVertices(numVerts);
    int frameVertSizeInByte = mesh.getFrameVerticesSizeInByte();
    mesh.ensureVertexCapacity(frameVertSizeInByte * numKeyFrames);

    updateMesh(mesh);

    // Mesh indices (sub-meshes).
    static const int numIndices = 3 * 2 * 6;
    static const ushort indices[] = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        2, 4, 7, 2, 7, 3,
        0, 5, 1, 0, 6, 5,
        2, 1, 5, 2, 5, 4,
        0, 3, 6, 3, 7, 6
    };

    SubMesh* subMesh = mesh.createSubMesh();
    subMesh->setIndexFormat(GL_UNSIGNED_SHORT);
    subMesh->setNumIndices(numIndices);
    int indexSizeInByte = subMesh->getIndicesSizeInByte();
    subMesh->ensureIndexCapacity(indexSizeInByte);
    uint* q = static_cast<uint*>(subMesh->getIndices());
    for (int k = 0; k < numIndices; ++k) {
        *(q++) = indices[k];
    }
    subMesh->onLoadComplete();  // set up index vbo
}

void AxisAlignedBox::updateMesh(Mesh &mesh) {
    updateCorners();

    static const int numVerts = 8;
    float* p = static_cast<float*>(mesh.getVertices());
    for (int k = 0; k < numVerts; ++k) {
        const Vector3f& v = mCorners[k];
        *(p++) = v.x;
        *(p++) = v.y;
        *(p++) = v.z;
    }
    mesh.setObjectBound(mMinimum, mMaximum);
    mesh.onLoadComplete();  // set up vertex vbo
}

