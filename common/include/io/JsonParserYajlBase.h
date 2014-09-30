//
//  JsonParserYajlBase.h
//
//  Created by Jingshu Huang on 11/21/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _JsonParserYajl_h
#define _JsonParserYajl_h

#include <stack>

#include "ParsedData.h"
#include <yajl_parse.h>
#include <yajl_gen.h>


class JsonParserYajlBase {
public:
    virtual ~JsonParserYajlBase() {}
	virtual int parseNull(const char *marker) = 0;
	virtual int parseBoolean(int boolVal, const char *marker) = 0;
	virtual int parseNumber(const char *numberVal, const char *marker) = 0;
	virtual int parseString(const char *stringVal, const char *marker) = 0;
	virtual int parseMapKey(const char *stringVal, size_t stringLen, const char *marker) = 0;
	virtual int parseStartMap(const char *marker) = 0;
	virtual int parseEndMap(const char *marker) = 0;
	virtual int parseStartArray(const char *marker) = 0;
	virtual int parseEndArray(const char *marker) = 0;

    static yajl_callbacks callbacks;

private:
	static int parseNull(void *ctx);
    static int parseBoolean(void *ctx, int boolVal);
    static int parseNumber(void *ctx, const char * numberVal, size_t numberLen);
    static int parseString(void *ctx, const unsigned char *stringVal, size_t stringLen);
    static int parseMapKey(void *ctx, const unsigned char *stringVal, size_t stringLen);
    static int parseStartMap(void *ctx);
    static int parseEndMap(void *ctx);
    static int parseStartArray(void * ctx);
    static int parseEndArray(void * ctx);
};


class YajlContext {
public:
    YajlContext() {}
    ~YajlContext() {}
    
	static const int BUFFER_SIZE = 65536; // 2^16
	JsonParserYajlBase *parser;
	yajl_handle hand;
	const char *content;
	char *buffer;
};

#endif
