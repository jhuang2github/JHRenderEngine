//
//  Mesh.cpp
//
//  Created by Jingshu Huang on 1/14/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "axisAlignedBox.h"
#include "bound.h"
#include "DataStream.h"
#include "Mesh.h"
#include "MeshSerializer.h"
#include "platformUtil.h"
#include "platformPathUtil.h"
#include "Serializer.h"
#include "SerializerManager.h"
#include "shape.h"
#include "SubMesh.h"
#include "VertexBufferObj.h"


string Mesh::kDefaultMeshName = "quad.pbin";

// Look-up key for each property.
int Mesh::kPosIndex = 0;
int Mesh::kTexCoordIndex = 1;
int Mesh::kColorIndex = 2;
int Mesh::kBlendIndicesIndex = 3;
int Mesh::kBlendWeightsIndex = 4;

VertexFormatSizeMap Mesh::kVertexFormatSize = Mesh::createVertexFormatSizeMap();
VertexFormatToAttrOffsetsMap Mesh::kVertexFormatToAttrOffsets = Mesh::createVertexFormatToAttrOffsetsMap();


// Class methods.
VertexFormatSizeMap Mesh::createVertexFormatSizeMap() {
    VertexFormatSizeMap sizeMap;
    sizeMap[VT_Pos] = sizeof(VertexPos);
    sizeMap[VT_PosUV] = sizeof(VertexPosUV);
    sizeMap[VT_PosColor] = sizeof(VertexPosColor);
    sizeMap[VT_PosUVColor] = sizeof(VertexPosUVColor);
    sizeMap[VT_Skinned] = sizeof(VertexSkinned);
    return sizeMap;
}


#define BUFFER_OFFSET(m) ((byte*)NULL + (m))

VertexFormatToAttrOffsetsMap Mesh::createVertexFormatToAttrOffsetsMap() {
    VertexFormatToAttrOffsetsMap formatToOffsetMap;
    byte* start;

    VertexPos vtPos;
    start = (byte*)&vtPos;
    AttrToOffsetMap mapPos;
    mapPos[kPosIndex] = BUFFER_OFFSET((byte*)&(vtPos.position) - start);
    formatToOffsetMap[VT_Pos] = mapPos;

    VertexPosUV vtPosUV;
    start = (byte*)&vtPosUV;
    AttrToOffsetMap mapPosUV;
    mapPosUV[kPosIndex] = BUFFER_OFFSET((byte*)&(vtPosUV.position) - start);
    mapPosUV[kTexCoordIndex] = BUFFER_OFFSET((byte*)&(vtPosUV.texcoord) - start);
    formatToOffsetMap[VT_PosUV] = mapPosUV;

    VertexPosColor vtPosColor;
    start = (byte*)&vtPosColor;
    AttrToOffsetMap mapPosColor;
    mapPosColor[kPosIndex] = BUFFER_OFFSET((byte*)&(vtPosColor.position) - start);
    mapPosColor[kColorIndex] = BUFFER_OFFSET((byte*)&(vtPosColor.color) - start);
    formatToOffsetMap[VT_PosColor] = mapPosColor;

    VertexPosUVColor vtPosUVColor;
    start = (byte*)&vtPosUVColor;
    AttrToOffsetMap mapPosUVColor;
    mapPosUVColor[kPosIndex] = BUFFER_OFFSET((byte*)&(vtPosUVColor.position) - start);
    mapPosUVColor[kTexCoordIndex] = BUFFER_OFFSET((byte*)&(vtPosUVColor.texcoord) - start);
    mapPosUVColor[kColorIndex] = BUFFER_OFFSET((byte*)&(vtPosUVColor.color) - start);
    formatToOffsetMap[VT_PosUVColor] = mapPosUVColor;

    VertexSkinned vtSkinned;
    start = (byte*)&vtSkinned;
    AttrToOffsetMap mapSkinned;
    mapSkinned[kPosIndex] = BUFFER_OFFSET((byte*)&(vtSkinned.position) - start);
    mapSkinned[kTexCoordIndex] = BUFFER_OFFSET((byte*)&(vtSkinned.texcoord) - start);
    mapSkinned[kColorIndex] = BUFFER_OFFSET((byte*)&(vtSkinned.color) - start);
    mapSkinned[kBlendIndicesIndex] = BUFFER_OFFSET((byte*)&(vtSkinned.blendindices) - start);
    mapSkinned[kBlendWeightsIndex] = BUFFER_OFFSET((byte*)&(vtSkinned.blendweights) - start);
    formatToOffsetMap[VT_Skinned] = mapSkinned;

    return formatToOffsetMap;
}


Mesh::Mesh() {
    mPath = "";
    mVertices = NULL;
    mNumVertices = 0;
    mVbo = NULL;
}


Mesh::Mesh(const string& fileName) {
    mPath = PlatformPathUtil::assetPath + "/"  + PlatformPathUtil::modelSubPath + "/" + fileName;
    mVertices = NULL;
    mNumVertices = 0;
    mVbo = NULL;
}


Mesh::~Mesh() {
    if (mVertices) {
        delete reinterpret_cast<byte*>(mVertices);
        mVertices = NULL;
    }
    mNumVertices = 0;

    for (vector<SubMesh*>::iterator iter = mSubMeshes.begin();
         iter != mSubMeshes.end(); ++iter) {
        delete *iter;
    }
    mSubMeshes.clear();

    if (mVbo) {
        delete mVbo;
    }
    mVbo = NULL;
}


byte* Mesh::getOffset(const EVertexFormat format, const int AttrIndex) {
    return kVertexFormatToAttrOffsets[format][AttrIndex];
}


int Mesh::getStride(const EVertexFormat format) {
    return kVertexFormatSize[format];
}


void Mesh::setObjectBound(const Vector3& min, const Vector3& max) {
    mObjBound.setObjectBound(AxisAlignedBox(min, max));
}

void Mesh::setObjectBound(const AxisAlignedBox& box) {
    mObjBound.setObjectBound(box);
}


void Mesh::ensureVertexCapacity(size_t byteSize) {
    assert(mVertices == NULL);
    mVertices = malloc(byteSize);
}


int Mesh::getFrameVerticesSizeInByte() {
    return mNumVertices * kVertexFormatSize[mVertexFormat];
}


int Mesh::getVerticesSizeInByte() {
    return getFrameVerticesSizeInByte() * mNumKeyFrames;
}


bool Mesh::load() {
    MeshSerializer* serializer = SerializerManager::getInstance()->getMeshSerializer(mPath);
    if (PlatformUtil::isZipped(mPath)) {
        ZipFileDataStream data(mPath);
        serializer->importMesh(data, this);
    } else {
        FileDataStream data(mPath);
        serializer->importMesh(data, this);
    }
    return true;
}

bool Mesh::loadFromShape(Shape& shape) {
    shape.loadIntoMesh(*this);
    return true;
}

SubMesh* Mesh::createSubMesh() {
    SubMesh* sub = new SubMesh(this);
    mSubMeshes.push_back(sub);
    return sub;
}


void Mesh::onLoadComplete() {
    if (!mVbo) {
        mVbo = new VertexVBO(this);
    } else {
        mVbo->sendData(this);
    }
}
