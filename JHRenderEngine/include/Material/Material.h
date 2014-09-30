//
//  Material.h
//
//  Created by Jingshu Huang on 1/18/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Material_h
#define _Material_h

#include "Asset.h"
#include "platformDef.h"


class ShadingTechnique;


// Material encapsulates rendering properties of an object. It encapsulates all
// aspects of the visual appearance, of an object.
class Material : public Asset {
public:
    Material();
    Material(const string& fileName);
    virtual ~Material();

    bool load();
    ShadingTechnique* addShadingTechnique();
    ShadingTechnique* getBestTechnique() const;

    static string kDefaultMaterialName;

private:
    string mPath;  // Full path for material file.

    typedef vector<ShadingTechnique*> ShadingTechniqueList;
    ShadingTechniqueList mTechniques;
};

#endif
