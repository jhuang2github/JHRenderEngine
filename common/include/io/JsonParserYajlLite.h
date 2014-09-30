//
//  JsonParserYajlLite.h
//
//  Created by Jingshu Huang on 11/26/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _JsonParserYajlLite_h
#define _JsonParserYajlLite_h

#include <stdexcept>
#include "JsonParserYajlBase.h"

class ParsedData;


class JsonParserYajlLite : public JsonParserYajlBase {
public:
    JsonParserYajlLite() : JsonParserYajlBase() { mRoot = NULL; mCurFrame = NULL; }
    ~JsonParserYajlLite() {}
    
    // Override
	int parseNull(const char *marker);
	int parseBoolean(int boolVal, const char *marker);
	int parseNumber(const char *numberVal, const char *marker);
	int parseString(const char *stringVal, const char *marker);
	int parseMapKey(const char *stringVal, size_t stringLen, const char *marker);
	int parseStartMap(const char *marker);
	int parseEndMap(const char *marker);
	int parseStartArray(const char *marker);
	int parseEndArray(const char *marker);
    
    ParsedData* parse(const char *content, size_t contentSize);
    
private:
    void addData(ParsedData* data);

    class StackFrame {
    public:
        StackFrame(ParsedData* data) : mData(data), mCurMapKey("") {}
        StackFrame(const string& str, ParsedData* data) : mData(data), mCurMapKey(str) {}
        ~StackFrame() { mCurMapKey = ""; }

        ParsedData* mData; // Does not own the data.
        string mCurMapKey;
    };

    stack<StackFrame*> mParseStack;
    StackFrame* mCurFrame;

    ParsedData* mRoot;  // Owns the data.
};

#endif
