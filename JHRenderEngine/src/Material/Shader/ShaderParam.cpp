//
//  ShaderParam.cpp
//
//  Created by Jingshu Huang on 5/21/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "ShaderParam.h"


const string ShaderParam::kPosition = "position";
const string ShaderParam::kTexCoords = "texCoords";
const string ShaderParam::kColor = "color";
const string ShaderParam::kViewProjMat = "viewProjMat";
const string ShaderParam::kProjectionMat = "projectionMat";
const string ShaderParam::kViewMat = "viewMat";
const string ShaderParam::kModelMat = "modelMat";
const string ShaderParam::kTexturePrefix = "texture";


ShaderParam::ParamNameToNameIdMap ShaderParam::kParamNameToNameIdMap =
    ShaderParam::createParamNameToNameIdMap();


ShaderParam::ParamNameToNameIdMap ShaderParam::createParamNameToNameIdMap() {
    ParamNameToNameIdMap nameMap;
    nameMap[kPosition] = kPositionId;
    nameMap[kTexCoords] = kTexCoordsId;
    nameMap[kColor] = kColorId;
    nameMap[kViewProjMat] = kViewProjMatId;
    nameMap[kProjectionMat] = kProjectionMatId;
    nameMap[kViewMat] = kViewMatId;
    nameMap[kModelMat] = kModelMatId;
    nameMap[kTexturePrefix] = kTexturePrefixId;
    return nameMap;
}
