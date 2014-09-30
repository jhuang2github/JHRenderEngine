//
//  MaterialSerializer.cpp
//
//  Created by Jingshu Huang on 2/8/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include <cstdio>
// For DOM-style json parsing
#include "rapidjson/include/document.h"
// For SAX-stuyle json parsing
#include "rapidjson/include/rapidjson.h"
#include "rapidjson/include/reader.h"

#include "DataStream.h"
#include "Material.h"
#include "MaterialSerializer.h"
#include "Pass.h"
#include "ShadingTechnique.h"

using namespace rapidjson;

static const char* kTechniquesKey = "techniques";
static const char* kPassesKey = "passes";
static const char* kShadersKey = "shaders";
static const char* kTexturesKey = "textures";
static const char* kFileKey = "file";
static const char* kVertexShaderKey = "vertexShader";
static const char* kFragmentShaderKey = "fragmentShader";



void MatSerializer::importMaterial(DataStream& stream, Material* material) {
//    importMaterialSAX(stream, material);
    importMaterialDOM(stream, material);
}


// DOM-style parsing with rapidjson
void MatSerializer::importMaterialDOM(DataStream& stream, Material* material) {
    Document doc;
    doc.Parse<0>(stream.getData());

    // Techniques.
    const Value& techniques = doc[kTechniquesKey];
    for (Value::ConstMemberIterator techIter = techniques.MemberBegin();
         techIter != techniques.MemberEnd(); ++techIter) {
        ShadingTechnique* technique = material->addShadingTechnique();

        // Passes.
        const Value& passes = (techIter->value)[kPassesKey];
        int passIndex = 0;
        for (Value::ConstMemberIterator passIter = passes.MemberBegin();
             passIter != passes.MemberEnd(); ++passIter) {
            const Value& passInfo = passIter->value;
            
            // Shaders.
            const Value& shaders = passInfo[kShadersKey];
            string vShader = shaders[kVertexShaderKey].GetString();
            string fShader = shaders[kFragmentShaderKey].GetString();
            assert(!vShader.empty() && !fShader.empty());
            Pass* pass = technique->addPass(vShader, fShader);
            
            // Textures.
            const Value& textures = passInfo[kTexturesKey];
            if (textures.IsObject()) {
                for (Value::ConstMemberIterator texIter = textures.MemberBegin();
                     texIter != textures.MemberEnd(); ++texIter) {
                    stringstream stream(texIter->name.GetString());
                    int texUnit = 0;
                    stream >> texUnit;
                    string texFileName = texIter->value[kFileKey].GetString();
                    assert(!texFileName.empty());
                    pass->loadTexture(texFileName, texUnit);
                }
            }

            pass->setHash(passIndex);
            passIndex++;
        }
    }
}


// SAX-style parsing with rapidjson
template<typename Encoding = rapidjson::UTF8<> >
class MaterialJsonReaderHandler {
private:
    const int static kLevelIndent = 4;
    typedef typename Encoding::Ch Ch;

    enum {
        Name,
        Value
    } state;

    int indent;
    int inArray;
    std::string name;

public:
    MaterialJsonReaderHandler() {
        indent = 0;
        inArray = 0;
        state = Name;
    }

    void Null()  {
        printf("null\n");
        state = Name;
    }

    void Bool(bool b)  {
        printf("%s\n", b ? "true" : "false");
        state = Name;
    }

    void Int(int v) {
        printf("%d\n", v);
        state = Name;
    }

    void Uint(unsigned v)  {
        printf("%u\n", v);
        state = Name;
    }

    void Int64(int64_t v) {
        printf("%lld\n", v);
        state = Name;
    }

    void Uint64(uint64_t v) {
        printf("%llu\n", v);
        state = Name;
    }

    void Double(double v) {
        printf("%f\n", v);
        state = Name;
    }

    void String(const Ch* s, rapidjson::SizeType l, bool b) {
        if (state == Name) {
            string space = string(indent, ' ');
            printf("%s%s: ", space.c_str(), s);
            name.assign(s, l);
            state = Value;
            string str = string(s);
        } else {
            printf("%s\n", s);
            state = Name;
        }
    }

    // Handler for dictionary.
    void StartObject() {
        printf("{\n");
        indent += kLevelIndent;
        state = Name;
    }

    void EndObject(rapidjson::SizeType) {
        indent -= kLevelIndent;
        string space = string(indent, ' ');
        printf("%s}\n", space.c_str());
    }

    // Handler for array.
    void StartArray() {
        printf("[\n");
        indent += kLevelIndent;

        inArray++;
        state = Value;
    }

    void EndArray(rapidjson::SizeType) {
        indent -= kLevelIndent;
        string space = string(indent, ' ');
        printf("%s]\n", space.c_str());
        inArray--;
    }
};


void MatSerializer::importMaterialSAX(DataStream& stream, Material* material) {
    GenericReader<UTF8<> > reader;
    const unsigned int parseFlags = kParseDefaultFlags;
    GenericStringStream<UTF8<> > jsonStream(stream.getData());
    MaterialJsonReaderHandler<UTF8<> > handler;
    reader.Parse<parseFlags, GenericStringStream<UTF8<> >, MaterialJsonReaderHandler<UTF8<> > >(
                    jsonStream, handler);
}

// Legacy format:
// shader simple.vsh simple.fsh
// texture cow.pvr
//
//void MatSerializer::importMaterial(DataStream& stream, Material* material) {
//    StrDataStream& input = reinterpret_cast<StrDataStream&>(stream);
//    stringstream& strStream = input.getStrStream();
//
//    string line;
//    while (std::getline(strStream, line)) {
//        stringstream lineStream;
//        lineStream.str(line);
//
//        string type;
//        lineStream >> type;
//        if (type == kShader) {
//            string vShader, fShader;
//            lineStream >> vShader >> fShader;
//            assert(!vShader.empty() && !fShader.empty());
//            material->addShader(vShader, fShader);
//        } else if (type == kTexture) {
//            string texFileName;
//            lineStream >> texFileName;
//            assert(!texFileName.empty());
//            material->addTexture(texFileName);
//        }
//    }
//}
