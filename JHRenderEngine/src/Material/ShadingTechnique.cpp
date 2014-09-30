//
//  ShadingTechnique.cpp
//
//  Created by Jingshu Huang on 1/23/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "Material.h"
#include "Pass.h"
#include "ShadingTechnique.h"


ShadingTechnique::ShadingTechnique(Material* parent) {
    mParent = parent;
    mPasses.clear();
}

ShadingTechnique::~ShadingTechnique() {
    for (RenderPassList::const_iterator iter = mPasses.begin();
         iter != mPasses.end(); ++iter) {
        if (*iter) {
            delete *iter;
        }
    }
    mPasses.clear();
}

Pass* ShadingTechnique::addPass(const string& vShaderFileName,
                                const string& fShaderFileName) {
    Pass* pass = new Pass(this);
    pass->loadShader(vShaderFileName, fShaderFileName);
    mPasses.push_back(pass);
    return pass;
}

