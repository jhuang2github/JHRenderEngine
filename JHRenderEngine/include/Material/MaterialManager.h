//
//  MaterialManager.h
//
//  Created by Jingshu Huang on 1/18/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _MaterialManager_h
#define _MaterialManager_h

#include "AssetManager.h"
#include "platformDef.h"
#include "Material.h"
#include "ShaderManager.h"
#include "Singleton.h"
#include "TextureManager.h"


class MaterialManager : public Singleton<MaterialManager>, public AssetManager<string, Material> {
public:
    Material* getDefaultMaterial() {
        if (!mDefaultMaterial) {
            mDefaultMaterial = load(Material::kDefaultMaterialName);
        }
        return mDefaultMaterial;
    }

private:
    Material* mDefaultMaterial;
};


//void MaterialManager::initialise(void)
//{
//    // Set up default material - don't use name contructor as we want to avoid applying defaults
//    mDefaultSettings = create("DefaultSettings", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
//    // Add a single technique and pass, non-programmable
//    mDefaultSettings->createTechnique()->createPass();
//    
//    // Set up a lit base white material
//    create("BaseWhite", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
//    // Set up an unlit base white material
//    MaterialPtr baseWhiteNoLighting = create("BaseWhiteNoLighting",
//                                             ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
//    baseWhiteNoLighting->setLightingEnabled(false);
//    
//}

#endif
