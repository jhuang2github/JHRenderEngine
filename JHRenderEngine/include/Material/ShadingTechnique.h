//
//  ShadingTechnique.h
//
//  Created by Jingshu Huang on 1/23/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _ShadingTechnique_h
#define _ShadingTechnique_h

#include "platformDef.h"

class Material;
class Pass;

typedef vector<Pass*> RenderPassList;


// Shading technique representing an approach to rendering a material. A
// technique can be either one or multiple passes.
class ShadingTechnique {
public:
    ShadingTechnique(Material* parent);
    ~ShadingTechnique();

    // If the technique is supported by the current hardware.
    bool isSupported() const;

    Pass* addPass(const string& vShaderFileName, const string& fShaderFileName);
    inline RenderPassList& getPasses() { return mPasses; }

public:
    Material* mParent;
    RenderPassList mPasses;

    // Scheme index, derived from scheme name. The names are held by
    // MaterialManager. For speed an index is used here.
//    ushort mSchemeIndex;
};

#endif
