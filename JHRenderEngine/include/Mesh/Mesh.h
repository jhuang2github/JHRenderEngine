//
//  Mesh.h
//
//  Created by Jingshu Huang on 1/14/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Mesh_h
#define _Mesh_h

#include "Asset.h"
#include "bound.h"
#include "vector3.h"


class AxisAlignedBox;
class Shape;
class SubMesh;
class VertexVBO;


// These fields have to use primitives instead of vector{2|3|4} since they are
// used for sending data to graphic card.
typedef struct {
    float3 position;
} VertexPos;

typedef struct {
    float3 position;
    float2 texcoord;
} VertexPosUV;

typedef struct {
    float3 position;
    uchar4 color;
} VertexPosColor;

typedef struct {
    float3 position;
    float2 texcoord;
    uchar4 color;
} VertexPosUVColor;

typedef struct {
    float3 position;
    float2 texcoord;
    uchar4 color;
    float4 blendindices;
    float4 blendweights;
} VertexSkinned;


typedef enum {
    VT_Pos = 0,
    VT_PosUV,
    VT_PosColor,
    VT_PosUVColor,
    VT_Skinned,
    VT_NumVertexFormat
} EVertexFormat;


typedef vector<SubMesh*> SubMeshList;

typedef unordered_map<int, int> VertexFormatSizeMap;
typedef unordered_map<int, byte*> AttrToOffsetMap;
typedef unordered_map<int, AttrToOffsetMap> VertexFormatToAttrOffsetsMap;


// A mesh may be a compound construct consisting of a list of submeshes.
// Each submesh may have different materials.
class Mesh : public Asset {
public:
    Mesh();
	Mesh(const string& fileName);
	virtual ~Mesh();

    inline void setVertexFormat(const EVertexFormat format) { mVertexFormat = format; }
    inline EVertexFormat getVertexFormat() { return mVertexFormat; }
    inline void* getVertices() { return mVertices; }
    inline void setNumVertices(int num) { mNumVertices = num; }
    inline int getNumVertices() { return mNumVertices; }
    inline void setNumKeyFrames(int num) { mNumKeyFrames = num; }
    inline int getNumKeyFrames() { return mNumKeyFrames; }
    inline SubMeshList& getSubMeshes() { return mSubMeshes; }
    inline VertexVBO* getVertexVbo() { return mVbo; }
    int getFrameVerticesSizeInByte();
    int getVerticesSizeInByte();

    void setObjectBound(const Vector3& min, const Vector3& max);
    void setObjectBound(const AxisAlignedBox& box);
    inline void setObjectBound(const Bound& bound) { mObjBound = bound; }
    inline const Bound& getObjectBound(void) const { return mObjBound; }

    bool load();
    bool loadFromShape(Shape& shape);
    void onLoadComplete();
    inline bool isLoaded() { return mVertices != NULL; }
    SubMesh* createSubMesh();
    void ensureVertexCapacity(size_t byteSize);

    static int kPosIndex;
    static int kTexCoordIndex;
    static int kColorIndex;
    static int kBlendIndicesIndex;
    static int kBlendWeightsIndex;
    static VertexFormatSizeMap kVertexFormatSize;
    static VertexFormatToAttrOffsetsMap kVertexFormatToAttrOffsets;
    static byte* getOffset(const EVertexFormat format, const int AttrIndex);
    static int getStride(EVertexFormat format);

    static string kDefaultMeshName;

private:
    static VertexFormatSizeMap createVertexFormatSizeMap();
    static VertexFormatToAttrOffsetsMap createVertexFormatToAttrOffsetsMap();

    EVertexFormat    mVertexFormat;
	void*            mVertices;      // vertex stream. Owns the data.
    int              mNumVertices;   // Number of vertices per key frame.
    int              mNumKeyFrames;
    SubMeshList      mSubMeshes;

    string           mPath;          // Full path to the mesh file.

    VertexVBO*       mVbo;

    Bound            mObjBound;      // Bounds in object space.
};

#endif
