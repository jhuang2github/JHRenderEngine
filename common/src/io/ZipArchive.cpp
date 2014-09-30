//
//  ZipArchive.cpp
//
//  Created by Jingshu Huang on 1/31/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include <zlib.h>
#include "ZipArchive.h"


static const uint zbufferSize = 131072;
static byte *zbuffer = new byte[zbufferSize];

void *ZipArchive::deflateData(void *data, uint dataSize, uint& zipSize) {
	if (!data || dataSize == 0) {
		return NULL;
	}
    
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
		return NULL;
	}
    
	strm.avail_in = dataSize;
	strm.next_in = (byte *)data;
    
	byte *out = NULL;
	if (zipSize > 0) {
		out = new byte[zipSize];
		strm.avail_out = zipSize;
		strm.next_out = out;
		if (deflate(&strm, Z_FINISH) == Z_STREAM_ERROR) {
			delete out;
			deflateEnd(&strm);
			return NULL;
		}
	} else {
	    uint have = 0;
		zipSize = 0;
		do {
			strm.avail_out = zbufferSize;
			strm.next_out = zbuffer;
			if (deflate(&strm, Z_FINISH) == Z_STREAM_ERROR) {
				if (out) {
					delete out;
				}
				deflateEnd(&strm);
				return NULL;
			}
			have = zbufferSize - strm.avail_out;
			if (have > 0) {
				if (out) {
					byte *tmp = new byte[zipSize + have];
					memcpy(tmp, out, zipSize);
					delete out;
					out = tmp;
				} else {
					out = new byte[have];
				}
				memcpy(out + zipSize, zbuffer, have);
				zipSize += have;
			}
		} while (strm.avail_out == 0);
	}
    
	deflateEnd(&strm);
	return out;
}

void *ZipArchive::inflateData(void *zipData, uint zipSize, uint& dataSize) {
	if (!zipData || zipSize == 0) {
		return NULL;
	}
    
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    if (inflateInit(&strm) != Z_OK) {
        return NULL;
    }
    
	strm.avail_in = zipSize;
	strm.next_in = (byte *)zipData;
    
	byte *out = NULL;
	if (dataSize > 0) {
		out = new byte[dataSize];
		strm.avail_out = dataSize;
		strm.next_out = out;
		int ret = inflate(&strm, Z_FINISH);
		if (ret != Z_OK && ret != Z_STREAM_END) {
			inflateEnd(&strm);
			delete out;
			return NULL;
		}
	} else {
	    uint have = 0;
		dataSize = 0;
		int ret;
		do {
			strm.avail_out = zbufferSize;
			strm.next_out = zbuffer;
			ret = inflate(&strm, Z_NO_FLUSH);
			if (ret != Z_OK && ret != Z_STREAM_END) {
				delete out;
				inflateEnd(&strm);
				return NULL;
			}
			have = zbufferSize - strm.avail_out;
			if (have > 0) {
				if (out) {
					byte *tmp = new byte[dataSize + have];
					memcpy(tmp, out, dataSize);
					delete out;
					out = tmp;
				} else {
					out = new byte[have];
				}
				memcpy(out + dataSize, zbuffer, have);
				dataSize += have;
			}
		} while (strm.avail_out == 0);
	}
    
    inflateEnd(&strm);
    return out;
}

