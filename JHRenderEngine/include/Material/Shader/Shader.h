//
//  Shader.h
//
//  Created by Jingshu Huang on 12/20/12.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef Shader_h
#define Shader_h

#include "Asset.h"
#include "ShaderAttr.h"
#include "ShaderUnfm.h"


//class StrDataStream;

typedef vector<ShaderAttr*> ShaderAttrList;
typedef vector<ShaderUnfm*> ShaderUnfmList;

// Shader program.
class Shader : public Asset {
  public:
    Shader(const string&);
    Shader(const string& vShader, const string& fShader);
    ~Shader(void);

    bool load();
    GLuint loadShaderSources(const char*, const char*);

    inline const ShaderAttrList& getAttributes() { return mAttrs; }
    void setAttrStrideAndOffset(const stringId& inNameId, const int strideInByte,
                                uchar* inOffset);
    void setAttrStride(const stringId& inNameId, const int strideInByte);
    void setAttrOffset(const stringId& inNameId, byte* inOffset);
    void enableAttribute(const int, const GLuint) const;
    void enableAttributes(const GLuint) const;
    void disableAttributes() const;

    inline ShaderUnfmList& getUniforms(const EShaderUniformScope scope) {
        if (!mUniforms[scope]) {
            mUniforms[scope] = new ShaderUnfmList;
        }
        return *mUniforms[scope];
    }
    void setUniformValue(const EShaderUniformScope scope, const stringId& nameId,
                         float* value);
    void enableUniforms(const EShaderUniformScope);

    inline GLuint getProgram() { return mProgram; }
    void use() const;

    // If the pass requires more than the default number of tex units. Get
    // the logical location of the additional tex related uniform on the fly.
    //void ensureTexUnit(const int texUnit);
    
private:
    static GLuint compileShader(const GLenum type, const char* shaderSrc);
    static void deleteShaders(GLuint vShaderId, GLuint fShaderId);

    void addAttribute(const string& inName, int inSizeInFloat);
    ShaderUnfm* addUniform(const EShaderUniformScope,
                           const string&,
                           const int,
                           const EShaderUnfmType);
    ShaderUnfm* addUniform(const EShaderUniformScope scope,
                           const string& name,
                           const int count,
                           const EShaderUnfmType type,
                           const float value);
    ShaderUnfm* addTextureUniform();

    // Map from shader uniform scope to a list of uniforms.
    typedef unordered_map<uint, ShaderUnfmList*> ShaderUniformMap;

    void setupShaderParams();
//    void setupShaderParams(StrDataStream& shaderStrStream);

    string              mVertexShaderPath;
    string              mFragShaderPath;
    GLuint              mProgram;
    ShaderAttrList      mAttrs;     // 0: position
    ShaderUniformMap    mUniforms;

    unordered_set<int>       mActiveTexUnits;
};

#endif
