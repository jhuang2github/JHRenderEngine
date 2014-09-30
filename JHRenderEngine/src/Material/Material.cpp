//
//  Material.cpp
//
//  Created by Jingshu Huang on 1/18/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "DataStream.h"
#include "MaterialManager.h"
#include "MaterialSerializer.h"
#include "PlatformUtil.h"
#include "PlatformPathUtil.h"
#include "SerializerManager.h"
#include "ShadingTechnique.h"

string Material::kDefaultMaterialName = "default.material";


Material::Material() {
    mPath.clear();
}

Material::Material(const string& fileName) {
    mPath = PlatformPathUtil::assetPath + "/" + PlatformPathUtil::materialSubPath + "/" + fileName;
}

Material::~Material() {
    for (ShadingTechniqueList::const_iterator iter = mTechniques.begin();
         iter != mTechniques.end(); ++iter) {
        if (*iter) {
            delete *iter;
        }
    }
    mTechniques.clear();
}

bool Material::load() {
    MaterialSerializer* serializer = SerializerManager::getInstance()->getMaterialSerializer(mPath);
    if (PlatformUtil::isZipped(mPath)) {
        ZipFileDataStream data(mPath);
        serializer->importMaterial(data, this);
    } else {
        FileDataStream data(mPath);
        serializer->importMaterial(data, this);
    }
    return true;
}

ShadingTechnique* Material::addShadingTechnique() {
    ShadingTechnique* tech = new ShadingTechnique(this);
    mTechniques.push_back(tech);
    return tech;
}

ShadingTechnique* Material::getBestTechnique() const {
    if (mTechniques.empty()) {
        return NULL;
    }
    // For now, we just use the first technique in a material.
    return mTechniques[0];
}

