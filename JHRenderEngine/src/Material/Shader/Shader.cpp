//
//  Shader.cpp
//
//  Created by Jingshu Huang on 12/20/12.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "DataStream.h"
#include "GLUtil.h"
#include "platformUtil.h"
#include "platformPathUtil.h"
#include "SerializerManager.h"
#include "Shader.h"
#include "ShaderSerializer.h"
#include "ShaderManager.h"


// Shader key words.
const static string kAttribute = "attribute";
const static string kUniform = "uniform";


#pragma Shader

Shader::Shader(const string& shaderPairName) {
    string vertShader, fragShader;
    decodeShaderName(shaderPairName, vertShader, fragShader);

    string path = PlatformPathUtil::assetPath + "/" + PlatformPathUtil::shaderSubPath + "/";
    mVertexShaderPath =  path + vertShader;
    mFragShaderPath = path + fragShader;
    mActiveTexUnits.clear();
    mProgram = 0;
}

Shader::Shader(const string& vertShader, const string& fragShader) {
    string path = PlatformPathUtil::assetPath + "/" + PlatformPathUtil::shaderSubPath + "/";
    mVertexShaderPath = path + vertShader;
    mFragShaderPath = path + fragShader;
    mActiveTexUnits.clear();
    mProgram = 0;
}

Shader::~Shader() {
    if (mProgram) {
        GL_deleteProgram(mProgram);
    }
    mProgram = 0;

    for (ShaderAttrList::const_iterator iter = mAttrs.begin();
         iter != mAttrs.end(); ++iter) {
        delete *iter;
    }
    mAttrs.clear();

    for (ShaderUniformMap::iterator iter = mUniforms.begin();
         iter != mUniforms.end(); ++iter) {
        ShaderUnfmList* unfms = iter->second;
        for (ShaderUnfmList::const_iterator it = unfms->begin();
             it != unfms->end(); ++it) {
            if (*it) {
                delete *it;
            }
        }
        unfms->clear();
        delete unfms;
    }
    mUniforms.clear();

    mActiveTexUnits.clear();
}


# pragma shader files loading and program linking

bool Shader::load() {
    ShaderSerializer* serializer = SerializerManager::getInstance()->getShaderSerializer(mVertexShaderPath);

    // Read the content of the shaders.
    FileDataStream* vertShader = NULL;
    FileDataStream* fragShader = NULL;
    if (PlatformUtil::isZipped(mVertexShaderPath)) {
        vertShader = new ZipFileDataStream(mVertexShaderPath);
    } else {
        vertShader = new FileDataStream(mVertexShaderPath);
    }

    if (PlatformUtil::isZipped(mFragShaderPath)) {
        fragShader = new ZipFileDataStream(mFragShaderPath);
    } else {
        fragShader = new FileDataStream(mFragShaderPath);
    }
    vector<FileDataStream*> shaderSrc;
    shaderSrc.push_back(vertShader);
    shaderSrc.push_back(fragShader);

    // build the shader program.
    bool success = serializer->importShader(shaderSrc, this);
    if (success) {
        setupShaderParams();
    }

    if (vertShader) {
        delete vertShader;
    }
    if (fragShader) {
        delete fragShader;
    }

    return success;
}

// Load ascii shaders.
GLuint Shader::loadShaderSources(const char* vertShaderSrc,
                                 const char* fragShaderSrc) {
    mProgram = GL_createProgram();

    GLuint vShader = compileShader(GL_VERTEX_SHADER, vertShaderSrc);
    GLuint fShader = compileShader(GL_FRAGMENT_SHADER, fragShaderSrc);
	if (!vShader || !fShader) {
        LOGD("[Shader loadShaderSource]: Error in compileShader. vshader = %d, fshader = %d. \n", vShader, fShader);
        LOGD("------- vert shader --------\n%s\n", vertShaderSrc);
        LOGD("------- frag shader --------\n%s\n", fragShaderSrc);
        GL_deleteProgram(mProgram);
        mProgram = 0;
	} else {
        GL_attachShader(mProgram, vShader);
        GL_attachShader(mProgram, fShader);
        bool success = GL_linkProgram(mProgram); // && GL_validateProgram(mProgram);
        if (!success) {
            LOGD("[Shader loadShaderSource]: Error in attach/link shader program.\n");
            mProgram = 0;
        }
        if (glGetErrorPrettyPrint(__LINE__)) {
            LOGD("[Shader loadShaderSource]: Error in loading shader program.\n");
            LOGD("\nvertex shader path = %s\n", mVertexShaderPath.c_str());
            LOGD("\nfragment shader path= %s\n", mFragShaderPath.c_str());            
        }
    }

    deleteShaders(vShader,  fShader);
    return mProgram;
}

