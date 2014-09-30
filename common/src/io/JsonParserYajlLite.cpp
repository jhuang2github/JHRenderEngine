//
//  JsonParserYajlLite.cpp
//
//  Created by Jingshu Huang on 11/26/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "JsonParserYajlLite.h"
#include "ParsedData.h"

static ParsedData* aton(const char* stringVal, const bool shouldBeNumber) {
    ParsedData* data = NULL;
    
    char *end = NULL;
    long long lval = strtoll(stringVal, &end, 0);
    if (*end == '\0') {
        data = (lval >= INT_MIN && lval <= INT_MAX) ? new ParsedData((int)lval) : new ParsedData(lval);
    } else {
        float fval = strtof(stringVal, &end);
        if (*end == '\0') {
            data = new ParsedData(fval);
        } else {
            if (!shouldBeNumber) {
                data = new ParsedData(stringVal);
            }
        }
    }
    
    return data;
}

void JsonParserYajlLite::addData(ParsedData* data) {
    if (!data) {
        return;
    }
    if (!mRoot) {
        mRoot = data;
        return;
    }
    
    assert(mCurFrame != NULL && mCurFrame->mData != NULL);
    switch (mCurFrame->mData->mType) {
        case MAP_TYPE:
            assert(mCurFrame->mCurMapKey.length() > 0);
            mCurFrame->mData->mMap->insert(
                ParsedData::DataMap::value_type(mCurFrame->mCurMapKey, data));
            break;
        case ARRAY_TYPE:
            mCurFrame->mData->mArray->push_back(data);
            break;
        default:
            break;
    }
}

int JsonParserYajlLite::parseNull(const char *marker) {
    // Do nothing. TODO(jhuang): revisit this.
    return 1;
}

int JsonParserYajlLite::parseBoolean(int boolVal, const char *marker) {
    addData(new ParsedData(boolVal));
    return 1;
}

int JsonParserYajlLite::parseNumber(const char *numberVal, const char *marker) {
    addData(aton(numberVal, true));
    return 1;
}

int JsonParserYajlLite::parseString(const char *stringVal, const char *marker) {
    addData(aton(stringVal, false));
    return 1;
}

int JsonParserYajlLite::parseMapKey(const char *stringVal, size_t stringLen,
                                    const char *marker) {
    assert(MAP_TYPE == mCurFrame->mData->mType);
    char* str = (char*)malloc(stringLen + 1);
	strncpy(str, stringVal, stringLen);
	str[stringLen] = '\0';
    mCurFrame->mCurMapKey = string(str);
    free(str);
    return 1;
}

int JsonParserYajlLite::parseStartMap(const char *marker) {
    ParsedData* data = new ParsedData(new ParsedData::DataMap);
    StackFrame* frame = new StackFrame(data);
    addData(data);
    mParseStack.push(frame);
    mCurFrame = frame;
    return 1;
}

int JsonParserYajlLite::parseEndMap(const char *marker) {
    delete mParseStack.top();
    mParseStack.pop();
    if (!mParseStack.empty()) {
        mCurFrame = mParseStack.top();
    } else {
        mCurFrame = NULL;
    }
    return 1;
}

int JsonParserYajlLite::parseStartArray(const char *marker) {
    ParsedData* data = new ParsedData(new ParsedData::DataList);
    StackFrame* frame = new StackFrame(data);
    addData(data);
    mParseStack.push(frame);
    mCurFrame = frame;
    return 1;
}

int JsonParserYajlLite::parseEndArray(const char *marker) {
    delete mParseStack.top();
    mParseStack.pop();
    if (!mParseStack.empty()) {
        mCurFrame = mParseStack.top();
    } else {
        mCurFrame = NULL;
    }
    return 1;
}

ParsedData* JsonParserYajlLite::parse(const char *content, size_t contentSize) {
    LOGD("[JsonParserYajlLite parse] content size = %ld", contentSize);
    
	YajlContext context;
	yajl_handle hand = yajl_alloc(&JsonParserYajlBase::callbacks, NULL, (void *)&context);
	char buffer[YajlContext::BUFFER_SIZE];
	context.buffer = buffer;
	context.hand = hand;
	context.content = content;
	context.parser = this;
    
	yajl_status status = yajl_parse(hand, (const unsigned char *)content, contentSize);
	if (status == yajl_status_ok) {
		status = yajl_complete_parse(hand);
	}
	yajl_free(hand);
    
	if (status != yajl_status_ok) {
        LOGD("JsonParserYajlLite error - parsing error: %d", status);
		return NULL;
	}
    
    return mRoot;
}
