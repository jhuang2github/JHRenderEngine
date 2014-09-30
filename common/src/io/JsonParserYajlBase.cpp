//
//  JsonParserYajlBase.cpp
//
//  Created by Jingshu Huang on 11/21/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "JsonParserYajlBase.h"


yajl_callbacks JsonParserYajlBase::callbacks = {
    parseNull,
    parseBoolean,
    NULL,
    NULL,
    parseNumber,
    parseString,
    parseStartMap,
    parseMapKey,
    parseEndMap,
    parseStartArray,
    parseEndArray
};


#pragma mark - JsonParserYajlBase static

int JsonParserYajlBase::parseNull(void *ctx) {
	YajlContext *context = (YajlContext *)ctx;
	return context->parser->parseNull(
        context->content + yajl_get_bytes_consumed(context->hand));
}

int JsonParserYajlBase::parseBoolean(void *ctx, int boolVal) {
	YajlContext *context = (YajlContext *)ctx;
	return context->parser->parseBoolean(boolVal, context->content + yajl_get_bytes_consumed(context->hand));
}

int JsonParserYajlBase::parseNumber(void *ctx, const char * numberVal, size_t numberLen) {
	YajlContext *context = (YajlContext *)ctx;
	strncpy(context->buffer, numberVal, numberLen);
	context->buffer[numberLen] = '\0';
	return context->parser->parseNumber(context->buffer, context->content + yajl_get_bytes_consumed(context->hand));
}

int JsonParserYajlBase::parseString(void *ctx, const unsigned char *stringVal, size_t stringLen) {
	YajlContext *context = (YajlContext *)ctx;
	if (stringLen < YajlContext::BUFFER_SIZE) {
		strncpy(context->buffer, (const char *)stringVal, stringLen);
		context->buffer[stringLen] = '\0';
		return context->parser->parseString(context->buffer,
            context->content + yajl_get_bytes_consumed(context->hand));
	} else {
		void *buffer = malloc(stringLen + 1);
        memcpy(buffer, stringVal, stringLen);
		((char *)buffer)[stringLen] = '\0';
		int rtn = context->parser->parseString(
            (const char *)buffer, context->content +
            yajl_get_bytes_consumed(context->hand));
        free(buffer);
        return rtn;
	}
}

int JsonParserYajlBase::parseMapKey(void *ctx, const unsigned char *stringVal, size_t stringLen) {
	YajlContext *context = (YajlContext *)ctx;
	return context->parser->parseMapKey(
        (const char *)stringVal, stringLen,
        context->content + yajl_get_bytes_consumed(context->hand));
}

int JsonParserYajlBase::parseStartMap(void *ctx) {
	YajlContext *context = (YajlContext *)ctx;
	return context->parser->parseStartMap(
        context->content + yajl_get_bytes_consumed(context->hand));
}

int JsonParserYajlBase::parseEndMap(void *ctx) {
	YajlContext *context = (YajlContext *)ctx;
	return context->parser->parseEndMap(
        context->content + yajl_get_bytes_consumed(context->hand));
}

int JsonParserYajlBase::parseStartArray(void * ctx) {
	YajlContext *context = (YajlContext *)ctx;
	return context->parser->parseStartArray(
        context->content + yajl_get_bytes_consumed(context->hand));
}

int JsonParserYajlBase::parseEndArray(void * ctx) {
	YajlContext *context = (YajlContext *)ctx;
	return context->parser->parseEndArray(
        context->content + yajl_get_bytes_consumed(context->hand));
}