GLuint Shader::compileShader(const GLenum type, const char* shaderSrc) {
    assert(shaderSrc != NULL);

    GLuint shaderId = glCreateShader(type);
    GL_shaderSource(shaderId, 1, &shaderSrc, NULL);
    GL_compileShader(shaderId);
    if (glGetErrorPrettyPrint(__LINE__)) {
        const string shaderType = (type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
        LOGD("\n[Shader compileShader]: Error\n------- %s shader ----\n\n %s\n------\n",
             shaderType.c_str(), shaderSrc);
    }
    
    return shaderId;
}

void Shader::deleteShaders(const GLuint vShaderId, const GLuint fShaderId) {
	if (vShaderId) {
		GL_deleteShader(vShaderId);
	}
	if (fShaderId) {
		GL_deleteShader(fShaderId);
	}
}


# pragma shader params setup

// Hard-coded shader params. Add all the possible params here, including both
// active or inactive, existing or non-existing ones.
void Shader::setupShaderParams() {
    if (mProgram <= 0) {
        return;
    }
    
    addAttribute(ShaderParam::kPosition, 3);    // attr 0
    addAttribute(ShaderParam::kTexCoords, 2);  // attr 1

    // Scene layer scoped uniforms.
    addUniform(UNFM_SCENE_LAYER_SCOPE, ShaderParam::kViewProjMat, 1, _Matrix4fv);
    addUniform(UNFM_SCENE_LAYER_SCOPE, ShaderParam::kProjectionMat, 1, _Matrix4fv);
    addUniform(UNFM_SCENE_LAYER_SCOPE, ShaderParam::kViewMat, 1, _Matrix4fv);
    
    // Model scoped uniforms.
	addUniform(UNFM_MODEL_SCOPE, ShaderParam::kModelMat, 1, _Matrix4fv);
    mActiveTexUnits.clear();
    while (addTextureUniform()) {
        // do nothing.
    }
}

//// TODO(jhuang): Parse GLSL file to set up shader params.
//void Shader::setupShaderParams(StrDataStream& shaderStrStream) {
//    string line;
//    stringstream& strStream = shaderStrStream.getStrStream();
//    while (std::getline(strStream, line)) {
//        stringstream lineStream;
//        lineStream.str(line);
//
//        string firstWord, type, name;
//        lineStream >> firstWord;
//        if (firstWord == kUniform) {
//            lineStream >> type >> name;
//            assert(!type.empty() && !name.empty());
//            //addUniform(UNFM_SCENE_LAYER_SCOPE, name, 1, _Matrix4fv);
//        } else if (firstWord == kAttribute) {
//            lineStream >> type >> name;
//            assert(!type.empty() && !name.empty());
//            //addAttribute(name, 3);
//        }
//    }
//}

void Shader::addAttribute(const string& name, const int sizeInFloat) {
    int loc = glGetAttribLocation(mProgram, name.c_str());
    if (loc < 0) {
        return;
    }
    mAttrs.push_back(new ShaderAttr(name,
                                    loc,
                                    sizeInFloat));
}

ShaderUnfm* Shader::addUniform(const EShaderUniformScope scope,
                               const string& name,
                               const int count,
                               const EShaderUnfmType type) {
    int loc = glGetUniformLocation(mProgram, name.c_str());
    if (loc < 0) {
        return NULL;
    }
    ShaderUnfm* unfm = new ShaderUnfm(name, loc, count, type);
    getUniforms(scope).push_back(unfm);
    return unfm;
}

ShaderUnfm* Shader::addUniform(const EShaderUniformScope scope,
                               const string& name,
                               const int count,
                               const EShaderUnfmType type,
                               const float value) {
    int loc = glGetUniformLocation(mProgram, name.c_str());
    if (loc < 0) {
        return NULL;
    }
    ShaderUnfm* unfm = new ShaderUnfm(name,
                                      loc,
                                      count,
                                      type);
    unfm->setValue(value);
    getUniforms(scope).push_back(unfm);
    return unfm;
}


ShaderUnfm* Shader::addTextureUniform() {
    int texUnit = mActiveTexUnits.size();

    stringstream stream;
    stream << ShaderParam::kTexturePrefix << texUnit;
    string name = stream.str();
    ShaderUnfm* unfm = addUniform(UNFM_MODEL_SCOPE, name, 1, _1i, texUnit);
    if (unfm) {
        mActiveTexUnits.insert(texUnit);
    }
    return unfm;
}

// If the pass specifies more than the shader's active number of tex units. Get
// the logical location of the additional tex related uniform on the fly.
//void Shader::ensureTexUnit(const int texUnit) {
////    if (mActiveTexUnits.find(texUnit) == mActiveTexUnits.end()) {
////        addTextureUniform();
////    }
//    LOGD("[ Shader::ensureTexUnit ] Warning: Pass specifies more textures than the shader requires.\n");
//}


#pragma shader params usage

void Shader::setAttrStrideAndOffset(const stringId& inNameId, const int strideInByte, uchar* inOffset) {
    for (ShaderAttrList::iterator iter = mAttrs.begin();
         iter != mAttrs.end(); ++iter) {
        assert(*iter);
        stringId nameId = (*iter)->getNameId();
        if (nameId == inNameId) {
            (*iter)->setStride(strideInByte);
            (*iter)->setOffset(inOffset);
            return;
        }
    }
    LOGE("[Shader::setAttrStrideAndOffset] Cannot find attribute Id=%d\n", (int)inNameId);
}

void Shader::setAttrStride(const stringId& inNameId, const int strideInByte) {
    for (ShaderAttrList::iterator iter = mAttrs.begin();
         iter != mAttrs.end(); ++iter) {
        assert(*iter);
        stringId nameId = (*iter)->getNameId();
        if (nameId == inNameId) {
            (*iter)->setStride(strideInByte);
            return;
        }
    }
    LOGE("[Shader::setAttrStride] Cannot find attribute Id=%d\n", (int)inNameId);
}

void Shader::setAttrOffset(const stringId& inNameId, uchar* inOffset) {
    for (ShaderAttrList::iterator iter = mAttrs.begin();
         iter != mAttrs.end(); ++iter) {
        assert(*iter);
        stringId nameId = (*iter)->getNameId();
        if (nameId == inNameId) {
            (*iter)->setOffset(inOffset);
            return;
        }
    }
    LOGD("[Shader::setAttrOffset] Cannot find attribute Id=%d\n", (int)inNameId);
}

// Set up all attributes parameters. All vertex info like position, texcoords,
// etc, are packed in one VBO.
void Shader::enableAttributes(const GLuint vboId) const {
    GL_bindBuffer(GL_ARRAY_BUFFER, vboId);
    for (ShaderAttrList::const_iterator iter = mAttrs.begin();
         iter != mAttrs.end(); ++iter) {
        assert(*iter);
        (*iter)->enable();
    }
}

// Set up an attribute parameter given its logical index.
void Shader::enableAttribute(const int attrIndex, const GLuint vboId) const {
    assert (attrIndex < mAttrs.size());
    ShaderAttr *attr = mAttrs.at(attrIndex);
    if(!attr || attr->getLocation() < 0) {
        return;
    }

    GL_bindBuffer(GL_ARRAY_BUFFER, vboId);
    attr->enable();
}

void Shader::disableAttributes() const {
    for (ShaderAttrList::const_iterator iter = mAttrs.begin();
         iter != mAttrs.end(); ++iter) {
        assert(*iter);
        (*iter)->disable();
    }
}

void Shader::enableUniforms(const EShaderUniformScope scope) {
    const ShaderUnfmList& unfms = getUniforms(scope);
    for (ShaderUnfmList::const_iterator iter = unfms.begin();
         iter != unfms.end(); ++iter) {
        assert(*iter);
        (*iter)->enable();
    }
}

void Shader::setUniformValue(const EShaderUniformScope scope,
                             const stringId& nameId, float* value) {
    const ShaderUnfmList& unfms = getUniforms(scope);
    for (ShaderUnfmList::const_iterator iter = unfms.begin();
         iter != unfms.end(); ++iter) {
        ShaderUnfm* unfm = *iter;
        if (unfm->getNameId() == nameId) {
            unfm->setValue(value);
            return;
        }
    }
    LOGD("[Shader::setUniformValue] Cannot find uniform Id=%d (scope = %d)\n",
         (int)nameId, scope);
}

#pragma shader program usage

void Shader::use() const {
    assert(mProgram > 0);
    GL_useProgram(mProgram);
}
